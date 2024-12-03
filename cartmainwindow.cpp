#include "cartmainwindow.h"
#include "./ui_cartmainwindow.h"
#include <QString>
#include <QSharedData>
#include <QObject>
#include <QThread>
#include <QTimer>
#include <QComboBox>
#include <cstring>
#include <chrono>
#include "phoenixthread.h"
#include "sharedmemorykeys.h"
#include "telemetrystruct.h"
#include "robotstatestruct.h"
#include "RGBControlStruct.h"
#include "currentLimits.h"
#include <iostream>

CartMainWindow::CartMainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::CartMainWindow)
    , sharedRobotState(SharedMemoryKeys::CART_STATE_KEY)
    , sharedTelemetry(SharedMemoryKeys::TELEMETRY_KEY)
    , sharedCurrentLimits(SharedMemoryKeys::CURRENT_LIMITS_KEY)
    , sharedRGBControl(SharedMemoryKeys::RGB_CONTROL_KEY)
{

    if(!sharedRobotState.create(sizeof(robotState)))
    {
        printf("Unable to create robot state shared memory");
        std::cout << "Error creating shared memory:" << sharedRobotState.errorString().toStdString();
    }

    if(!sharedTelemetry.create(sizeof(telemetry)))
    {
        printf("Unable to create telemetry shared memory");
        std::cout << "Error creating shared memory:" << sharedTelemetry.errorString().toStdString();
    }
    if(!sharedCurrentLimits.create(sizeof(currentLimits)))
    {
        printf("Unable to create current limits shared memory");
        std::cout << "Error creating shared memory:" + sharedCurrentLimits.errorString().toStdString();
    }
    if(!sharedRGBControl.create(sizeof(rgbControl)))
    {
        printf("Unable to create rgb control shared memory");
        std::cout << "Error creating shared memory:" + sharedRGBControl.errorString().toStdString();
    }

    std::cout<< "Attaching Shared Memory" << std::endl;
    sharedRobotState.attach();
    sharedTelemetry.attach();
    sharedCurrentLimits.attach();
    sharedRGBControl.attach();



    std::cout << "Running UI Setup" << std::endl;
    ui->setupUi(this);
    setupRGBOptions();
    on_loadRGBDefault_clicked();


    std::cout<< "Loading Default Current Limits" << std::endl;
    //TODO: Load Defaults
    currentLimits.DriveCurrentLimit = ui->driveCurrentLimit->value();
    currentLimits.LiftCurrentLimit = ui->liftCurrentLimitDial->value();
    currentLimits.driveBrake = true;
    currentLimits.liftBrake = true;

    std::cout<< "Setting Disable Mode" << std::endl;

    setDisabled();
    robotState.driveEnable = !ui->disableDrive->isChecked();
    robotState.liftEnable = !ui->disableLift->isChecked();


    robotState.currentRobotState = RobotState::Disabled;

    rgbControl.override = false;

    std::cout << "Initialize Shared Memory Values" << std::endl;
    postCurrentLimits();
    postTelemetry();
    postRGBControl();
    postRobotStateUpdate();


    std::cout << "Starting Cart Control Thread" << std::endl;
    phoenixThread = new PhoenixThread();
    phoenixThread->start();

    std::cout << "Starting Telemetry" << std::endl;

   QTimer *timer = new QTimer(this);
   connect(timer, SIGNAL(timeout()), this, SLOT(readTelemetry()));
   timer->start(100);
}

void CartMainWindow::setupRGBOptions()
{
    QComboBox* box = ui->animationTypeBox;
    box->addItem("Color Flow", QVariant::fromValue(RGBAnimationType::ColorFlow));
    box->addItem("Larson", QVariant::fromValue(RGBAnimationType::Larson));
    box->addItem("Fire", QVariant::fromValue(RGBAnimationType::Fire));
    box->addItem("Rainbow", QVariant::fromValue(RGBAnimationType::Rainbow));
    box->addItem("RGBFade", QVariant::fromValue(RGBAnimationType::RGBFade));
    box->addItem("Single Fade", QVariant::fromValue(RGBAnimationType::SingleFade));
    box->addItem("Strobe", QVariant::fromValue(RGBAnimationType::Strobe));
    box->addItem("Twinkle", QVariant::fromValue(RGBAnimationType::Twinkle));
    box->addItem("TwinkleOff", QVariant::fromValue(RGBAnimationType::TwinkleOff));
    box->addItem("Festive", QVariant::fromValue(RGBAnimationType::Festive));
    rgbControl.twinklePercent = 6;
}

CartMainWindow::~CartMainWindow()
{
    delete ui;
    robotState.currentRobotState = RobotState::Disabled;
    postRobotStateUpdate();
    sharedRobotState.detach();
    sharedTelemetry.detach();
    sharedCurrentLimits.detach();
    sharedRGBControl.detach();
    phoenixThread->quit();
}

