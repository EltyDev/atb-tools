#ifndef PATTERNENTRY_HPP_
    #define PATTERNENTRY_HPP_

    #include "Serializable.hpp"

class PatternEntry : public Serializable {
    public:
        PatternEntry() = default;
        PatternEntry(uint16_t layers, uint16_t centerX, uint16_t centerY, uint16_t width, uint16_t height, uint32_t layerOffset);
        PatternEntry(std::ifstream &file);
        ~PatternEntry() = default;
        void serialize(std::ofstream &stream) const override;
        void deserialize(std::ifstream &stream) override;
        uint16_t getLayers() const;
        uint16_t getCenterX() const;
        uint16_t getCenterY() const;
        uint16_t getWidth() const;
        uint16_t getHeight() const;
        uint32_t getLayerOffset() const;
    protected:
    private:
        uint16_t _layers;
        uint16_t _centerX;
        uint16_t _centerY;
        uint16_t _width;
        uint16_t _height;
        uint32_t _layerOffset;
};

#endif /* !PATTERNENTRY_HPP_ */
