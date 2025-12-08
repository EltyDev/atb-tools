#include "ATB/Format.hpp"
#include "StreamHelper.hpp"

Format::Format(const ATBValue formatValue, bool isAtb) : _value(formatValue)
{
    if (isAtb)
        return;
    switch (static_cast<GXValue>(_value)) {
        case GXValue::RGBA8:
            _value = ATBValue::RGBA8;
            break;
        case GXValue::RGB5A3:
            _value = ATBValue::RGB5A3;
            break;
        case GXValue::CI8:
            _value = ATBValue::CI8;
            break;
        case GXValue::CI4:
            _value = ATBValue::CI4;
            break;
        case GXValue::IA8:
            _value = ATBValue::IA8;
            break;
        case GXValue::IA4:
            _value = ATBValue::IA4;
            break;
        case GXValue::I8:
            _value = ATBValue::I8;
            break;
        case GXValue::I4:
            _value = ATBValue::I4;
            break;
        case GXValue::A8:
            _value = ATBValue::A8;
            break;
        case GXValue::CMPR:
            _value = ATBValue::CMPR;
            break;
        default:
            throw std::runtime_error("Invalid ATB format value");
    }
}

Format::Format() : _value(ATBValue::RGBA8)
{
}

void Format::serialize(std::ostream &stream) const
{
    StreamHelper::write(stream, _value);
}

void Format::deserialize(std::istream &stream)
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

uint8_t Format::getBppFromGX() const {
	switch (_value) {
        case ATBValue::RGBA8:
            return 32;
        case ATBValue::RGB5A3:
            return 16;
        case ATBValue::CI8:
            return 8;
        case ATBValue::CI4:
            return 4;
        case ATBValue::IA8:
            return 16;
        case ATBValue::IA4:
            return 8;
        case ATBValue::I8:
            return 8;
        case ATBValue::I4:
            return 4;
        case ATBValue::A8:
            return 8;
        case ATBValue::CMPR:
            return 4;
        default:
            return 4;
    }
}