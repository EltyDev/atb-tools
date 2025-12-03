#ifndef TPLFILE_HPP_
    #define TPLFILE_HPP_

    #include "Serializable.hpp"
    #include "TPL/ImageHeader.hpp"
    #include "TPL/ImageTable.hpp"
    #include "TPL/PaletteHeader.hpp"
    #include "TPL/Header.hpp"
    #include <vector>
    #include <filesystem>

class ATBFile;

class TPLFile : public Serializable {
    public:
        TPLFile(ATBFile &atbFile);
        TPLFile(const std::filesystem::path path);
        ~TPLFile() = default;
        void serialize(std::ostream &stream) const override;
        void deserialize(std::istream &stream) override;
        const Header &getHeader() const;
        const std::vector<ImageTable> &getImageTables() const;
        const std::vector<PaletteHeader> &getPalettes() const;
        const std::vector<ImageHeader> &getImages() const;
        size_t getSize() const override;
    protected:
    private:
        Header _header;
        std::vector<ImageTable> _imageTables;
        std::vector<PaletteHeader> _palettes;
        std::vector<ImageHeader> _images;
        
};

#endif /* !TPLFILE_HPP_ */
