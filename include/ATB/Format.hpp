#ifndef FORMAT_HPP_
    #define FORMAT_HPP_

    #include <cstdint>
    #include "Serializable.hpp"

class Format : public Serializable {
    public:

        enum class ATBValue : uint8_t {
            RGBA8,
            RGB5A3,
            RGB5A3_DUPE,
            CI8,
            CI4,
            IA8,
            IA4,
            I8,
            I4,
            A8,
            CMPR
        };

        enum class GXValue : uint8_t {
            I4 = 0,
            I8 = 1,
            IA4 = 2,
            IA8 = 3,
            RGB5A3 = 5,
            RGBA8 = 6,
            CI4 = 8,
            CI8 = 9,
            CMPR = 14,
            A8 = 39
        };

        Format(const ATBValue formatValue);
        Format();
        ~Format() = default;
        void serialize(std::ofstream &stream) const override;
        void deserialize(std::ifstream &stream) override;
        size_t getSize() const override;
        ATBValue getValue() const;
        GXValue getGXValue() const;
    protected:
    private:
        ATBValue _value;
};

#endif /* !FORMAT_HPP_ */
