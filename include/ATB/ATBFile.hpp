#ifndef ATBFILE_HPP_
    #define ATBFILE_HPP_

    #include <filesystem>
    #include <vector>

    #include "PatternEntry.hpp"
    #include "Bank.hpp"
    #include "TextureEntry.hpp"
    #include "Frame.hpp"
    #include "Layer.hpp"

    #define TEXTURE_ALIGNMENT 32

class TPLFile;

namespace pugi {
    class xml_document;
}

class ATBFile : public Serializable {
    public:
        ATBFile(const std::filesystem::path path);
        ATBFile(const TPLFile &tplFile, pugi::xml_document &doc);
        ~ATBFile() = default;
        void serialize(std::ofstream &stream) const override;
        void deserialize(std::ifstream &stream) override;
        size_t getSize() const override;
        void pack();
        void unpack();
        std::vector<PatternEntry> &getPatterns();
        std::vector<Bank> &getBanks();
        std::vector<TextureEntry> &getTextures();
        std::vector<Frame> &getFrames();
        std::vector<Layer> &getLayers();
        uint32_t getBankOffset() const;
        uint32_t getPatternOffset() const;
        uint32_t getTextureOffset() const;
    protected:
    private:
        std::vector<PatternEntry> _patterns;
        std::vector<Bank> _banks;
        std::vector<TextureEntry> _textures;
        std::vector<Frame> _frames;
        std::vector<Layer> _layers;
        std::filesystem::path _path;
        uint32_t _bankOffset;
        uint32_t _patternOffset;
        uint32_t _textureOffset;
};

#endif /* !ATBFILE_HPP_ */
