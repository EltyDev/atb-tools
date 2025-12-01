#include "FrameEntry.hpp"
#include "StreamHelper.hpp"

FrameEntry::FrameEntry(uint16_t patternIndex, uint16_t frameLength, uint16_t shiftX, uint16_t shiftY, uint16_t flip)
    : _patternIndex(patternIndex), _frameLength(frameLength), _shiftX(shiftX), _shiftY(shiftY), _flip(flip) {}

FrameEntry::FrameEntry(std::ifstream &stream)
{
    this->deserialize(stream);
}

void FrameEntry::deserialize(std::ifstream &stream)
{
    _patternIndex = StreamHelper::read<uint16_t>(stream);
    _frameLength = StreamHelper::read<uint16_t>(stream);
    _shiftX = StreamHelper::read<uint16_t>(stream);
    _shiftY = StreamHelper::read<uint16_t>(stream);
    _flip = StreamHelper::read<uint16_t>(stream);
}

void FrameEntry::serialize(std::ofstream &stream) const
{
    StreamHelper::write(stream, _patternIndex);
    StreamHelper::write(stream, _frameLength);
    StreamHelper::write(stream, _shiftX);
    StreamHelper::write(stream, _shiftY);
    StreamHelper::write(stream, _flip);
}


uint16_t FrameEntry::getPatternIndex() const
{
    return this->_patternIndex;
}

uint16_t FrameEntry::getFrameLength() const
{
    return this->_frameLength;
}

uint16_t FrameEntry::getShiftX() const
{
    return this->_shiftX;
}

uint16_t FrameEntry::getShiftY() const
{
    return this->_shiftY;
}

uint16_t FrameEntry::getFlip() const
{
    return this->_flip;
}

