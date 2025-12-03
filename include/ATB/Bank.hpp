#ifndef BANK_HPP_
    #define BANK_HPP_

    #include "Serializable.hpp"
    #include <cstdint>

class Bank : public Serializable {
    public:
        Bank() = default;
        Bank(uint16_t frames, uint32_t frameOffset);
        Bank(std::istream &stream);
        ~Bank() = default;
        void serialize(std::ostream &stream) const override;
        void deserialize(std::istream &stream) override;
        size_t getSize() const override;
        uint16_t getFrames() const;
        uint32_t getFrameOffset() const;
    protected:
    private:
        uint16_t _frames;
        uint16_t _padding;
        uint32_t _frameOffset;
};

#endif /* !BANK_HPP_ */
