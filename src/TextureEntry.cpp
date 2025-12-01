#include "TextureEntry.hpp"
#include "StreamHelper.hpp"

TextureEntry::TextureEntry(std::ifstream &stream)
{
    this->deserialize(stream);
}

TextureEntry::TextureEntry(uint8_t bpp, const Format &format, uint16_t paletteSize, uint16_t width, uint16_t height, uint32_t imageSize, uint32_t paletteOffset, uint32_t imageOffset)
    : _bpp(bpp), _format(format), _paletteSize(paletteSize), _width(width), _height(height), _imageSize(imageSize), _paletteOffset(paletteOffset), _imageOffset(imageOffset)
{
}

void TextureEntry::serialize(std::ofstream &stream) const
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
    stream.seekp(_imageOffset, std::ios::beg);
    StreamHelper::write(stream, _imageData.data(), _imageSize);
    if (_format.getValue() == Format::Value::C4 || _format.getValue() == Format::Value::C8) {
        stream.seekp(_paletteOffset, std::ios::beg);
        StreamHelper::write(stream, _paletteData.data(), _paletteSize);
    }
    stream.seekp(lastPos);
}

void TextureEntry::deserialize(std::ifstream &stream)
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
    if (_format.getValue() == Format::Value::C4 || _format.getValue() == Format::Value::C8) {
        stream.seekg(_paletteOffset, std::ios::beg);
        _paletteData.resize(_paletteSize);
        StreamHelper::read(stream, _paletteData.data(), _paletteSize);
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

Format TextureEntry::asGXFormat() const
{
    Format gxFormat = _format;
    switch (_format.getValue()) {
        case Format::Value::C8:
            if (_bpp == 24)
                throw std::runtime_error("C8 format with 24 bpp cannot be converted to GX format");
            break;
        case Format::Value::A8:
            gxFormat = Format(Format::Value::RGBA32);
            break;
        default:
            break;
    }
    return gxFormat;
}

std::vector<uint8_t> TextureEntry::getPaletteData() const
{
    return _paletteData;
}

std::vector<uint8_t> TextureEntry::getImageData() const
{
    return _imageData;
}