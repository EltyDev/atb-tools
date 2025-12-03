#include "ATB/Layer.hpp"
#include "StreamHelper.hpp"

Layer::Layer(uint8_t alpha, uint8_t flip, uint16_t texture_index, uint16_t texcoord_upper_left_x, uint16_t texcoord_upper_left_y, uint16_t texcoord_width, uint16_t texcoord_height, uint16_t shift_x, uint16_t shift_y, uint16_t upper_left_vertex_x, uint16_t upper_left_vertex_y, uint16_t upper_right_vertex_x, uint16_t upper_right_vertex_y, uint16_t lower_right_vertex_x, uint16_t lower_right_vertex_y, uint16_t lower_left_vertex_x, uint16_t lower_left_vertex_y)
    : _alpha(alpha), _flip(flip), _texture_index(texture_index), _texcoord_upper_left_x(texcoord_upper_left_x), _texcoord_upper_left_y(texcoord_upper_left_y), _texcoord_width(texcoord_width), _texcoord_height(texcoord_height), _shift_x(shift_x), _shift_y(shift_y), _upper_left_vertex_x(upper_left_vertex_x), _upper_left_vertex_y(upper_left_vertex_y), _upper_right_vertex_x(upper_right_vertex_x), _upper_right_vertex_y(upper_right_vertex_y), _lower_right_vertex_x(lower_right_vertex_x), _lower_right_vertex_y(lower_right_vertex_y), _lower_left_vertex_x(lower_left_vertex_x), _lower_left_vertex_y(lower_left_vertex_y)
{
}

void Layer::serialize(std::ostream &stream) const
{
    StreamHelper::write(stream, _alpha);
    StreamHelper::write(stream, _flip);
    StreamHelper::write(stream, _texture_index);
    StreamHelper::write(stream, _texcoord_upper_left_x);
    StreamHelper::write(stream, _texcoord_upper_left_y);
    StreamHelper::write(stream, _texcoord_width);
    StreamHelper::write(stream, _texcoord_height);
    StreamHelper::write(stream, _shift_x);
    StreamHelper::write(stream, _shift_y);
    StreamHelper::write(stream, _upper_left_vertex_x);
    StreamHelper::write(stream, _upper_left_vertex_y);
    StreamHelper::write(stream, _upper_right_vertex_x);
    StreamHelper::write(stream, _upper_right_vertex_y);
    StreamHelper::write(stream, _lower_right_vertex_x);
    StreamHelper::write(stream, _lower_right_vertex_y);
    StreamHelper::write(stream, _lower_left_vertex_x);
    StreamHelper::write(stream, _lower_left_vertex_y);
}

void Layer::deserialize(std::istream &stream)
{
    _alpha = StreamHelper::read<uint8_t>(stream);
    _flip = StreamHelper::read<uint8_t>(stream);
    _texture_index = StreamHelper::read<uint16_t>(stream);
    _texcoord_upper_left_x = StreamHelper::read<uint16_t>(stream);
    _texcoord_upper_left_y = StreamHelper::read<uint16_t>(stream);
    _texcoord_width = StreamHelper::read<uint16_t>(stream);
    _texcoord_height = StreamHelper::read<uint16_t>(stream);
    _shift_x = StreamHelper::read<uint16_t>(stream);
    _shift_y = StreamHelper::read<uint16_t>(stream);
    _upper_left_vertex_x = StreamHelper::read<uint16_t>(stream);
    _upper_left_vertex_y = StreamHelper::read<uint16_t>(stream);
    _upper_right_vertex_x = StreamHelper::read<uint16_t>(stream);
    _upper_right_vertex_y = StreamHelper::read<uint16_t>(stream);
    _lower_right_vertex_x = StreamHelper::read<uint16_t>(stream);
    _lower_right_vertex_y = StreamHelper::read<uint16_t>(stream);
    _lower_left_vertex_x = StreamHelper::read<uint16_t>(stream);
    _lower_left_vertex_y = StreamHelper::read<uint16_t>(stream);
}

uint8_t Layer::getAlpha() const
{
    return this->_alpha;
}

uint8_t Layer::getFlip() const
{
    return this->_flip;
}

uint16_t Layer::getTextureIndex() const
{
    return this->_texture_index;
}

uint16_t Layer::getTexcoordUpperLeftX() const
{
    return this->_texcoord_upper_left_x;
}

uint16_t Layer::getTexcoordUpperLeftY() const
{
    return this->_texcoord_upper_left_y;
}

uint16_t Layer::getTexcoordWidth() const
{
    return this->_texcoord_width;
}

uint16_t Layer::getTexcoordHeight() const
{
    return this->_texcoord_height;
}

uint16_t Layer::getShiftX() const
{
    return this->_shift_x;
}

uint16_t Layer::getShiftY() const
{
    return this->_shift_y;
}

uint16_t Layer::getUpperLeftVertexX() const
{
    return this->_upper_left_vertex_x;
}

uint16_t Layer::getUpperLeftVertexY() const
{
    return this->_upper_left_vertex_y;
}

uint16_t Layer::getUpperRightVertexX() const
{
    return this->_upper_right_vertex_x;
}

uint16_t Layer::getUpperRightVertexY() const
{
    return this->_upper_right_vertex_y;
}

uint16_t Layer::getLowerRightVertexX() const
{
    return this->_lower_right_vertex_x;
}

uint16_t Layer::getLowerRightVertexY() const
{
    return this->_lower_right_vertex_y;
}

uint16_t Layer::getLowerLeftVertexX() const
{
    return this->_lower_left_vertex_x;
}

uint16_t Layer::getLowerLeftVertexY() const
{
    return this->_lower_left_vertex_y;
}

size_t Layer::getSize() const
{
    return sizeof(_alpha) + sizeof(_flip) + sizeof(_texture_index) + sizeof(_texcoord_upper_left_x) + sizeof(_texcoord_upper_left_y) +
        sizeof(_texcoord_width) + sizeof(_texcoord_height) + sizeof(_shift_x) + sizeof(_shift_y) +
        sizeof(_upper_left_vertex_x) + sizeof(_upper_left_vertex_y) + sizeof(_upper_right_vertex_x) + sizeof(_upper_right_vertex_y) +
        sizeof(_lower_right_vertex_x) + sizeof(_lower_right_vertex_y) + sizeof(_lower_left_vertex_x) + sizeof(_lower_left_vertex_y);
}