#include "TPL/PaletteHeader.hpp"
#include "StreamHelper.hpp"

PaletteHeader::PaletteHeader(uint16_t numEntries, uint8_t unpacked, uint32_t dataOffset, const Format &format, const std::vector<uint8_t> &paletteData)
    : _numEntries(numEntries), _unpacked(unpacked), _padding(0), _format(format), _dataOffset(dataOffset), _paletteData(paletteData)
{
}

PaletteHeader::PaletteHeader(std::ifstream &stream)
{
    this->deserialize(stream);
}

void PaletteHeader::serialize(std::ofstream &stream) const
{
    StreamHelper::write(stream, _numEntries);
    StreamHelper::write(stream, _unpacked);
    StreamHelper::write(stream, _padding);
    StreamHelper::write(stream, static_cast<uint32_t>(_format.getValue()));
    StreamHelper::write(stream, _dataOffset);
    std::streampos lastPos = stream.tellp();
    stream.seekp(_dataOffset, std::ios::beg);
    StreamHelper::write(stream, _paletteData.data(), _numEntries * 2);
    stream.seekp(lastPos);
}

void PaletteHeader::deserialize(std::ifstream &stream)
{
    if (stream.tellg() == 0)
        return;
    _numEntries = StreamHelper::read<uint16_t>(stream);
    _unpacked = StreamHelper::read<uint8_t>(stream);
    _padding = StreamHelper::read<uint8_t>(stream);
    _format = Format(static_cast<Format::ATBValue>(StreamHelper::read<uint32_t>(stream)));
    _dataOffset = StreamHelper::read<uint32_t>(stream);
    std::streampos lastPos = stream.tellg();
    stream.seekg(_dataOffset, std::ios::beg);
    _paletteData.resize(_numEntries * 2);
    StreamHelper::read(stream, _paletteData.data(), _numEntries * 2);
    stream.seekg(lastPos);
}

uint16_t PaletteHeader::getNumEntries() const
{
    return this->_numEntries;
}

uint8_t PaletteHeader::getUnpacked() const
{
    return this->_unpacked;
}

uint32_t PaletteHeader::getDataOffset() const
{
    return this->_dataOffset;
}

Format PaletteHeader::getFormat() const
{
    return this->_format;
}

const std::vector<uint8_t> &PaletteHeader::getPaletteData() const
{
    return this->_paletteData;
}

size_t PaletteHeader::getSize() const
{
    return sizeof(_numEntries) + sizeof(_unpacked) + sizeof(_padding) + sizeof(uint32_t) /* format */ + sizeof(_dataOffset);
}
