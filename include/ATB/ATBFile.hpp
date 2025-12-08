#ifndef ATBFILE_HPP_
    #define ATBFILE_HPP_

    #include <filesystem>
    #include <vector>

    #include "PatternEntry.hpp"
    #include "Bank.hpp"
    #include "TextureEntry.hpp"
    #include "Frame.hpp"
    #include "Layer.hpp"

    #define ATB_TEXTURE_ALIGNMENT 16

class TPLFile;

namespace pugi {
    class xml_document;
}

class ATBFile : public Serializable {
    public:
        ATBFile(const std::filesystem::path path);
        ATBFile(const TPLFile &tplFile);
        ~ATBFile() = default;
        void serialize(std::ostream &stream) const override;
        void deserialize(std::istream &stream) override;
        size_t getSize() const override;
        void exportToFile(const std::filesystem::path path) const;
        void pack();
        void unpack();
        const std::vector<PatternEntry> &getPatterns() const;
        const std::vector<Bank> &getBanks() const;
        const std::vector<TextureEntry> &getTextures() const;
        const std::vector<Frame> &getFrames() const;
        const std::vector<Layer> &getLayers() const;
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
