#include "ATB/Bank.hpp"
#include "StreamHelper.hpp"

Bank::Bank(uint16_t frames, uint32_t frameOffset)
    : _frames(frames), _frameOffset(frameOffset), _padding(0)
{
}

Bank::Bank(std::istream &stream)
{
    this->deserialize(stream);
}

void Bank::serialize(std::ostream &stream) const
{
    StreamHelper::write(stream, _frames);
    StreamHelper::write(stream, _padding);
    StreamHelper::write(stream, _frameOffset);
}

void Bank::deserialize(std::istream &stream)
{
    _frames = StreamHelper::read<uint16_t>(stream);
    _padding = StreamHelper::read<uint16_t>(stream);
    _frameOffset = StreamHelper::read<uint32_t>(stream);
}

uint16_t Bank::getFrames() const
{
    return this->_frames;
}

uint32_t Bank::getFrameOffset() const
{
    return this->_frameOffset;
}

size_t Bank::getSize() const
{
    return sizeof(_frames) + sizeof(_padding) + sizeof(_frameOffset);
}