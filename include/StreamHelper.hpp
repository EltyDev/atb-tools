#ifndef STREAMHELPER_HPP_
    #define STREAMHELPER_HPP_

    #include <bit>
    #include <cstring>
    #include <fstream>
    #include <type_traits>
    #include <algorithm>
    #include <vector>
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
    static void write(std::ostream &stream, const T &data)
    {
        T be = toBigEndian(data);
        stream.write(reinterpret_cast<const char *>(&be), sizeof(T));
    }

    template <typename T>
    static void write(std::ostream &stream, const T* buffer, size_t count)
    {
        for (size_t i = 0; i < count; i++) {
            T be = toBigEndian(buffer[i]);
            stream.write(reinterpret_cast<const char *>(&be), sizeof(T));
        }
    }

    template <typename T> requires std::is_base_of_v<Serializable, T>
    static void write(std::ostream &stream, const T &data)
    {
        data.serialize(stream);
    }

    template <typename T> requires std::is_base_of_v<Serializable, T>
    static void write(std::ostream &stream, const T* buffer, size_t count)
    {
        for (size_t i = 0; i < count; ++i)
            buffer[i].serialize(stream);
    }

    template <typename T>
    static T read(std::istream &stream)
    {
        T raw{};
        stream.read(reinterpret_cast<char *>(&raw), sizeof(T));
        return fromBigEndian(raw);
    }

    template <typename T>
    static void read(std::istream &stream, T* buffer, size_t count)
    {
        for (size_t i = 0; i < count; i++) {
            T raw{};
            stream.read(reinterpret_cast<char *>(&raw), sizeof(T));
            buffer[i] = fromBigEndian(raw);
        }
    }

    template <typename T> requires std::is_base_of_v<Serializable, T>
    static T read(std::istream &stream)
    {
        T data;
        data.deserialize(stream);
        return data;
    }

    template <typename T> requires std::is_base_of_v<Serializable, T>
    static void read(std::istream &stream, T* buffer, size_t count)
    {
        for (size_t i = 0; i < count; ++i)
            buffer[i] = read<T>(stream);
    }

    static std::streamoff getFreeZoneSize(std::fstream &stream, std::vector<uint32_t> offsets)
    {
        std::streamoff offset = 0;
        std::vector<uint32_t> usefulOffsets;
        std::streampos currentPos = stream.tellg();
        for (uint32_t offset : offsets) {
            if (offset >= currentPos)
                usefulOffsets.push_back(offset);
        }
        for (; StreamHelper::read<uint8_t>(stream) == 0x00 && !stream.eof() ; offset++) {
            currentPos = stream.tellg();
            bool isBreak = false;
            for (uint32_t off : usefulOffsets) {
                if (currentPos >= off) {
                    isBreak = true;
                    break;
                }
            }
            if (isBreak)
                break;
        }
        return offset;
    }
};

#endif /* !STREAMHELPER_HPP_ */