#ifndef IMAGETABLE_HPP_
    #define IMAGETABLE_HPP_

    #include "Serializable.hpp"
    #include <cstdint>

class ImageTable : public Serializable {
    public:
        ImageTable(uint32_t imageOffset, uint32_t paletteOffset);
        ImageTable() = default;
        ~ImageTable() = default;
        ImageTable(std::ifstream &stream);
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

#endif /* !IMAGETABLE_HPP_ */
