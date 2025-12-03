#include "TPL/Header.hpp"
#include "StreamHelper.hpp"

Header::Header(uint32_t numTextures)
    : _magic(MAGIC_NUMBER), _numTextures(numTextures)
{
    _offsetTableOffset = this->getSize();
}

Header::Header(std::istream &stream)
{
    this->deserialize(stream);
}

void Header::serialize(std::ostream &stream) const
{
    StreamHelper::write(stream, _magic);
    StreamHelper::write(stream, _numTextures);
    StreamHelper::write(stream, _offsetTableOffset);
}

void Header::deserialize(std::istream &stream)
{
    _magic = StreamHelper::read<uint32_t>(stream);
    if (_magic != MAGIC_NUMBER)
        throw std::runtime_error("Invalid TPL Header: incorrect magic number");
    _numTextures = StreamHelper::read<uint32_t>(stream);
    _offsetTableOffset = StreamHelper::read<uint32_t>(stream);
}

uint32_t Header::getNumTextures() const
{
    return _numTextures;
}

uint32_t Header::getOffsetTableOffset() const
{
    return _offsetTableOffset;
}

size_t Header::getSize() const
{
    return sizeof(_magic) + sizeof(_numTextures) + sizeof(_offsetTableOffset);
}
