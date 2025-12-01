#include "Format.hpp"
#include "StreamHelper.hpp"

Format::Format(const Value formatValue) : _value(formatValue)
{
}

Format::Format() : _value(Value::RGBA32)
{
}

void Format::serialize(std::ofstream &stream) const
{
    StreamHelper::write(stream, _value);
}

void Format::deserialize(std::ifstream &stream)
{
    _value = StreamHelper::read<Format::Value>(stream);
}

Format::Value Format::getValue() const
{
    return _value;
}