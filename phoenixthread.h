#ifndef PHOENIXTHREAD_H
#define PHOENIXTHREAD_H


#include <QThread>
#include<QSharedMemory>

#include "telemetrystruct.h"
#include "currentLimits.h"
#include "robotstatestruct.h"
#include "RGBControlStruct.h"

#include "drivetrain.h"
#include "lifttable.h"
#include "rgbs.h"

class PhoenixThread : public QThread
{


public:
    PhoenixThread();
    void run();
    void quit();
    void sleepApp(int ms);

private:
    //SharedMem
    QSharedMemory sharedTelemetry;
    QSharedMemory sharedRobotState;
    QSharedMemory sharedCurrentLimits;
    QSharedMemory sharedRGBControl;

    //Thread Comm Structs
    Telemetry telemetry;

    CurrentLimits currentLimits;
    CurrentLimits lastCurrentLimits;

    RGBControlStruct rgbControl;
    RGBControlStruct lastRGBControl;

    RobotStateStruct robotState;
    RobotStateStruct lastRobotState;

    //Subsystems
    Drivetrain drivetrain;
    LiftTable liftTable;
    RGBs rgbs;

    void readSharedMemory();
    void writeSharedMemory();

    double handleDeadZone(double input);

    bool AreRGBControlStructsEqual(RGBControlStruct one, RGBControlStruct two);

    void updateRGBs();

};


#endif // PHOENIXTHREAD_H

