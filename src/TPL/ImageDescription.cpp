#include "TPL/ImageDescription.hpp"
#include "StreamHelper.hpp"

ImageDescription::ImageDescription(uint32_t imageOffset, uint32_t paletteOffset)
    : _imageOffset(imageOffset), _paletteOffset(paletteOffset)
{
}

ImageDescription::ImageDescription(std::istream &stream)
{
    this->deserialize(stream);
}

void ImageDescription::serialize(std::ostream &stream) const
{
    StreamHelper::write(stream, _imageOffset);
    StreamHelper::write(stream, _paletteOffset);
}

void ImageDescription::deserialize(std::istream &stream)
{
    _imageOffset = StreamHelper::read<uint32_t>(stream);
    _paletteOffset = StreamHelper::read<uint32_t>(stream);
}

uint32_t ImageDescription::getImageOffset() const
{
    return _imageOffset;
}

uint32_t ImageDescription::getPaletteOffset() const
{
    return _paletteOffset;
}

size_t ImageDescription::getSize() const
{
    return sizeof(_imageOffset) + sizeof(_paletteOffset);
}

