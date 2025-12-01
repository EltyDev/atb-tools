#ifndef BANK_HPP_
    #define BANK_HPP_

    #include "Serializable.hpp"

class Bank : public Serializable {
    public:
        Bank() = default;
        Bank(uint16_t frames, uint16_t padding, uint32_t frameOffset);
        Bank(std::ifstream &stream);
        ~Bank() = default;
        void serialize(std::ofstream &stream) const override;
        void deserialize(std::ifstream &stream) override;
        uint16_t getFrames() const;
        uint16_t getPadding() const;
        uint32_t getFrameOffset() const;
    protected:
    private:
        uint16_t _frames;
        uint16_t _padding;
        uint32_t _frameOffset;
};

#endif /* !BANK_HPP_ */
