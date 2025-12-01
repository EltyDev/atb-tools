#ifndef ATBFILE_HPP_
    #define ATBFILE_HPP_

    #include <filesystem>
    #include <vector>

    #include "PatternEntry.hpp"
    #include "FrameEntry.hpp"

class ATBFile : public Serializable {
    public:
        ATBFile(const std::filesystem::path path);
        ~ATBFile() = default;
        void serialize(std::ofstream &stream) const override;
        void deserialize(std::ifstream &stream) override;
    protected:
    private:
        std::vector<PatternEntry> _patterns;
        std::vector<std::vector<FrameEntry>> _banks;
};

struct Bank {
    uint16_t frames;
    uint16_t padding;
    uint32_t offset;
};

#endif /* !ATBFILE_HPP_ */
