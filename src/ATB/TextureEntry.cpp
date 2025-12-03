#include "ATB/TextureEntry.hpp"
#include "StreamHelper.hpp"

TextureEntry::TextureEntry(std::istream &stream)
{
    this->deserialize(stream);
}

TextureEntry::TextureEntry(uint8_t bpp, const Format &format, uint16_t paletteSize, uint16_t width, uint16_t height, uint32_t imageSize, uint32_t paletteOffset, uint32_t imageOffset)
    : _bpp(bpp), _format(format), _paletteSize(paletteSize), _width(width), _height(height), _imageSize(imageSize), _paletteOffset(paletteOffset), _imageOffset(imageOffset)
{
}

void TextureEntry::serialize(std::ostream &stream) const
{
    StreamHelper::write(stream, _bpp);
    StreamHelper::write(stream, _format);
    StreamHelper::write(stream, _paletteSize);
    StreamHelper::write(stream, _width);
    StreamHelper::write(stream, _height);
    StreamHelper::write(stream, _imageSize);
    StreamHelper::write(stream, _paletteOffset);
    StreamHelper::write(stream, _imageOffset);
    std::streampos lastPos = stream.tellp();
    if (_format.getValue() == Format::ATBValue::CI4 || _format.getValue() == Format::ATBValue::CI8) {
        stream.seekp(_paletteOffset, std::ios::beg);
        StreamHelper::write(stream, _paletteData.data(), _paletteSize * 2);
    }
    stream.seekp(_imageOffset, std::ios::beg);
    StreamHelper::write(stream, _imageData.data(), _imageSize);
    stream.seekp(lastPos);
}

void TextureEntry::deserialize(std::istream &stream)
{
    _bpp = StreamHelper::read<uint8_t>(stream);
    _format = StreamHelper::read<Format>(stream);
    _paletteSize = StreamHelper::read<uint16_t>(stream);
    _width = StreamHelper::read<uint16_t>(stream);
    _height = StreamHelper::read<uint16_t>(stream);
    _imageSize = StreamHelper::read<uint32_t>(stream);
    _paletteOffset = StreamHelper::read<uint32_t>(stream);
    _imageOffset = StreamHelper::read<uint32_t>(stream);
    std::streampos lastPos = stream.tellg();
    stream.seekg(_imageOffset, std::ios::beg);
    _imageData.resize(_imageSize);
    StreamHelper::read(stream, _imageData.data(), _imageSize);
    if (_format.getValue() == Format::ATBValue::CI4 || _format.getValue() == Format::ATBValue::CI8) {
        stream.seekg(_paletteOffset, std::ios::beg);
        _paletteData.resize(_paletteSize * 2);
        StreamHelper::read(stream, _paletteData.data(), _paletteSize * 2);
    }
    stream.seekg(lastPos);
}


uint8_t TextureEntry::getBpp() const
{
    return this->_bpp;
}

Format TextureEntry::getFormat() const
{
    return this->_format;
}

uint16_t TextureEntry::getPaletteSize() const
{
    return this->_paletteSize;
}

uint16_t TextureEntry::getWidth() const
{
    return this->_width;
}

uint16_t TextureEntry::getHeight() const
{
    return this->_height;
}

uint32_t TextureEntry::getImageSize() const
{
    return this->_imageSize;
}

uint32_t TextureEntry::getPaletteOffset() const
{
    return this->_paletteOffset;
}

uint32_t TextureEntry::getImageOffset() const
{
    return this->_imageOffset;
}

std::vector<uint8_t> TextureEntry::getPaletteData() const
{
    return _paletteData;
}

std::vector<uint8_t> TextureEntry::getImageData() const
{
    return _imageData;
}

size_t TextureEntry::getSize() const
{
    return sizeof(_bpp) + _format.getSize() + sizeof(_paletteSize) + sizeof(_width) + sizeof(_height) +
        sizeof(_imageSize) + sizeof(_paletteOffset) + sizeof(_imageOffset);
}

void TextureEntry::writePadding(std::fstream &stream) const
{
    stream.seekp(getSize(), std::ios::cur);
    stream.seekg(stream.tellp(), std::ios::beg);
    std::streampos lastPos = stream.tellp();
    std::streampos nextPos = (16 - static_cast<size_t>(stream.tellp()) % 16);
    if (!StreamHelper::isFree(stream, stream.tellp() + nextPos)) {
        stream.seekp(lastPos);
        return;
    }
    stream.seekp(lastPos);
    while (stream.tellp() % 16 != 0)
        StreamHelper::write<uint8_t>(stream, 0x88);
    uint32_t paletteOffset = getPaletteOffset();
    stream.seekg(paletteOffset + (_paletteSize * 2), std::ios::beg);
    if (StreamHelper::isFree(stream, _imageOffset)) {
        stream.seekp(paletteOffset + (_paletteSize * 2), std::ios::beg);
        while (stream.tellp() < _imageOffset)
            StreamHelper::write<uint8_t>(stream, 0x88);
    }
    stream.seekg(lastPos + nextPos);
    std::streampos minPos = std::min(_imageOffset, paletteOffset);
    if (!StreamHelper::isFree(stream, minPos)) {
        stream.seekp(lastPos);
        return;
    }
    stream.seekp(lastPos);
    while (stream.tellp() < minPos)
        StreamHelper::write<uint8_t>(stream, 0x88);
    stream.seekp(lastPos);
}