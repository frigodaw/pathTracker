/*********************************************************************************/
/********** THIS FILE IS GENERATED BY TOUCHGFX DESIGNER, DO NOT MODIFY ***********/
/*********************************************************************************/
#include <new>
#include <gui_generated/common/FrontendApplicationBase.hpp>
#include <gui/common/FrontendHeap.hpp>
#include <touchgfx/transitions/NoTransition.hpp>
#include <texts/TextKeysAndLanguages.hpp>
#include <touchgfx/Texts.hpp>
#include <touchgfx/hal/HAL.hpp>
#include <platform/driver/lcd/LCD16bpp.hpp>
#include <gui/startscreen_screen/StartScreenView.hpp>
#include <gui/startscreen_screen/StartScreenPresenter.hpp>
#include <gui/appmenuscreen_screen/AppMenuScreenView.hpp>
#include <gui/appmenuscreen_screen/AppMenuScreenPresenter.hpp>
#include <gui/appactivityscreen_screen/AppActivityScreenView.hpp>
#include <gui/appactivityscreen_screen/AppActivityScreenPresenter.hpp>
#include <gui/debuggpsdatascreen_screen/DebugGpsDataScreenView.hpp>
#include <gui/debuggpsdatascreen_screen/DebugGpsDataScreenPresenter.hpp>
#include <gui/debugsdcardscreen_screen/DebugSdCardScreenView.hpp>
#include <gui/debugsdcardscreen_screen/DebugSdCardScreenPresenter.hpp>
#include <gui/debugenvsensorsscreen_screen/DebugEnvSensorsScreenView.hpp>
#include <gui/debugenvsensorsscreen_screen/DebugEnvSensorsScreenPresenter.hpp>

using namespace touchgfx;

FrontendApplicationBase::FrontendApplicationBase(Model& m, FrontendHeap& heap)
    : touchgfx::MVPApplication(),
      transitionCallback(),
      frontendHeap(heap),
      model(m)
{
    touchgfx::HAL::getInstance()->setDisplayOrientation(touchgfx::ORIENTATION_PORTRAIT);
    touchgfx::Texts::setLanguage(GB);
    reinterpret_cast<touchgfx::LCD16bpp&>(touchgfx::HAL::lcd()).enableTextureMapperAll();
}

/*
 * Screen Transition Declarations
 */

// StartScreen

void FrontendApplicationBase::gotoStartScreenScreenNoTransition()
{
    transitionCallback = touchgfx::Callback<FrontendApplicationBase>(this, &FrontendApplication::gotoStartScreenScreenNoTransitionImpl);
    pendingScreenTransitionCallback = &transitionCallback;
}

void FrontendApplicationBase::gotoStartScreenScreenNoTransitionImpl()
{
    touchgfx::makeTransition<StartScreenView, StartScreenPresenter, touchgfx::NoTransition, Model >(&currentScreen, &currentPresenter, frontendHeap, &currentTransition, &model);
}

// AppMenuScreen

void FrontendApplicationBase::gotoAppMenuScreenScreenNoTransition()
{
    transitionCallback = touchgfx::Callback<FrontendApplicationBase>(this, &FrontendApplication::gotoAppMenuScreenScreenNoTransitionImpl);
    pendingScreenTransitionCallback = &transitionCallback;
}

void FrontendApplicationBase::gotoAppMenuScreenScreenNoTransitionImpl()
{
    touchgfx::makeTransition<AppMenuScreenView, AppMenuScreenPresenter, touchgfx::NoTransition, Model >(&currentScreen, &currentPresenter, frontendHeap, &currentTransition, &model);
}

// AppActivityScreen

void FrontendApplicationBase::gotoAppActivityScreenScreenNoTransition()
{
    transitionCallback = touchgfx::Callback<FrontendApplicationBase>(this, &FrontendApplication::gotoAppActivityScreenScreenNoTransitionImpl);
    pendingScreenTransitionCallback = &transitionCallback;
}

void FrontendApplicationBase::gotoAppActivityScreenScreenNoTransitionImpl()
{
    touchgfx::makeTransition<AppActivityScreenView, AppActivityScreenPresenter, touchgfx::NoTransition, Model >(&currentScreen, &currentPresenter, frontendHeap, &currentTransition, &model);
}

// DebugGpsDataScreen

void FrontendApplicationBase::gotoDebugGpsDataScreenScreenNoTransition()
{
    transitionCallback = touchgfx::Callback<FrontendApplicationBase>(this, &FrontendApplication::gotoDebugGpsDataScreenScreenNoTransitionImpl);
    pendingScreenTransitionCallback = &transitionCallback;
}

void FrontendApplicationBase::gotoDebugGpsDataScreenScreenNoTransitionImpl()
{
    touchgfx::makeTransition<DebugGpsDataScreenView, DebugGpsDataScreenPresenter, touchgfx::NoTransition, Model >(&currentScreen, &currentPresenter, frontendHeap, &currentTransition, &model);
}

// DebugSdCardScreen

void FrontendApplicationBase::gotoDebugSdCardScreenScreenNoTransition()
{
    transitionCallback = touchgfx::Callback<FrontendApplicationBase>(this, &FrontendApplication::gotoDebugSdCardScreenScreenNoTransitionImpl);
    pendingScreenTransitionCallback = &transitionCallback;
}

void FrontendApplicationBase::gotoDebugSdCardScreenScreenNoTransitionImpl()
{
    touchgfx::makeTransition<DebugSdCardScreenView, DebugSdCardScreenPresenter, touchgfx::NoTransition, Model >(&currentScreen, &currentPresenter, frontendHeap, &currentTransition, &model);
}

// DebugEnvSensorsScreen

void FrontendApplicationBase::gotoDebugEnvSensorsScreenScreenNoTransition()
{
    transitionCallback = touchgfx::Callback<FrontendApplicationBase>(this, &FrontendApplication::gotoDebugEnvSensorsScreenScreenNoTransitionImpl);
    pendingScreenTransitionCallback = &transitionCallback;
}

void FrontendApplicationBase::gotoDebugEnvSensorsScreenScreenNoTransitionImpl()
{
    touchgfx::makeTransition<DebugEnvSensorsScreenView, DebugEnvSensorsScreenPresenter, touchgfx::NoTransition, Model >(&currentScreen, &currentPresenter, frontendHeap, &currentTransition, &model);
}
