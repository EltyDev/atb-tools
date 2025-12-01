#include <iostream>
#include <filesystem>


int main(const int argc, char const * const *argv) {
    const std::filesystem::path path(argv[0]);
    const std::string usage = "Usage: " + path.filename().string() + " <pack/unpack> <folder/file>";
    if (argc < 3)
        goto error;
    if (!strcmp(argv[1], "pack")) {

    } else if (!strcmp(argv[1], "unpack")) {

    } else {
        error: 
            std::cout << usage << std::endl;
            return (1);
    }
    return (0);
}