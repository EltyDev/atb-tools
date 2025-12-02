#ifndef HEADER_HPP_
    #define HEADER_HPP_

    #include "Serializable.hpp"
    #include <cstdint>

#define MAGIC_NUMBER 0x20AF30

class Header : public Serializable {
    public:
        Header() = default;
        Header(uint32_t numTextures);
        Header(std::ifstream &stream);
        ~Header() = default;
        void serialize(std::ofstream &stream) const override;
        void deserialize(std::ifstream &stream) override;
        uint32_t getNumTextures() const;
        uint32_t getOffsetTableOffset() const;
        size_t getSize() const override;
    protected:
    private:
        uint32_t _magic;
        uint32_t _numTextures;
        uint32_t _offsetTableOffset;
};

#endif /* !HEADER_HPP_ */
