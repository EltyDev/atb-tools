#ifndef FRAME_HPP_
    #define FRAME_HPP_

    #include "Serializable.hpp"

class Frame : public Serializable {
    public:
        Frame() = default;
        Frame(uint16_t patternIndex, uint16_t length, uint16_t shiftX, uint16_t shiftY, uint16_t flip);
        ~Frame() = default;
        void serialize(std::ofstream &stream) const override;
        void deserialize(std::ifstream &stream) override;
        uint16_t getPatternIndex() const;
        uint16_t getLength() const;
        uint16_t getShiftX() const;
        uint16_t getShiftY() const;
        uint16_t getFlip() const;
    protected:
    private:
        uint16_t _patternIndex;
        uint16_t _length;
        uint16_t _shiftX;
        uint16_t _shiftY;
        uint16_t _flip;
        uint16_t _unk;
    };

#endif /* !FRAME_HPP_ */
