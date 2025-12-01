#include "ATBFile.hpp"
#include "StreamHelper.hpp"
#include "TextureEntry.hpp"

ATBFile::ATBFile(const std::filesystem::path path)
{
    std::ifstream file(path, std::ios::binary);
    if (!file.is_open())
        throw std::runtime_error("Could not open file: " + path.string());
    this->deserialize(file);
    file.close();
}

void ATBFile::deserialize(std::ifstream &stream)
{
    if (!stream.is_open())
        throw std::runtime_error("Stream is not open");
    if (stream.left < 16)
        throw std::runtime_error("Stream is too small to be a valid ATB file");

    uint16_t numBanks = StreamHelper::read<uint16_t>(stream);
    uint16_t numPatterns = StreamHelper::read<uint16_t>(stream);
    uint16_t numTextures = StreamHelper::read<uint16_t>(stream);
    uint16_t numReferences = StreamHelper::read<uint16_t>(stream);
    uint32_t bankOffset = StreamHelper::read<uint32_t>(stream);
    uint32_t patternOffset = StreamHelper::read<uint32_t>(stream);

    if (patternOffset != 20)
        throw std::runtime_error("Invalid ATB file: pattern offset is not 20");

    uint32_t textureOffset = StreamHelper::read<uint32_t>(stream);

    _textures.resize(numTextures);
    StreamHelper::read(stream, _textures.data(), numTextures);
    _banks.resize(numBanks);
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
    _patterns.resize(numPatterns);
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

void ATBFile::serialize(std::ofstream &stream) const
{
    // Implementation of serialization logic goes here
}