//void CartMainWindow::PushMessageToConsole(QString message)
//{
//    ui->console->append(message.append("\n"));
//    ui->console->scroll(0, 0);
//}

void CartMainWindow::postRobotStateUpdate()
{
    sharedRobotState.lock();
    std::memcpy(sharedRobotState.data(), &robotState, sizeof(robotState));
    sharedRobotState.unlock();
}

void CartMainWindow::postTelemetry()
{
    sharedTelemetry.lock();
    std::memcpy(sharedTelemetry.data(), &telemetry, sizeof(telemetry));
    sharedTelemetry.unlock();
}

void CartMainWindow::postCurrentLimits()
{
    sharedCurrentLimits.lock();
    std::memcpy(sharedCurrentLimits.data(), &currentLimits, sizeof(currentLimits));
    sharedCurrentLimits.unlock();
}

void CartMainWindow::postRGBControl()
{
    sharedRGBControl.lock();
    std::memcpy(sharedRGBControl.data(), &rgbControl, sizeof(rgbControl));
    sharedRGBControl.unlock();
}

void CartMainWindow::readTelemetry()
{
    sharedTelemetry.lock();
    std::memcpy(&telemetry, sharedTelemetry.data(), sizeof(telemetry));
    sharedTelemetry.unlock();

    //std::cout<< "Updating telemetry" << std::endl;
    ui->voltageDisplay->display(telemetry.voltage);
    ui->voltageDisplay->update();
    ui->leftDriveProgressBar->setValue(telemetry.leftDriveDutyCycle * 1000.0);
    ui->rightDriveProgressBar->setValue(telemetry.rightDriveDutyCycle * 1000.0);
    ui->leftFrontAmpsLCD->display(telemetry.leftFrontAmps);
    ui->leftMidAmpsLCD->display(telemetry.leftMidAmps);
    ui->leftRearAmpsLCD->display(telemetry.leftRearAmps);

    ui->rightFrontAmpsLCD->display(telemetry.rightFrontAmps);
    ui->rightMidAmpsLCD->display(telemetry.rightMidAmps);
    ui->rightRearAmpsLCD->display(telemetry.rightRearAmps);

    ui->liftPowerProgressBar->setValue(telemetry.liftDutyCycle * 1000.0);

    ui->powerOutputBar->setValue(telemetry.totalPowerWatts);


}
void CartMainWindow::setDisabled()
{
    ui->enableDisableButton->setText("Disabled");
    ui->enableDisableButton->setStyleSheet
            ("QPushButton{background-color: qlineargradient(spread:reflect, x1:0.5, y1:0.75, x2:1, y2:0.75, stop:0.705263 rgba(225, 18, 0, 255), stop:0.994764 rgba(0, 0, 0, 255));color:white;}");
    robotState.currentRobotState = RobotState::Disabled;
    postRobotStateUpdate();
    std::cout<< "Disabled" << std::endl;
}

void CartMainWindow::setEnabled()
{
    ui->enableDisableButton->setText("Enabled");
    ui->enableDisableButton->setStyleSheet("QPushButton{background-color: qlineargradient(spread:reflect, x1:0.5, y1:0.75, x2:1, y2:0.75, stop:0.705263 rgba(0, 225, 0, 255), stop:0.994764 rgba(0, 0, 0, 255));color:white;}");
    robotState.currentRobotState = RobotState::Enabled;
    robotState.lastEnableTime = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
    postRobotStateUpdate();
    std::cout <<"Enabled" << std::endl;
}
void CartMainWindow::handleEStop(QString s)
{

}

void CartMainWindow::on_enableDisableButton_clicked()
{
    if(robotState.currentRobotState == RobotState::Enabled)
    {
        setDisabled();
    }
    else if(robotState.currentRobotState == RobotState::Disabled)
    {
        setEnabled();
    }
}


void CartMainWindow::on_disableDrive_stateChanged(int arg1)
{
    robotState.driveEnable = !ui->disableDrive->isChecked();
    postRobotStateUpdate();
}


void CartMainWindow::on_disableLift_stateChanged(int arg1)
{
    robotState.liftEnable = !ui->disableLift->isChecked();
    postRobotStateUpdate();
}


void CartMainWindow::on_driveCurrentLimit_valueChanged(int value)
{
    currentLimits.DriveCurrentLimit = value;
    postCurrentLimits();
}


void CartMainWindow::on_driveBrakeBox_stateChanged(int arg1)
{
    currentLimits.driveBrake = ui->driveBrakeBox->isChecked();
    postCurrentLimits();
}


void CartMainWindow::on_checkBox_stateChanged(int arg1)
{

}


void CartMainWindow::on_RedSlider_valueChanged(int value)
{
    rgbControl.red = value;
    postRGBControl();
}


void CartMainWindow::on_BlueSlider_valueChanged(int value)
{
    rgbControl.blue = value;
    postRGBControl();
}


