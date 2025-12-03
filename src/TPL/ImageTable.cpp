#include "TPL/ImageTable.hpp"
#include "StreamHelper.hpp"

ImageTable::ImageTable(uint32_t imageOffset, uint32_t paletteOffset)
    : _imageOffset(imageOffset), _paletteOffset(paletteOffset)
{
}

ImageTable::ImageTable(std::istream &stream)
{
    this->deserialize(stream);
}

void ImageTable::serialize(std::ostream &stream) const
{
    StreamHelper::write(stream, _imageOffset);
    StreamHelper::write(stream, _paletteOffset);
}

void ImageTable::deserialize(std::istream &stream)
{
    _imageOffset = StreamHelper::read<uint32_t>(stream);
    _paletteOffset = StreamHelper::read<uint32_t>(stream);
}

uint32_t ImageTable::getImageOffset() const
{
    return _imageOffset;
}

uint32_t ImageTable::getPaletteOffset() const
{
    return _paletteOffset;
}

size_t ImageTable::getSize() const
{
    return sizeof(_imageOffset) + sizeof(_paletteOffset);
}
