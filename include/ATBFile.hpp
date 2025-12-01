#ifndef ATBFILE_HPP_
    #define ATBFILE_HPP_

    #include <filesystem>
    #include <vector>

    #include "PatternEntry.hpp"
    #include "Bank.hpp"
    #include "TextureEntry.hpp"
    #include "Frame.hpp"
    #include "Layer.hpp"

class ATBFile : public Serializable {
    public:
        ATBFile(const std::filesystem::path path);
        ~ATBFile() = default;
        void serialize(std::ofstream &stream) const override;
        void deserialize(std::ifstream &stream) override;
    protected:
    private:
        std::vector<PatternEntry> _patterns;
        std::vector<Bank> _banks;
        std::vector<TextureEntry> _textures;
        std::vector<Frame> _frames;
        std::vector<Layer> _layers;
};

#endif /* !ATBFILE_HPP_ */
