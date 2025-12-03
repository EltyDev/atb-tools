#include "ATB/PatternEntry.hpp"
#include "StreamHelper.hpp"

PatternEntry::PatternEntry(uint16_t layers, uint16_t centerX, uint16_t centerY, uint16_t width, uint16_t height, uint32_t layerOffset)
    : _layers(layers), _centerX(centerX), _centerY(centerY), _width(width), _height(height), _layerOffset(layerOffset), _padding(0)
{}

PatternEntry::PatternEntry(std::istream &stream)
{
    this->deserialize(stream);
}

void PatternEntry::deserialize(std::istream &stream)
{
    _layers = StreamHelper::read<uint16_t>(stream);
    _centerX = StreamHelper::read<uint16_t>(stream);
    _centerY = StreamHelper::read<uint16_t>(stream);
    _width = StreamHelper::read<uint16_t>(stream);
    _height = StreamHelper::read<uint16_t>(stream);
    _padding = StreamHelper::read<uint16_t>(stream);
    _layerOffset = StreamHelper::read<uint32_t>(stream);
}

void PatternEntry::serialize(std::ostream &stream) const
{
    StreamHelper::write(stream, _layers);
    StreamHelper::write(stream, _centerX);
    StreamHelper::write(stream, _centerY);
    StreamHelper::write(stream, _width);
    StreamHelper::write(stream, _height);
    StreamHelper::write(stream, _padding);
    StreamHelper::write(stream, _layerOffset);
}

uint16_t PatternEntry::getLayers() const
{
    return this->_layers;
}

uint16_t PatternEntry::getCenterX() const
{
    return this->_centerX;
}

uint16_t PatternEntry::getCenterY() const
{
    return this->_centerY;
}

uint16_t PatternEntry::getWidth() const
{
    return this->_width;
}

uint16_t PatternEntry::getHeight() const
{
    return this->_height;
}

uint32_t PatternEntry::getLayerOffset() const
{
    return this->_layerOffset;
}

size_t PatternEntry::getSize() const
{
    return sizeof(_layers) + sizeof(_centerX) + sizeof(_centerY) + sizeof(_width) + sizeof(_height) + sizeof(_padding) + sizeof(_layerOffset);
}