void CartMainWindow::on_GreenSlider_valueChanged(int value)
{
    rgbControl.green = value;
    postRGBControl();
}


void CartMainWindow::on_WhiteSlider_valueChanged(int value)
{
    rgbControl.white = value;
    postRGBControl();
}


void CartMainWindow::on_BrightnessSlider_valueChanged(int value)
{
    rgbControl.brightness = (value / 1000.0);
    postRGBControl();
}


void CartMainWindow::on_SpeedSlider_valueChanged(int value)
{
    rgbControl.speed = (value / 1000.0);
    postRGBControl();
}


void CartMainWindow::on_animationTypeBox_currentIndexChanged(int index)
{
    rgbControl.animationType = this->ui->animationTypeBox->itemData(index).value<RGBAnimationType>();
    postRGBControl();
}


void CartMainWindow::on_comboBox_currentTextChanged(const QString &arg1)
{
    rgbControl.twinklePercent = arg1.toInt();
    postRGBControl();
}


void CartMainWindow::on_sparkingSlider_valueChanged(int value)
{
    rgbControl.sparking = (value / 1000.0);
    postRGBControl();
}


void CartMainWindow::on_coolingSlider_valueChanged(int value)
{
    rgbControl.cooling = (value / 1000.0);
    postRGBControl();
}


void CartMainWindow::on_overrideDefault_toggled(bool checked)
{
    rgbControl.override = checked;
    postRGBControl();
}


void CartMainWindow::on_SpeedSlider_sliderReleased()
{

}


void CartMainWindow::on_animationTypeBox_currentTextChanged(const QString &arg1)
{

}


void CartMainWindow::on_EStopSlider_valueChanged(int value)
{
    if(value == 8)
    {
        robotState.currentRobotState = RobotState::EmergencyStop;
        postRobotStateUpdate();
        ui->enableDisableButton->setDisabled(true);
        ui->enableDisableButton->setText("E-STOPPED");
        ui->enableDisableButton->setStyleSheet
                ("QPushButton{background-color: qlineargradient(spread:reflect, x1:0.5, y1:0.75, x2:1, y2:0.75, stop:0.705263 rgba(225, 18, 0, 255), stop:0.994764 rgba(0, 0, 0, 255));color:gray;}");
    }
}


void CartMainWindow::on_CloseApp_clicked()
{
    phoenixThread->quit();
    close();
    exit(0);
}


void CartMainWindow::on_liftBrakeBox_stateChanged(int arg1)
{
    std::cout << "Ui lift brake triggered" << std::endl;
    currentLimits.liftBrake = ui->liftBrakeBox->isChecked();
    postCurrentLimits();
}


void CartMainWindow::on_loadRGBDefault_clicked()
{
    rgbControl = rgbFileManager.loadDefaultControlStruct();
    populateRgbUiFromCurrentStruct();
    postRGBControl();
}


void CartMainWindow::on_saveRGBDefault_clicked()
{
    rgbFileManager.saveDefaultControlStruct(rgbControl);
}


void CartMainWindow::on_loadRGBFromFile_clicked()
{
    RGBControlStruct temp;
    if(rgbFileManager.loadSavedRGBControlStruct(temp))
    {
        rgbControl = temp;
        populateRgbUiFromCurrentStruct();
        postRGBControl();
    }
}


void CartMainWindow::on_saveRGBToFile_clicked()
{
    rgbFileManager.saveRGBControlStructToFile(rgbControl);
}

void CartMainWindow::populateRgbUiFromCurrentStruct()
{
    ui->overrideDefault->setChecked(rgbControl.override);
    ui->RedSlider->setValue(rgbControl.red);
    ui->GreenSlider->setValue(rgbControl.green);
    ui->BlueSlider->setValue(rgbControl.blue);
    ui->WhiteSlider->setValue(rgbControl.white);
    ui->BrightnessSlider->setValue(floor(rgbControl.brightness * 1000.0));
    ui->SpeedSlider->setValue(floor(rgbControl.speed * 1000.0));
    ui->sparkingSlider->setValue(floor(rgbControl.sparking * 1000.0));
    ui->coolingSlider->setValue(floor(rgbControl.cooling * 1000.0));

    RGBAnimationType type = static_cast<RGBAnimationType>(rgbControl.animationType);
    int typeIndex = ui->animationTypeBox->findData(QVariant::fromValue(type));
    if( typeIndex != -1)
    {
        ui->animationTypeBox->setCurrentIndex(typeIndex);
    }
    else
    {
        std::cout << "Could not find animation type in combo box" << std::endl;
    }

    int twinkleIndex = ui->comboBox->findText(QString::number(rgbControl.twinklePercent));
    if(twinkleIndex != -1)
    {
        ui->comboBox->setCurrentIndex(twinkleIndex);
    }
    else
    {
        std::cout << "Could not find twinkle percent in combo box" << std::endl;
    }
}

