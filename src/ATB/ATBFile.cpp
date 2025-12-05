#include "ATB/ATBFile.hpp"
#include "TPL/TPLFile.hpp"
#include "StreamHelper.hpp"
#include <pugixml.hpp>

ATBFile::ATBFile(const std::filesystem::path path) : _path(path)
{
    std::ifstream file(_path, std::ios::binary);
    if (!file.is_open())
        throw std::runtime_error("Could not open file: " + _path.string());
    this->deserialize(file);
    file.close();
}

void ATBFile::deserialize(std::istream &stream)
{
    if (!stream.good())
        throw std::runtime_error("Stream is not open");
    stream.seekg(0, std::ios::end);
    std::streampos streamSize = stream.tellg();
    stream.seekg(0, std::ios::beg);
    if (streamSize < 20)
        throw std::runtime_error("Stream is too small to be a valid ATB file");

    uint16_t numBanks = StreamHelper::read<uint16_t>(stream);
    uint16_t numPatterns = StreamHelper::read<uint16_t>(stream);
    uint16_t numTextures = StreamHelper::read<uint16_t>(stream);
    stream.seekg(2, std::ios::cur);
    _bankOffset = StreamHelper::read<uint32_t>(stream);
    _patternOffset = StreamHelper::read<uint32_t>(stream);

    if (_patternOffset != 20)
        throw std::runtime_error("Invalid ATB file: pattern offset is not 20");

    _textureOffset = StreamHelper::read<uint32_t>(stream);
    stream.seekg(_textureOffset, std::ios::beg);
    _textures.resize(numTextures);
    StreamHelper::read(stream, _textures.data(), numTextures);
    stream.seekg(_bankOffset, std::ios::beg);
    _banks.reserve(numBanks);
    for (uint16_t i = 0; i < numBanks; i++) {
        Bank &bank = _banks.emplace_back(stream);
        std::streampos lastPos = stream.tellg();
        stream.seekg(bank.getFrameOffset(), std::ios::beg);
        size_t currentSize = _frames.size();
        size_t nbFrames = bank.getFrames();
        _frames.resize(_frames.size() + nbFrames);
        StreamHelper::read(stream, _frames.data() + currentSize, nbFrames);
        stream.seekg(lastPos);
    }
    stream.seekg(_patternOffset, std::ios::beg);
    _patterns.reserve(numPatterns);
    for (uint16_t i = 0; i < numPatterns; i++) {
        PatternEntry &pattern = _patterns.emplace_back(stream);
        std::streampos lastPos = stream.tellg();
        stream.seekg(pattern.getLayerOffset(), std::ios::beg);
        size_t currentSize = _layers.size();
        size_t nbLayers = pattern.getLayers();
        _layers.resize(_layers.size() + nbLayers);
        StreamHelper::read(stream, _layers.data() + currentSize, nbLayers);
        stream.seekg(lastPos);
    }
}

void ATBFile::serialize(std::ostream &stream) const
{
    StreamHelper::write<uint16_t>(stream, static_cast<uint16_t>(_banks.size()));
    StreamHelper::write<uint16_t>(stream, static_cast<uint16_t>(_patterns.size()));
    StreamHelper::write<uint16_t>(stream, static_cast<uint16_t>(_textures.size()));
    StreamHelper::write<uint16_t>(stream, 0);
    StreamHelper::write<uint32_t>(stream, _bankOffset);
    StreamHelper::write<uint32_t>(stream, _patternOffset);
    StreamHelper::write<uint32_t>(stream, _textureOffset);
    stream.seekp(_textureOffset, std::ios::beg);
    StreamHelper::write<TextureEntry>(stream, _textures.data(), _textures.size());
    uint32_t frameIndex = 0;
    stream.seekp(_bankOffset, std::ios::beg);
    for (const Bank &bank : _banks) {
        bank.serialize(stream);
        std::streampos lastPos = stream.tellp();
        stream.seekp(bank.getFrameOffset(), std::ios::beg);
        for (uint16_t i = 0; i < bank.getFrames(); i++)
            _frames[frameIndex + i].serialize(stream);
        frameIndex += bank.getFrames();
        stream.seekp(lastPos);
    }
    uint32_t layerIndex = 0;
    stream.seekp(_patternOffset, std::ios::beg);
    for (const PatternEntry &pattern : _patterns) {
        pattern.serialize(stream);
        std::streampos lastPos = stream.tellp();
        stream.seekp(pattern.getLayerOffset(), std::ios::beg);
        for (uint16_t i = 0; i < pattern.getLayers(); i++) {
            _layers[layerIndex + i].serialize(stream);
        }
        layerIndex += pattern.getLayers();
        stream.seekp(lastPos);
    }
}

