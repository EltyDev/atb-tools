#include "TPL/ImageHeader.hpp"
#include "StreamHelper.hpp"

ImageHeader::ImageHeader(uint16_t height, uint16_t width, const Format &format, uint32_t dataOffset, uint32_t wrapS, uint32_t wrapT, uint32_t minFilter, uint32_t magFilter, float lodBias, uint8_t edgeLod, uint8_t minLod, uint8_t maxLod, uint8_t unpacked, const std::vector<uint8_t> &data)
    : _height(height), _width(width), _format(format), _dataOffset(dataOffset), _wraps(wrapS), _wrapt(wrapT), _minfilter(minFilter), _magfilter(magFilter), _lodbias(lodBias), _edgelod(edgeLod), _minlod(minLod), _maxlod(maxLod), _unpacked(unpacked), _data(data)
{
}

ImageHeader::ImageHeader(std::istream &stream)
{
    this->deserialize(stream);
}

#include <iostream>

void ImageHeader::serialize(std::ostream &stream) const
{
    std::cout << "Height : " << _height << ", Width : " << _width << ", DataOffset : " << _dataOffset << std::endl;
    StreamHelper::write(stream, _height);
    StreamHelper::write(stream, _width);
    StreamHelper::write(stream, static_cast<uint32_t>(_format.getGXValue()));
    StreamHelper::write(stream, _dataOffset);
    StreamHelper::write(stream, _wraps);
    StreamHelper::write(stream, _wrapt);
    StreamHelper::write(stream, _minfilter);
    StreamHelper::write(stream, _magfilter);
    StreamHelper::write(stream, _lodbias);
    StreamHelper::write(stream, _edgelod);
    StreamHelper::write(stream, _minlod);
    StreamHelper::write(stream, _maxlod);
    StreamHelper::write(stream, _unpacked);
    std::streampos lastPos = stream.tellp();
    stream.seekp(_dataOffset, std::ios::beg);
    StreamHelper::write(stream, _data.data(), _data.size());
    stream.seekp(lastPos);
}

void ImageHeader::deserialize(std::istream &stream)
{
    _height = StreamHelper::read<uint16_t>(stream);
    _width = StreamHelper::read<uint16_t>(stream);
    _format = Format(static_cast<Format::ATBValue>(StreamHelper::read<uint32_t>(stream)));
    _dataOffset = StreamHelper::read<uint32_t>(stream);
    _wraps = StreamHelper::read<uint32_t>(stream);
    _wrapt = StreamHelper::read<uint32_t>(stream);
    _minfilter = StreamHelper::read<uint32_t>(stream);
    _magfilter = StreamHelper::read<uint32_t>(stream);
    _lodbias = StreamHelper::read<float>(stream);
    _edgelod = StreamHelper::read<uint8_t>(stream);
    _minlod = StreamHelper::read<uint8_t>(stream);
    _maxlod = StreamHelper::read<uint8_t>(stream);
    _unpacked = StreamHelper::read<uint8_t>(stream);
    std::streampos lastPos = stream.tellg();
    stream.seekg(_dataOffset, std::ios::beg);
    size_t dataSize = 0;
    switch (_format.getValue()) {
        case Format::ATBValue::RGBA8:
            dataSize = _width * _height * 4;
            break;
        case Format::ATBValue::RGB5A3:
        case Format::ATBValue::RGB5A3_DUPE:
        case Format::ATBValue::I8:
        case Format::ATBValue::IA8:
        case Format::ATBValue::CI8:
        case Format::ATBValue::A8:
            dataSize = _width * _height * 2;
            break;
        case Format::ATBValue::I4:
        case Format::ATBValue::IA4:
        case Format::ATBValue::CI4:
            dataSize = _width * _height;
            break;
        case Format::ATBValue::CMPR:
            dataSize = (_width * _height) / 2;
            break;
        default:
            dataSize = 0;
            break;
    }
    _data.resize(dataSize);
    StreamHelper::read(stream, _data.data(), dataSize);
    stream.seekg(lastPos);
}

uint16_t ImageHeader::getHeight() const
{
    return this->_height;
}

uint16_t ImageHeader::getWidth() const
{
    return this->_width;
}
Format ImageHeader::getFormat() const
{
    return this->_format;
}

uint32_t ImageHeader::getDataOffset() const
{
    return this->_dataOffset;
}

uint32_t ImageHeader::getWrapS() const
{
    return this->_wraps;
}

uint32_t ImageHeader::getWrapT() const
{
    return this->_wrapt;
}

uint32_t ImageHeader::getMinFilter() const
{
    return this->_minfilter;
}

uint32_t ImageHeader::getMagFilter() const
{
    return this->_magfilter;
}

float ImageHeader::getLodBias() const
{
    return this->_lodbias;
}

uint8_t ImageHeader::getEdgeLod() const
{
    return this->_edgelod;
}

uint8_t ImageHeader::getMinLod() const
{
    return this->_minlod;
}

uint8_t ImageHeader::getMaxLod() const
{
    return this->_maxlod;
}

uint8_t ImageHeader::getUnpacked() const
{
    return this->_unpacked;
}

const std::vector<uint8_t> &ImageHeader::getData() const
{
    return this->_data;
}

size_t ImageHeader::getSize() const
{
    return sizeof(_height) + sizeof(_width) + sizeof(uint32_t) /* format */ + sizeof(_dataOffset) + sizeof(_wraps) + sizeof(_wrapt) + sizeof(_minfilter) + sizeof(_magfilter) + sizeof(_lodbias) + sizeof(_edgelod) + sizeof(_minlod) + sizeof(_maxlod) + sizeof(_unpacked);
}