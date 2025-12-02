#include "ATB/Format.hpp"
#include "StreamHelper.hpp"

Format::Format(const ATBValue formatValue) : _value(formatValue)
{
}

Format::Format() : _value(ATBValue::RGBA8)
{
}

void Format::serialize(std::ofstream &stream) const
{
    StreamHelper::write(stream, _value);
}

void Format::deserialize(std::ifstream &stream)
{
    _value = StreamHelper::read<Format::ATBValue>(stream);
}

Format::ATBValue Format::getValue() const
{
    return _value;
}

size_t Format::getSize() const
{
    return sizeof(uint8_t);
}

Format::GXValue Format::getGXValue() const
{
    switch (_value) {
        case ATBValue::RGBA8:
            return GXValue::RGBA8;
        case ATBValue::RGB5A3:
        case ATBValue::RGB5A3_DUPE:
            return GXValue::RGB5A3;
        case ATBValue::CI8:
            return GXValue::CI8;
        case ATBValue::CI4:
            return GXValue::CI4;
        case ATBValue::IA8:
            return GXValue::IA8;
        case ATBValue::IA4:
            return GXValue::IA4;
        case ATBValue::I8:
            return GXValue::I8;
        case ATBValue::I4:
            return GXValue::I4;
        case ATBValue::A8:
            return GXValue::A8;
        case ATBValue::CMPR:
            return GXValue::CMPR;
        default:
            throw std::runtime_error("Invalid ATB format value");
    }
}