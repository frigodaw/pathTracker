/*********************************************************************************/
/********** THIS FILE IS GENERATED BY TOUCHGFX DESIGNER, DO NOT MODIFY ***********/
/*********************************************************************************/
#ifndef ACTIVITYDATACONTAINTERBASE_HPP
#define ACTIVITYDATACONTAINTERBASE_HPP

#include <gui/common/FrontendApplication.hpp>
#include <touchgfx/containers/Container.hpp>
#include <touchgfx/widgets/TextAreaWithWildcard.hpp>

class ActivityDataContainterBase : public touchgfx::Container
{
public:
    ActivityDataContainterBase();
    virtual ~ActivityDataContainterBase() {}
    virtual void initialize();

protected:
    FrontendApplication& application() {
        return *static_cast<FrontendApplication*>(touchgfx::Application::getInstance());
    }

    /*
     * Member Declarations
     */
    touchgfx::TextAreaWithOneWildcard AvgVelocityTextBox;
    touchgfx::TextAreaWithOneWildcard VelocityTextBox;
    touchgfx::TextAreaWithOneWildcard DistanceTextBox;
    touchgfx::TextAreaWithOneWildcard TimerTextBox;

    /*
     * Wildcard Buffers
     */
    static const uint16_t AVGVELOCITYTEXTBOX_SIZE = 8;
    touchgfx::Unicode::UnicodeChar AvgVelocityTextBoxBuffer[AVGVELOCITYTEXTBOX_SIZE];
    static const uint16_t VELOCITYTEXTBOX_SIZE = 8;
    touchgfx::Unicode::UnicodeChar VelocityTextBoxBuffer[VELOCITYTEXTBOX_SIZE];
    static const uint16_t DISTANCETEXTBOX_SIZE = 8;
    touchgfx::Unicode::UnicodeChar DistanceTextBoxBuffer[DISTANCETEXTBOX_SIZE];
    static const uint16_t TIMERTEXTBOX_SIZE = 16;
    touchgfx::Unicode::UnicodeChar TimerTextBoxBuffer[TIMERTEXTBOX_SIZE];

private:

};

#endif // ACTIVITYDATACONTAINTERBASE_HPP
