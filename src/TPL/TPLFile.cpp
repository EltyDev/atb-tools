#include "TPL/TPLFile.hpp"
#include "StreamHelper.hpp"
#include "ATB/ATBFile.hpp"
#include "TPL/ImageHeader.hpp"

TPLFile::TPLFile(const std::filesystem::path path)
{
    std::ifstream file(path, std::ios::binary);
    if (!file.is_open())
        throw std::runtime_error("Could not open file: " + path.string());
    this->deserialize(file);
    file.close();
}

TPLFile::TPLFile(ATBFile &atbFile) {
    size_t textureNumber = atbFile.getTextures().size();
    _header = Header(textureNumber);
    _imageTables.reserve(textureNumber);
    _images.reserve(textureNumber);
    size_t textureHeadersOffset = _header.getSize() + textureNumber * ImageTable().getSize();
    size_t paletteHeadersOffset = textureHeadersOffset  + textureNumber * ImageHeader().getSize();
    size_t dataOffset = paletteHeadersOffset + textureNumber * PaletteHeader().getSize();
    dataOffset = ((dataOffset - 1) | (TPL_TEXTURE_ALIGNMENT - 1)) + 1;
    for (const TextureEntry &texture : atbFile.getTextures()) {
        _images.emplace_back(texture.getHeight(), texture.getHeight(), texture.getFormat(), dataOffset, 0, 0, 1, 1, 0.f, 0, 0, 0, 0, texture.getImageData());
        dataOffset += texture.getImageSize();
        dataOffset = ((dataOffset - 1) | (TPL_TEXTURE_ALIGNMENT - 1)) + 1;
    }
    dataOffset = ((dataOffset - 1) | (TPL_TEXTURE_ALIGNMENT - 1)) + 1;
    for (const TextureEntry &texture : atbFile.getTextures()) {
        if (texture.getPaletteSize() == 0) continue;
        _palettes.emplace_back(texture.getPaletteSize(), 0, dataOffset, Format(Format::ATBValue::RGB5A3_DUPE), texture.getPaletteData());
        dataOffset += texture.getPaletteSize() * 2; 
        dataOffset = ((dataOffset - 1) | (TPL_TEXTURE_ALIGNMENT - 1)) + 1;
    }
    for (const TextureEntry &texture : atbFile.getTextures()) {
        if (texture.getPaletteSize() > 0) {
            _imageTables.emplace_back(textureHeadersOffset, paletteHeadersOffset);
            paletteHeadersOffset += PaletteHeader().getSize();
        } else
            _imageTables.emplace_back(textureHeadersOffset, 0);
        textureHeadersOffset += ImageHeader().getSize();
    }

}

void TPLFile::serialize(std::ostream &stream) const
{
    _header.serialize(stream);
    stream.seekp(_header.getOffsetTableOffset());
    StreamHelper::write(stream, _imageTables.data(), _imageTables.size());
    for (size_t i = 0, j = 0; i < _imageTables.size(); i++) {
        stream.seekp(_imageTables[i].getImageOffset(), std::ios::beg);
        _images[i].serialize(stream);
        if (_imageTables[i].getPaletteOffset() != 0) {
            stream.seekp(_imageTables[i].getPaletteOffset(), std::ios::beg);
            _palettes[j].serialize(stream);
            ++j;
        }
    }
}

void TPLFile::deserialize(std::istream &stream)
{
    _header.deserialize(stream);
    _imageTables.resize(_header.getNumTextures());
    stream.seekg(_header.getOffsetTableOffset());
    StreamHelper::read(stream, _imageTables.data(), _header.getNumTextures());
    _images.reserve(_header.getNumTextures());
    for (const ImageTable &imageTable : _imageTables) {
        stream.seekg(imageTable.getImageOffset(), std::ios::beg);
        _images.emplace_back(stream);
        if (imageTable.getPaletteOffset() != 0) {
            stream.seekg(imageTable.getPaletteOffset(), std::ios::beg);
            _palettes.emplace_back(stream);
        }
    }
}

const Header &TPLFile::getHeader() const
{
    return _header;
}

const std::vector<ImageTable> &TPLFile::getImageTables() const
{
    return _imageTables;
}

const std::vector<PaletteHeader> &TPLFile::getPalettes() const
{
    return _palettes;
}

const std::vector<ImageHeader> &TPLFile::getImages() const
{
    return _images;
}

size_t TPLFile::getSize() const
{
    size_t size = _header.getSize();
    for (const ImageTable &imageTable : _imageTables)
        size += imageTable.getSize();
    for (const PaletteHeader &palette : _palettes)
        size += palette.getSize();
    for (const ImageHeader &image : _images)
        size += image.getSize();
    return size;
}