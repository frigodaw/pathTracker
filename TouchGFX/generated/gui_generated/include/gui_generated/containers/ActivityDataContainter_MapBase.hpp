/*********************************************************************************/
/********** THIS FILE IS GENERATED BY TOUCHGFX DESIGNER, DO NOT MODIFY ***********/
/*********************************************************************************/
#ifndef ACTIVITYDATACONTAINTER_MAPBASE_HPP
#define ACTIVITYDATACONTAINTER_MAPBASE_HPP

#include <gui/common/FrontendApplication.hpp>
#include <touchgfx/containers/Container.hpp>
#include <touchgfx/widgets/TextureMapper.hpp>
#include <touchgfx/widgets/TextAreaWithWildcard.hpp>

class ActivityDataContainter_MapBase : public touchgfx::Container
{
public:
    ActivityDataContainter_MapBase();
    virtual ~ActivityDataContainter_MapBase() {}
    virtual void initialize();

protected:
    FrontendApplication& application() {
        return *static_cast<FrontendApplication*>(touchgfx::Application::getInstance());
    }

    /*
     * Member Declarations
     */
    touchgfx::TextureMapper ArrowIcon;
    touchgfx::TextAreaWithTwoWildcards ScaleTextBox;

    /*
     * Wildcard Buffers
     */
    static const uint16_t SCALETEXTBOXBUFFER1_SIZE = 8;
    touchgfx::Unicode::UnicodeChar ScaleTextBoxBuffer1[SCALETEXTBOXBUFFER1_SIZE];
    static const uint16_t SCALETEXTBOXBUFFER2_SIZE = 4;
    touchgfx::Unicode::UnicodeChar ScaleTextBoxBuffer2[SCALETEXTBOXBUFFER2_SIZE];

private:

};

#endif // ACTIVITYDATACONTAINTER_MAPBASE_HPP