ATBFile::ATBFile(const TPLFile &tplFile, pugi::xml_document &doc)
{
    _patternOffset = 20;
    uint32_t textureCount = tplFile.getHeader().getNumTextures();
    _textures.resize(textureCount);
    uint32_t layerOffset = _patternOffset + (_patterns.size() * sizeof(TextureEntry));
    _bankOffset = layerOffset + (_layers.size() * sizeof(Layer));
    uint32_t frameOffset = _bankOffset + (_banks.size() * sizeof(Bank));
    _textureOffset = frameOffset + (_frames.size() * sizeof(Frame));
    uint32_t textureDataOffset = _textureOffset + (textureCount * sizeof(TextureEntry));
    textureDataOffset = (textureDataOffset + TEXTURE_ALIGNMENT - 1) & ~(TEXTURE_ALIGNMENT - 1);
    pugi::xml_node patterns = doc.child("patterns");
    pugi::xml_node tmp = patterns.first_child();
    size_t i = 0;
    while (tmp) {
        tmp = tmp.next_sibling();
        std::string name = tmp.name();
        if (!name.empty() && name == "pattern") {
            pugi::xml_node child = tmp.first_child().next_sibling().first_child();
            uint16_t layers = child.text().as_uint();
            child = tmp.next_sibling().next_sibling().first_child();
            uint16_t centerX = child.text().as_int();
            child = tmp.next_sibling().next_sibling().next_sibling().first_child();
            uint16_t centerY = child.text().as_int();
            child = tmp.next_sibling().next_sibling().next_sibling().next_sibling().first_child();
            uint16_t width = child.text().as_uint();
            child = tmp.next_sibling().next_sibling().next_sibling().next_sibling().next_sibling().first_child();
            uint16_t height = child.text().as_uint();
            _patterns[i] = PatternEntry(layers, centerX, centerY, width, height, frameOffset);
        }
    }

}

void ATBFile::pack()
{
    pugi::xml_document doc;
    doc.load_file(_path.string().c_str());
    pugi::xml_node banks = doc.child("banks");
    pugi::xml_node texture = doc.child("texture");
    std::string textureName = texture.first_child().text().as_string();
    if (!banks)
        throw std::runtime_error("No banks node found in XML file");
    if (!texture || textureName.empty())
        throw std::runtime_error("No texture node found in XML file");
    std::filesystem::path tplPath = _path.parent_path() / textureName;
    TPLFile tplFile(tplPath);
    pugi::xml_node tmp = banks.first_child();
    size_t numBanks = 0;
	while (tmp) {
        tmp = tmp.next_sibling();
        std::string name = tmp.name();
        if (!name.empty() && name == "bank")
            numBanks++;
    }
    _banks.resize(numBanks);
    pugi::xml_node frames = doc.child("frames");
    if (!frames)
        throw std::runtime_error("No frames node found in XML file");
    tmp = frames.first_child();
    size_t numFrames = 0;
    std::vector<std::string> frameNames;
    while (tmp) {
        tmp = tmp.next_sibling();
        std::string name = tmp.name();
        if (!name.empty() && name == "frame") {
            pugi::xml_node node = tmp.first_child();
            while (node) {
                node = node.next_sibling();
                std::string nodeName = node.name();
                if (!nodeName.empty())
                    frameNames.emplace_back(nodeName);
            }
        }
    }
    _frames.resize(frameNames.size());
    pugi::xml_node patterns = doc.child("patterns");
    if (!patterns)
        throw std::runtime_error("No patterns node found in XML file");
    tmp = patterns.first_child();
    size_t numPatterns = 0;
    std::vector<std::string> patternNames;
    while (tmp) {
        tmp = tmp.next_sibling();
        std::string name = tmp.name();
        if (!name.empty() && name == "pattern")
            patternNames.emplace_back(name);
    }
    _patterns.resize(patternNames.size());
    pugi::xml_node layers = doc.child("layers");
    if (!layers)
        throw std::runtime_error("No layers node found in XML file");
    tmp = layers.first_child();
    std::vector<std::string> layerNames;
    while (tmp) {
        tmp = tmp.next_sibling();
        std::string name = tmp.name();
        if (!name.empty() && name == "layer") {
            int i = 0;
            pugi::xml_node node = tmp.first_child();
            while (node) {
                node = node.next_sibling();
                std::string nodeName = node.name();
                if (!nodeName.empty())
                    layerNames.emplace_back(nodeName);
            }
        }
    }
    _layers.resize(layerNames.size());
    tmp = patterns.first_child();
    *this = ATBFile(tplFile, doc);
}

