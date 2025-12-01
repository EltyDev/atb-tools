#include "Frame.hpp"
#include "StreamHelper.hpp"

Frame::Frame(uint16_t patternIndex, uint16_t length, uint16_t shiftX, uint16_t shiftY, uint16_t flip)
    : _patternIndex(patternIndex), _length(length), _shiftX(shiftX), _shiftY(shiftY), _flip(flip), _unk(0)
{

}

void Frame::serialize(std::ofstream &stream) const
{
    StreamHelper::write(stream, _patternIndex);
    StreamHelper::write(stream, _length);
    StreamHelper::write(stream, _shiftX);
    StreamHelper::write(stream, _shiftY);
    StreamHelper::write(stream, _flip);
    StreamHelper::write(stream, _unk);
}

void Frame::deserialize(std::ifstream &stream)
{
    _patternIndex = StreamHelper::read<uint16_t>(stream);
    _length = StreamHelper::read<uint16_t>(stream);
    _shiftX = StreamHelper::read<uint16_t>(stream);
    _shiftY = StreamHelper::read<uint16_t>(stream);
    _flip = StreamHelper::read<uint16_t>(stream);
    _unk = StreamHelper::read<uint16_t>(stream);
}

uint16_t Frame::getPatternIndex() const
{
    return this->_patternIndex;
}

uint16_t Frame::getLength() const
{
    return this->_length;
}

uint16_t Frame::getShiftX() const
{
    return this->_shiftX;
}

uint16_t Frame::getShiftY() const
{
    return this->_shiftY;
}

uint16_t Frame::getFlip() const
{
    return this->_flip;
}