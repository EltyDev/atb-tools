#ifndef IMAGEDESCRIPTION_HPP_
    #define IMAGEDESCRIPTION_HPP_

    #include "Serializable.hpp"

class ImageDescription : public Serializable {
    public:
        ImageDescription(uint32_t imageOffset, uint32_t paletteOffset);
        ImageDescription(std::ifstream &stream);
        ~ImageDescription() = default;
        void serialize(std::ofstream &stream) const override;
        void deserialize(std::ifstream &stream) override;
        size_t getSize() const override;
        uint32_t getImageOffset() const;
        uint32_t getPaletteOffset() const;
    protected:
    private:
        uint32_t _imageOffset;
        uint32_t _paletteOffset;

};

#endif /* !IMAGEDESCRIPTION_HPP_ */
