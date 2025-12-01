#ifndef FORMAT_HPP_
    #define FORMAT_HPP_

    #include <cstdint>
    #include "Serializable.hpp"

class Format : public Serializable {
    public:

        enum class Value : uint8_t {
            RGBA32,
            RGB5A3,
            RGB5A3_DUPE,
            C8,
            C4,
            IA8,
            IA4,
            I8,
            I4,
            A8,
            CMPR
        };

        Format(const Value formatValue);
        Format();
        ~Format() = default;
        void serialize(std::ofstream &stream) const override;
        void deserialize(std::ifstream &stream) override;
        Value getValue() const;
    protected:
    private:
        Value _value;
};

#endif /* !FORMAT_HPP_ */
