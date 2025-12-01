#ifndef FRAMEENTRY_HPP_
    #define FRAMEENTRY_HPP_

    #include <cstdint>
    #include <fstream>

    #include "Serializable.hpp"

class FrameEntry : public Serializable {
    public:
        FrameEntry(std::ifstream &file);
        FrameEntry(uint16_t patternIndex, uint16_t frameLength, uint16_t shiftX, uint16_t shiftY, uint16_t flip);
        ~FrameEntry() = default;
        void serialize(std::ofstream &stream) const override;
        void deserialize(std::ifstream &stream) override;
        uint16_t getPatternIndex() const;
        uint16_t getFrameLength() const;
        uint16_t getShiftX() const;
        uint16_t getShiftY() const;
        uint16_t getFlip() const;
    protected:
    private:
        uint16_t _patternIndex;
        uint16_t _frameLength;
        uint16_t _shiftX;
        uint16_t _shiftY;
        uint16_t _flip;
};

#endif /* !FRAMEENTRY_HPP_ */
