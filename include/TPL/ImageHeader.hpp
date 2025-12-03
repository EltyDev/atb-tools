#ifndef IMAGEHEADER_HPP_
    #define IMAGEHEADER_HPP_

    #include "Serializable.hpp"
    #include "ATB/Format.hpp"
    #include <vector>

class ImageHeader : public Serializable {
    public:
        ImageHeader() = default;
        ImageHeader(uint16_t height, uint16_t width, const Format &format, uint32_t dataOffset, uint32_t wrapS, uint32_t wrapT, uint32_t minFilter, uint32_t magFilter, float lodBias, uint8_t edgeLod, uint8_t minLod, uint8_t maxLod, uint8_t unpacked, const std::vector<uint8_t> &data);
        ImageHeader(std::istream &stream);
        ~ImageHeader() = default;
        void serialize(std::ostream &stream) const override;
        void deserialize(std::istream &stream) override;
        size_t getSize() const override;
        uint16_t getHeight() const;
        uint16_t getWidth() const;
        Format getFormat() const;
        uint32_t getDataOffset() const;
        uint32_t getWrapS() const;
        uint32_t getWrapT() const;
        uint32_t getMinFilter() const;
        uint32_t getMagFilter() const;
        float getLodBias() const;
        uint8_t getEdgeLod() const;
        uint8_t getMinLod() const;
        uint8_t getMaxLod() const;
        uint8_t getUnpacked() const;
        const std::vector<uint8_t> &getData() const;
    protected:
    private:
        uint16_t _height;
        uint16_t _width;
        Format _format;
        uint32_t _dataOffset;
        uint32_t _wraps;
        uint32_t _wrapt;
        uint32_t _minfilter;
        uint32_t _magfilter;
        float _lodbias;
        uint8_t _edgelod;
        uint8_t _minlod;
        uint8_t _maxlod;
        uint8_t _unpacked;
        std::vector<uint8_t> _data;
};

#endif /* !IMAGEHEADER_HPP_ */
