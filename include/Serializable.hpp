#ifndef SERIALIZABLE_HPP_
    #define SERIALIZABLE_HPP_

#include <fstream>

class Serializable {
    public:
        virtual void serialize(std::ostream &stream) const = 0;
        virtual void deserialize(std::istream &stream) = 0;
        virtual size_t getSize() const = 0;
    protected:
    private:
};

#endif /* !SERIALIZABLE_HPP_ */