void ATBFile::unpack()
{
    TPLFile tplFile(*this);
    std::filesystem::path tplPath = _path;
    if (!std::filesystem::exists(_path.stem()) && !std::filesystem::create_directory(_path.stem()))
        throw std::runtime_error("Could not create directory: " + _path.stem().string());
    tplPath = _path.stem() / tplPath.filename();
    tplPath.replace_extension(".tpl");
    std::ofstream tplOutput(tplPath,  std::ios::binary);
    if (!tplOutput.is_open())
        throw std::runtime_error("Could not open file: " + tplPath.string());
    tplFile.serialize(tplOutput);
    tplOutput.close();
    pugi::xml_document doc;
    pugi::xml_node banks = doc.append_child("banks");
    for (size_t i = 0; i < _banks.size(); i++) {
        pugi::xml_node bank = banks.append_child("bank");
        bank.append_attribute("name").set_value(("BANK" + std::to_string(i)).c_str());
        pugi::xml_node frame_count = bank.append_child("frame_count");
        frame_count.text() = _banks[i].getFrames();
        pugi::xml_node first_frame = bank.append_child("first_frame");
        first_frame.text() = ("FRAME" + std::to_string(i)).c_str();
    }
    pugi::xml_node frames = doc.append_child("frames");
    size_t frameIndex = 0;
    for (size_t i = 0; i < _banks.size(); i++) {
        pugi::xml_node frame = frames.append_child("frame");
        frame.append_attribute("name").set_value(("FRAME" + std::to_string(frameIndex)).c_str());
        for (size_t j = 0; j < _banks[i].getFrames(); j++) {
            Frame &currentFrame = _frames[frameIndex + j];
            if (currentFrame.getLength() == -1)
                frame.append_child("pattern").text() = "NULL";
            else
                frame.append_child("pattern").text() = ("PATTERN" + std::to_string(currentFrame.getPatternIndex())).c_str();
            frame.append_child("frame_length").text() = std::to_string(currentFrame.getLength()).c_str();
            frame.append_child("shift_x").text() = std::to_string(currentFrame.getShiftX()).c_str();
            frame.append_child("shift_y").text() = std::to_string(currentFrame.getShiftY()).c_str();
            frame.append_child("flip").text() = std::to_string(currentFrame.getFlip()).c_str();
        }
        frameIndex += _banks[i].getFrames();
    }
    size_t layerIndex = 0;
    pugi::xml_node patterns = doc.append_child("patterns");
    pugi::xml_node layers = doc.append_child("layers");
    for (size_t i = 0; i < _patterns.size(); i++) {
        pugi::xml_node pattern = patterns.append_child("pattern");
        pattern.append_attribute("name").set_value(("PATTERN" + std::to_string(i)).c_str());
        pattern.append_child("layer_count").text() = std::to_string(_patterns[i].getLayers()).c_str();
        pattern.append_child("center_x").text() = std::to_string(_patterns[i].getCenterX()).c_str();
        pattern.append_child("center_y").text() = std::to_string(_patterns[i].getCenterY()).c_str();
        pattern.append_child("width").text() = std::to_string(_patterns[i].getWidth()).c_str();
        pattern.append_child("height").text() = std::to_string(_patterns[i].getHeight()).c_str();
        pattern.append_child("layer").text() = ("LAYER" + std::to_string(layerIndex)).c_str();

        pugi::xml_node layer = layers.append_child("layer");
        layer.append_attribute("name").set_value(("LAYER" + std::to_string(layerIndex)).c_str());
        for (size_t j = 0; j < _patterns[i].getLayers(); j++) {
            Layer &currentLayer = _layers[layerIndex + j];
            layer.append_child("alpha").text() = std::to_string(currentLayer.getAlpha()).c_str();
            layer.append_child("flip").text() = std::to_string(currentLayer.getFlip()).c_str();
            layer.append_child("texture_index").text() = std::to_string(currentLayer.getTextureIndex()).c_str();
            layer.append_child("texcoord_upper_left_x").text() = std::to_string(currentLayer.getTexcoordUpperLeftX()).c_str();
            layer.append_child("texcoord_upper_left_y").text() = std::to_string(currentLayer.getTexcoordUpperLeftY()).c_str();
            layer.append_child("texcoord_width").text() = std::to_string(currentLayer.getTexcoordWidth()).c_str();
            layer.append_child("texcoord_height").text() = std::to_string(currentLayer.getTexcoordHeight()).c_str();
            layer.append_child("shift_x").text() = std::to_string(currentLayer.getShiftX()).c_str();
            layer.append_child("shift_y").text() = std::to_string(currentLayer.getShiftY()).c_str();
            layer.append_child("upper_left_vertex_x").text() = std::to_string(currentLayer.getUpperLeftVertexX()).c_str();
            layer.append_child("upper_left_vertex_y").text() = std::to_string(currentLayer.getUpperLeftVertexY()).c_str();
            layer.append_child("upper_right_vertex_x").text() = std::to_string(currentLayer.getUpperRightVertexX()).c_str();
            layer.append_child("upper_right_vertex_y").text() = std::to_string(currentLayer.getUpperRightVertexY()).c_str();
            layer.append_child("lower_right_vertex_x").text() = std::to_string(currentLayer.getLowerRightVertexX()).c_str();
            layer.append_child("lower_right_vertex_y").text() = std::to_string(currentLayer.getLowerRightVertexY()).c_str();
            layer.append_child("lower_left_vertex_x").text() = std::to_string(currentLayer.getLowerLeftVertexX()).c_str();
            layer.append_child("lower_left_vertex_y").text() = std::to_string(currentLayer.getLowerLeftVertexY()).c_str();
        }
        layerIndex += _patterns[i].getLayers();
    }
    doc.append_child("texture").text() = tplPath.filename().string().c_str();
    tplPath.replace_extension(".xml");
    doc.save_file(tplPath.c_str());
}

