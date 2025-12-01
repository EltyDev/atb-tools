#ifndef STREAMHELPER_HPP_
    #define STREAMHELPER_HPP_

    #include <fstream>
    #include "Serializable.hpp"

class StreamHelper {
    public:
        template <typename T>
        static void write(std::ofstream &stream, const T &data)
        {
            stream.write(reinterpret_cast<const char *>(&data), sizeof(T));
        }

        template <typename T>
        static void write(std::ofstream &stream, const T* buffer, size_t count)
        {
            stream.write(reinterpret_cast<const char *>(buffer), sizeof(T) * count);
        }

        template <typename T> requires std::is_base_of_v<Serializable, T>
        static void write(std::ofstream &stream, const T &data)
        {
            data.serialize(stream);
        }

        template <typename T> requires std::is_base_of_v<Serializable, T>
        static void write(std::ofstream &stream, const T* buffer, size_t count)
        {
            for (size_t i = 0; i < count; ++i)
                buffer[i].serialize(stream);
        }

        template <typename T>
        static T read(std::ifstream &stream)
        {
            T data;
            stream.read(reinterpret_cast<char *>(&data), sizeof(T));
            return data;
        }

        template <typename T>
        static void read(std::ifstream &stream, T* buffer, size_t count)
        {
            stream.read(reinterpret_cast<char *>(buffer), sizeof(T) * count);
        }

        template <typename T> requires std::is_base_of_v<Serializable, T>
        static T read(std::ifstream &stream)
        {
            T data;
            data.deserialize(stream);
            return data;
        }
        template <typename T> requires std::is_base_of_v<Serializable, T>
        static void read(std::ifstream &stream, T* buffer, size_t count)
        {
            for (size_t i = 0; i < count; ++i)
                buffer[i].deserialize(stream);
        }
    protected:
    private:
};

#endif /* !STREAMHELPER_HPP_ */
