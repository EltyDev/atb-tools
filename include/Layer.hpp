#ifndef LAYER_HPP_
    #define LAYER_HPP_

    #include "Serializable.hpp"

class Layer : public Serializable {
    public:
        Layer() = default;
        Layer(uint8_t alpha, uint8_t flip, uint16_t texture_index, uint16_t texcoord_upper_left_x, uint16_t texcoord_upper_left_y, uint16_t texcoord_width, uint16_t texcoord_height, uint16_t shift_x, uint16_t shift_y, uint16_t upper_left_vertex_x, uint16_t upper_left_vertex_y, uint16_t upper_right_vertex_x, uint16_t upper_right_vertex_y, uint16_t lower_right_vertex_x, uint16_t lower_right_vertex_y, uint16_t lower_left_vertex_x, uint16_t lower_left_vertex_y);
        void serialize(std::ofstream &stream) const override;
        void deserialize(std::ifstream &stream) override;
        uint8_t getAlpha() const;
        uint8_t getFlip() const;
        uint16_t getTextureIndex() const;
        uint16_t getTexcoordUpperLeftX() const;
        uint16_t getTexcoordUpperLeftY() const;
        uint16_t getTexcoordWidth() const;
        uint16_t getTexcoordHeight() const;
        uint16_t getShiftX() const;
        uint16_t getShiftY() const;
        uint16_t getUpperLeftVertexX() const;
        uint16_t getUpperLeftVertexY() const;
        uint16_t getUpperRightVertexX() const;
        uint16_t getUpperRightVertexY() const;
        uint16_t getLowerRightVertexX() const;
        uint16_t getLowerRightVertexY() const;
        uint16_t getLowerLeftVertexX() const;
        uint16_t getLowerLeftVertexY() const;
        ~Layer() = default;

    protected:
    private:
        uint8_t _alpha;
        uint8_t _flip;
        uint16_t _texture_index;
        uint16_t _texcoord_upper_left_x;
        uint16_t _texcoord_upper_left_y;
        uint16_t _texcoord_width;
        uint16_t _texcoord_height;
        uint16_t _shift_x;
        uint16_t _shift_y;
        uint16_t _upper_left_vertex_x;
        uint16_t _upper_left_vertex_y;
        uint16_t _upper_right_vertex_x;
        uint16_t _upper_right_vertex_y;
        uint16_t _lower_right_vertex_x;
        uint16_t _lower_right_vertex_y;
        uint16_t _lower_left_vertex_x;
        uint16_t _lower_left_vertex_y;
};

#endif /* !LAYER_HPP_ */
