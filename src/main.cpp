#include <iostream>
#include <filesystem>
#include <cstring>
#include "ATB/ATBFile.hpp"

int packATB(const std::filesystem::path filePath) {
    ATBFile atbFile(filePath);
    atbFile.pack();
    return (0);
}

int unpackATB(const std::filesystem::path filePath) {
    ATBFile atbFile(filePath);
    atbFile.unpack();
    return (0);
}

int test(const std::filesystem::path filePath) {
    ATBFile atbFile(filePath);
    atbFile.exportToFile(filePath.stem().string() + "_test.atb");
    return (0);
}

int main(const int argc, char const * const *argv) {
    const std::filesystem::path path(argv[0]);
    const std::string usage = "Usage: " + path.filename().string() + " <pack/unpack> <file>";
    if (argc != 3) {
        error_usage: 
            std::cout << usage << std::endl;
            return (1);
    }
    try {
        if (!strcmp(argv[1], "pack"))
            return packATB(std::filesystem::path(argv[2]));
        else if (!strcmp(argv[1], "unpack"))
            return unpackATB(std::filesystem::path(argv[2]));
        else if (!strcmp(argv[1], "test"))
            test(std::filesystem::path(argv[2]));
        else
            goto error_usage;
    } catch (const std::exception &e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return (1);
    }
    return (0);
}