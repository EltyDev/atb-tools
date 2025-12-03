#ifndef PALETTEHEADER_HPP_
    #define PALETTEHEADER_HPP_

    #include "Serializable.hpp"
    #include "ATB/Format.hpp"
    #include <vector>

class PaletteHeader : public Serializable {
    public:
        PaletteHeader() = default;
        PaletteHeader(uint16_t numEntries, uint8_t unpacked, uint32_t dataOffset, const Format &format, const std::vector<uint8_t> &paletteData);
        PaletteHeader(std::istream &stream);
        ~PaletteHeader() = default;
        void serialize(std::ostream &stream) const override;
        void deserialize(std::istream &stream) override;
        size_t getSize() const override;
        uint16_t getNumEntries() const;
        uint8_t getUnpacked() const;
        uint32_t getDataOffset() const;
        Format getFormat() const;
        const std::vector<uint8_t> &getPaletteData() const;
    protected:
    private:
        uint16_t _numEntries;
        uint8_t _unpacked;
        uint8_t _padding;
        Format _format;
        uint32_t _dataOffset;
        std::vector<uint8_t> _paletteData;
};

#endif /* !PALETTEHEADER_HPP_ */
