/*********************************************************************************/
/********** THIS FILE IS GENERATED BY TOUCHGFX DESIGNER, DO NOT MODIFY ***********/
/*********************************************************************************/
#ifndef FRONTENDAPPLICATIONBASE_HPP
#define FRONTENDAPPLICATIONBASE_HPP

#include <mvp/MVPApplication.hpp>
#include <gui/model/Model.hpp>

class FrontendHeap;

class FrontendApplicationBase : public touchgfx::MVPApplication
{
public:
    FrontendApplicationBase(Model& m, FrontendHeap& heap);
    virtual ~FrontendApplicationBase() { }

    // StartScreen
    void gotoStartScreenScreenNoTransition();

    // AppMenuScreen
    void gotoAppMenuScreenScreenNoTransition();

    // AppActivityScreen
    void gotoAppActivityScreenScreenNoTransition();

    // DebugGpsDataScreen
    void gotoDebugGpsDataScreenScreenNoTransition();

    // DebugSdCardScreen
    void gotoDebugSdCardScreenScreenNoTransition();

    // DebugEnvSensorsScreen
    void gotoDebugEnvSensorsScreenScreenNoTransition();

    // AppSettingsScreen
    void gotoAppSettingsScreenScreenNoTransition();

protected:
    touchgfx::Callback<FrontendApplicationBase> transitionCallback;
    FrontendHeap& frontendHeap;
    Model& model;

    // StartScreen
    void gotoStartScreenScreenNoTransitionImpl();

    // AppMenuScreen
    void gotoAppMenuScreenScreenNoTransitionImpl();

    // AppActivityScreen
    void gotoAppActivityScreenScreenNoTransitionImpl();

    // DebugGpsDataScreen
    void gotoDebugGpsDataScreenScreenNoTransitionImpl();

    // DebugSdCardScreen
    void gotoDebugSdCardScreenScreenNoTransitionImpl();

    // DebugEnvSensorsScreen
    void gotoDebugEnvSensorsScreenScreenNoTransitionImpl();

    // AppSettingsScreen
    void gotoAppSettingsScreenScreenNoTransitionImpl();
};

#endif // FRONTENDAPPLICATIONBASE_HPP
