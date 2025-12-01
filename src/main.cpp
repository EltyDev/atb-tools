#include <iostream>
#include <filesystem>
#include "ATBFile.hpp"


int main(const int argc, char const * const *argv) {
    const std::filesystem::path path(argv[0]);
    const std::string usage = "Usage: " + path.filename().string() + " <pack/unpack> <folder/file>";
    
    if (argc < 3)
        goto error;
    if (!strcmp(argv[1], "pack")) {
        ATBFile atbFile((std::filesystem::path(argv[2])));
    } else if (!strcmp(argv[1], "unpack")) {
        ATBFile atbFile((std::filesystem::path(argv[2])));
    } else {
        error: 
            std::cout << usage << std::endl;
            return (1);
    }
    return (0);
}