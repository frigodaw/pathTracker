#include <touchgfx/widgets/Map.hpp>
#include <touchgfx/hal/HAL.hpp>

namespace touchgfx
{

Map::Map() :
      BoxWithBorder(),
      size(2),
      xOffset(5),
      yOffset(35)
{
}


void Map::drawPixel(float x, float y)
{
    uint16_t* framebuffer = touchgfx::HAL::getInstance()->lockFrameBuffer();

    for(uint8_t i=0u; i<size; i++)
    {
        for(uint8_t j=0u; j<size; j++)
        {
            uint32_t pixelAddress = (xOffset + x + i) + (yOffset + y + j)*touchgfx::HAL::DISPLAY_WIDTH;
            framebuffer[pixelAddress] = 0u;
        }
    }
    touchgfx::HAL::getInstance()->unlockFrameBuffer();
}

} // namespace touchgfx
