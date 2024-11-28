#ifndef CARTMAINWINDOW_H
#define CARTMAINWINDOW_H

#include <QMainWindow>
#include <QSharedMemory>
#include "currentLimits.h"
#include "telemetrystruct.h"
#include "robotstatestruct.h"
#include "RGBControlStruct.h"
#include "phoenixthread.h"
#include "rgbfilemanager.h"

QT_BEGIN_NAMESPACE
namespace Ui { class CartMainWindow; }
QT_END_NAMESPACE

class CartMainWindow : public QMainWindow
{
    Q_OBJECT

public:
    CartMainWindow(QWidget *parent = nullptr);
    ~CartMainWindow();

//    void PushMessageToConsole(QString message);

private slots:
    void on_enableDisableButton_clicked();

    void on_disableDrive_stateChanged(int arg1);

    void on_disableLift_stateChanged(int arg1);

    void on_driveCurrentLimit_valueChanged(int value);

    void readTelemetry();

    void on_driveBrakeBox_stateChanged(int arg1);

    void on_checkBox_stateChanged(int arg1);


    void on_RedSlider_valueChanged(int value);

    void on_BlueSlider_valueChanged(int value);

    void on_GreenSlider_valueChanged(int value);

    void on_WhiteSlider_valueChanged(int value);

    void on_BrightnessSlider_valueChanged(int value);

    void on_SpeedSlider_sliderReleased();

    void on_SpeedSlider_valueChanged(int value);

    void on_animationTypeBox_currentTextChanged(const QString &arg1);

    void on_animationTypeBox_currentIndexChanged(int index);

    void on_comboBox_currentTextChanged(const QString &arg1);

    void on_sparkingSlider_valueChanged(int value);

    void on_coolingSlider_valueChanged(int value);

    void on_overrideDefault_toggled(bool checked);

    void on_EStopSlider_valueChanged(int value);

    void on_CloseApp_clicked();

    void on_liftBrakeBox_stateChanged(int arg1);

    void on_loadRGBDefault_clicked();

    void on_saveRGBDefault_clicked();

    void on_loadRGBFromFile_clicked();

    void on_saveRGBToFile_clicked();

private:

    void setupRGBOptions();
    PhoenixThread* phoenixThread;
    RGBFileManager rgbFileManager;

    Ui::CartMainWindow *ui;
    QSharedMemory sharedTelemetry;
    QSharedMemory sharedCurrentLimits;
    QSharedMemory sharedRobotState;
    QSharedMemory sharedRGBControl;

    RobotStateStruct robotState;
    Telemetry telemetry;
    CurrentLimits currentLimits;
    RGBControlStruct rgbControl;

    void postRobotStateUpdate();
    void postTelemetry();
    void postCurrentLimits();
    void postRGBControl();

    void populateRgbUiFromCurrentStruct();

    void handleEStop(QString s);

    void setDisabled();
    void setEnabled();

};
#endif // CARTMAINWINDOW_H
