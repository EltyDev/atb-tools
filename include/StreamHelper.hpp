#include <bit>
#include <cstring>
#include <fstream>
#include <type_traits>
#include "Serializable.hpp"

class StreamHelper {
public:

    template<typename T>
    static T toBigEndian(T value) {
        static_assert(std::is_trivially_copyable_v<T>, "Type must be trivially copyable");

        if constexpr (sizeof(T) == 1) {
            return value;
        } else {
            if constexpr (std::endian::native == std::endian::little) {
                T swapped;
                std::byte buf[sizeof(T)];

                std::memcpy(buf, &value, sizeof(T));
                std::reverse(buf, buf + sizeof(T));
                std::memcpy(&swapped, buf, sizeof(T));
                return swapped;
            } else {
                return value;
            }
        }
    }

    template<typename T>
    static T fromBigEndian(T value) {
        return toBigEndian(value);
    }

    template <typename T>
    static void write(std::ofstream &stream, const T &data)
    {
        T be = toBigEndian(data);
        stream.write(reinterpret_cast<const char *>(&be), sizeof(T));
    }

    template <typename T>
    static void write(std::ofstream &stream, const T* buffer, size_t count)
    {
        for (size_t i = 0; i < count; i++) {
            T be = toBigEndian(buffer[i]);
            stream.write(reinterpret_cast<const char *>(&be), sizeof(T));
        }
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
        T raw{};
        stream.read(reinterpret_cast<char *>(&raw), sizeof(T));
        return fromBigEndian(raw);
    }

    template <typename T>
    static void read(std::ifstream &stream, T* buffer, size_t count)
    {
        for (size_t i = 0; i < count; i++) {
            T raw{};
            stream.read(reinterpret_cast<char *>(&raw), sizeof(T));
            buffer[i] = fromBigEndian(raw);
        }
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
            buffer[i] = read<T>(stream);
    }
};
