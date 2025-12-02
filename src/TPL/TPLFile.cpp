#include "TPL/TPLFile.hpp"
#include "StreamHelper.hpp"
#include "ATB/ATBFile.hpp"

TPLFile::TPLFile(const std::filesystem::path path)
{
    std::ifstream file(path, std::ios::binary);
    if (!file.is_open())
        throw std::runtime_error("Could not open file: " + path.string());
    this->deserialize(file);
    file.close();
}

TPLFile::TPLFile(ATBFile &atbFile) {
    _header = Header(atbFile.getTextures().size());
    _imageTables.reserve(atbFile.getTextures().size());
    _palettes.reserve(atbFile.getTextures().size());
    _images.reserve(atbFile.getTextures().size());
    size_t ImageTableSize = ImageTable().getSize();
    size_t imageHeaderSize = ImageHeader().getSize();
    size_t paletteHeaderSize = PaletteHeader().getSize();
    uint32_t currentOffset = _header.getSize() + atbFile.getTextures().size() * ImageTableSize;
    for (TextureEntry &texture : atbFile.getTextures()) {
        uint32_t paletteOffset = 0;
        _imageTables.emplace_back(currentOffset, currentOffset + imageHeaderSize);
        size_t imageOffset = currentOffset  + (_palettes.size() * paletteHeaderSize);
        imageOffset = ((imageOffset - 1)|(TEXTURE_ALIGNMENT-1))+1;
        _images.emplace_back(texture.getHeight(), texture.getWidth(), texture.getFormat(), imageOffset, 0, 0, 1, 1, 0.0f, 0, 0, 0, 0, texture.getImageData());
        if (texture.getFormat().getValue() == Format::ATBValue::CI4 || texture.getFormat().getValue() == Format::ATBValue::CI8) {
            paletteOffset = currentOffset;
            _palettes.emplace_back(texture.getPaletteSize(), 0, paletteOffset + paletteHeaderSize, texture.getFormat(), texture.getPaletteData());
            currentOffset += _palettes[0].getSize();
        } else {
            _palettes.emplace_back();
        }
        currentOffset += imageHeaderSize + texture.getImageSize();
    }
}

#include <iostream>

void TPLFile::serialize(std::ofstream &stream) const
{
    _header.serialize(stream);
    StreamHelper::write(stream, _imageTables.data(), _imageTables.size());
    std::cout << _header.getNumTextures() << " textures found in TPL file." << std::endl;
    for (size_t i = 0; i < _imageTables.size(); i++) {
        stream.seekp(_imageTables[i].getImageOffset(), std::ios::beg);
        _images[i].serialize(stream);
        if (_imageTables[i].getPaletteOffset() != 0) {
            stream.seekp(_imageTables[i].getPaletteOffset(), std::ios::beg);
            _palettes[i].serialize(stream);
        }
    }
}

void TPLFile::deserialize(std::ifstream &stream)
{
    _header.deserialize(stream);
    _imageTables.resize(_header.getNumTextures());
    StreamHelper::read(stream, _imageTables.data(), _header.getNumTextures());
    _images.reserve(_header.getNumTextures());
    _palettes.reserve(_header.getNumTextures());
    for (const ImageTable &imageTable : _imageTables) {
        stream.seekg(imageTable.getImageOffset(), std::ios::beg);
        _images.emplace_back(stream);
        stream.seekg(imageTable.getPaletteOffset(), std::ios::beg);
        _palettes.emplace_back(stream);
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