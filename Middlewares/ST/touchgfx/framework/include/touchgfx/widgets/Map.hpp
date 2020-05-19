#ifndef MAP_HPP
# define MAP_HPP

#include <touchgfx/widgets/BoxWithBorder.hpp>
#include <touchgfx/Color.hpp>

namespace touchgfx
{

class Map : public BoxWithBorder
{
public:
    Map();

    void drawPixel(float x, float y);

private:
    uint8_t size;
    uint8_t xOffset;
    uint8_t yOffset;

};
} // namespace touchgfx

#endif // MAP_HPP
