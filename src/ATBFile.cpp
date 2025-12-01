#include "ATBFile.hpp"
#include "StreamHelper.hpp"
#include "TextureEntry.hpp"

ATBFile::ATBFile(const std::filesystem::path path)
{
    std::ifstream file(path, std::ios::binary);
    if (!file.is_open())
        throw std::runtime_error("Could not open file: " + path.string());
    this->deserialize(file);
    file.close();
}

void ATBFile::deserialize(std::ifstream &stream)
{
    if (!stream.is_open())
        throw std::runtime_error("Stream is not open");
    if (stream.left < 16)
        throw std::runtime_error("Stream is too small to be a valid ATB file");

    uint16_t numBanks = StreamHelper::read<uint16_t>(stream);
    uint16_t numPatterns = StreamHelper::read<uint16_t>(stream);
    uint16_t numTextures = StreamHelper::read<uint16_t>(stream);
    uint16_t numReferences = StreamHelper::read<uint16_t>(stream);
    uint32_t bankOffset = StreamHelper::read<uint32_t>(stream);
    uint32_t patternOffset = StreamHelper::read<uint32_t>(stream);

    if (patternOffset != 20)
        throw std::runtime_error("Invalid ATB file: pattern offset is not 20");

    uint32_t textureOffset = StreamHelper::read<uint32_t>(stream);

    stream.seekg(patternOffset, std::ios::beg);
    _patterns.reserve(numPatterns);
    for (uint16_t i = 0; i < numPatterns; ++i)
        _patterns.emplace_back(stream);
    stream.seekg(bankOffset, std::ios::beg);
    _banks.reserve(numBanks);
    std::vector<Bank> banksInfo;
    banksInfo.resize(numBanks);
    StreamHelper::read<Bank>(stream, banksInfo.data(), numBanks);
    for (uint16_t i = 0; i < numBanks; ++i) {
        _banks.emplace_back();
        stream.seekg(banksInfo[i].offset, std::ios::beg);
        _banks[i].reserve(banksInfo[i].frames);
        for (uint16_t j = 0; j < banksInfo[i].frames; ++j)
            _banks[i].emplace_back(stream);
    }
    stream.seekg(textureOffset, std::ios::beg);
    std::vector<TextureEntry> textures;
    textures.resize(numTextures);
    StreamHelper::read<TextureEntry>(stream, textures.data(), numTextures);
    for (uint16_t i = 0; i < numTextures; ++i) {

    }

}

void ATBFile::serialize(std::ofstream &stream) const
{
    // Implementation of serialization logic goes here
}
