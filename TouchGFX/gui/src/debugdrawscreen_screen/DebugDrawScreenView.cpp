#include <gui/debugdrawscreen_screen/DebugDrawScreenView.hpp>

DebugDrawScreenView::DebugDrawScreenView()
{

}

void DebugDrawScreenView::setupScreen()
{
    DebugDrawScreenViewBase::setupScreen();
    ResizeLine();
}

void DebugDrawScreenView::tearDownScreen()
{
    DebugDrawScreenViewBase::tearDownScreen();
}

void DebugDrawScreenView::ResizeLine()
{
    static uint16_t sizeX = 0u;
    static uint16_t sizeY = 0u;
    
    static uint8_t add = 5u;

    if((sizeX > 180u) || (sizeX > 180u))
    {
        sizeX = 0u;
        sizeY = 0u;
    }
    else
    {
        sizeX += add;
        sizeY += add;
    }

    TestLine.setEnd(sizeX, sizeY);
    TestLine.invalidate();

}