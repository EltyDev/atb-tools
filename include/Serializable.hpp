#ifndef SERIALIZABLE_HPP_
    #define SERIALIZABLE_HPP_

#include <fstream>

class Serializable {
    public:
        virtual void serialize(std::ofstream &stream) const = 0;
        virtual void deserialize(std::ifstream &stream) = 0;
    protected:
    private:
};

#endif /* !SERIALIZABLE_HPP_ */