const std::vector<PatternEntry> &ATBFile::getPatterns() const
{
    return this->_patterns;
}

const std::vector<Bank> &ATBFile::getBanks() const
{
    return this->_banks;
}

const std::vector<TextureEntry> &ATBFile::getTextures() const
{
    return this->_textures;
}

const std::vector<Frame> &ATBFile::getFrames() const
{
    return this->_frames;
}

const std::vector<Layer> &ATBFile::getLayers() const
{
    return this->_layers;
}

uint32_t ATBFile::getBankOffset() const
{
    return this->_bankOffset;
}

uint32_t ATBFile::getPatternOffset() const
{
    return this->_patternOffset;
}

uint32_t ATBFile::getTextureOffset() const
{
    return this->_textureOffset;
}

size_t ATBFile::getSize() const
{
    size_t size = 20;
    size += _textures.size() * sizeof(TextureEntry);
    for (const Bank &bank : _banks) {
        size += sizeof(Bank);
        size += bank.getFrames() * sizeof(Frame);
    }
    for (const PatternEntry &pattern : _patterns) {
        size += sizeof(PatternEntry);
        size += pattern.getLayers() * sizeof(Layer);
    }
    return size;
}

void ATBFile::exportToFile(const std::filesystem::path path) const
{
    std::fstream output(path, std::ios::binary | std::ios::in | std::ios::out | std::ios::trunc);
    if (!output.is_open())
        throw std::runtime_error("Could not create file: " + path.string());
    serialize(output);
    output.seekp(_textureOffset, std::ios::beg);
    std::vector<uint32_t> _offsets;
    for (const TextureEntry &texture : getTextures()) {
        _offsets.emplace_back(texture.getImageOffset());
        _offsets.emplace_back(texture.getPaletteOffset());
        texture.writePadding(output, _offsets);
    }
    output.close();
}