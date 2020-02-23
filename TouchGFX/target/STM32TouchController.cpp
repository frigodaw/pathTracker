#include <STM32TouchController.hpp>

/* USER CODE BEGIN BSP user includes */
#include <stm32f429i_discovery_ts.h>
/* USER CODE END BSP user includes */

/*extern "C"
{
    uint32_t LCD_GetXSize();
    uint32_t LCD_GetYSize();
}*/

#define TOUCHCONTROLLER_LCDSIZE_X        (240u)
#define TOUCHCONTROLLER_LCDSIZE_Y        (320u)

using namespace touchgfx;

void STM32TouchController::init()
{
    /* USER CODE BEGIN F4TouchController_init */
    /* Add code for touch controller Initialization*/
    BSP_TS_Init(TOUCHCONTROLLER_LCDSIZE_X, TOUCHCONTROLLER_LCDSIZE_Y);
    /* USER CODE END F4TouchController_init */
}

bool STM32TouchController::sampleTouch(int32_t& x, int32_t& y)
{
    /* USER CODE BEGIN F4TouchController_sampleTouch */
    TS_StateTypeDef state;
    BSP_TS_GetState(&state);
    if (state.TouchDetected)
    {
        x = state.X; // Note this change
        y = state.Y; // Note this change
        return true;
    }
    return false;
    /* USER CODE END F4TouchController_sampleTouch */
}
