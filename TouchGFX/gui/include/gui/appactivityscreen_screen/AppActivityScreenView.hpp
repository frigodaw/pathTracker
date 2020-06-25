#ifndef APPACTIVITYSCREENVIEW_HPP
#define APPACTIVITYSCREENVIEW_HPP

#include <gui_generated/appactivityscreen_screen/AppActivityScreenViewBase.hpp>
#include <gui/appactivityscreen_screen/AppActivityScreenPresenter.hpp>

#define APP_ALPHA_FULLVISIBLE           255u
#define APP_ALPHA_INVISIBLE             0u

class AppActivityScreenView : public AppActivityScreenViewBase
{
public:
    AppActivityScreenView();
    ~AppActivityScreenView() {}
    void setupScreen();
    void tearDownScreen();

    void StartStopActivity(void);
    void FinishActivity(void);
    void ZoomIn(void);
    void ZoomOut(void);
    void ChangeActivityDataCC(void);
    void SetActivityDataScreen(AppActivity_activeScreen_T screen);
    void SetBitmapButton(const uint16_t bitmapId);
    void ShowFixImage(bool isFix);
    void ShowSdCard(bool isSdCard);

    void FlushTrackList(void);
    void SetTrackScale(uint32_t scaleVal);
    bool AddCoordsToTrackList(AppActivity_coordinatesXY_T coordsXY);
    void TrackRedraw(void);

    void ConfirmMapSelection(void);
    void DisplayPreviousMap(void);
    void DisplayNextMap(void);

    void NotifySignalChanged_gpsData_fixQuality(uint8_t newFixQuality);
    void NotifySignalChanged_activityData_timer(uint32_t newTimer);
    void NotifySignalChanged_activityData_distance(float newDistance);
    void NotifySignalChanged_activityData_time(uint32_t newTime);
    void NotifySignalChanged_activityData_speed(float newSpeed);
    void NotifySignalChanged_activityData_avgSpeed(float newAvgSpeed);
    void NotifySignalChanged_activityData_maxSpeed(float newMaxSpeed);
    void NotifySignalChanged_activityData_altitude(float newAltitude);
    void NotifySignalChanged_activityData_slope(float newSlope);
    void NotifySignalChanged_activityData_altiUp(int32_t newAltiUp);
    void NotifySignalChanged_activityData_altiDown(int32_t newAltiDown);
    void NotifySignalChanged_activityData_altiMax(int32_t newAltiMax);
    void NotifySignalChanged_activityData_slopeMax(float newSlopeMax);
    void NotifySignalChanged_sensorData_altitude(float newAltitude);
protected:
};

#endif // APPACTIVITYSCREENVIEW_HPP
