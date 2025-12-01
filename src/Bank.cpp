#include "Bank.hpp"
#include "StreamHelper.hpp"

Bank::Bank(uint16_t frames, uint16_t padding, uint32_t frameOffset)
    : _frames(frames), _padding(padding), _frameOffset(frameOffset)
{
}

Bank::Bank(std::ifstream &stream)
{
    this->deserialize(stream);
}

void Bank::serialize(std::ofstream &stream) const
{
    StreamHelper::write(stream, _frames);
    StreamHelper::write(stream, _padding);
    StreamHelper::write(stream, _frameOffset);
}

void Bank::deserialize(std::ifstream &stream)
{
    _frames = StreamHelper::read<uint16_t>(stream);
    _padding = StreamHelper::read<uint16_t>(stream);
    _frameOffset = StreamHelper::read<uint32_t>(stream);
}

uint16_t Bank::getFrames() const
{
    return this->_frames;
}

uint16_t Bank::getPadding() const
{
    return this->_padding;
}

uint32_t Bank::getFrameOffset() const
{
    return this->_frameOffset;
}