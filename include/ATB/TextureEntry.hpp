#ifndef TEXTUREENTRY_HPP_
    #define TEXTUREENTRY_HPP_

    #include "Format.hpp"
    #include <vector>


class TextureEntry : public Serializable {
    public:
        TextureEntry() = default;
        TextureEntry(std::istream &file);
        TextureEntry(uint8_t bpp, const Format &format, uint16_t paletteSize, uint16_t width, uint16_t height, uint32_t imageSize, uint32_t paletteOffset, uint32_t imageOffset);
        void serialize(std::ostream &stream) const override;
        void deserialize(std::istream &stream) override;
        size_t getSize() const override;
        uint8_t getBpp() const;
        Format getFormat() const;
        uint16_t getPaletteSize() const;
        uint16_t getWidth() const;
        uint16_t getHeight() const;
        uint32_t getImageSize() const;
        uint32_t getPaletteOffset() const;
        uint32_t getImageOffset() const;
        std::vector<uint8_t> getPaletteData() const;
        std::vector<uint8_t> getImageData() const;
        void writePadding(std::fstream &stream) const;
        ~TextureEntry() = default;

    protected:
    private:
        uint8_t _bpp;
        Format _format;
        uint16_t _paletteSize;
        uint16_t _width;
        uint16_t _height;
        uint32_t _imageSize;
        uint32_t _paletteOffset;
        std::vector<uint8_t> _paletteData;
        uint32_t _imageOffset;
        std::vector<uint8_t> _imageData;
};

#endif /* !TEXTUREENTRY_HPP_ */
