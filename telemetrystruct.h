#pragma once
#ifndef TELEMETRYSTRUCT_H
#define TELEMETRYSTRUCT_H

struct Telemetry
{
    //overall
    double totalPowerWatts;
    double voltage;

    //drive
    double leftDriveDutyCycle;
    double rightDriveDutyCycle;
    double leftFrontAmps;
    double leftMidAmps;
    double leftRearAmps;
    double rightFrontAmps;
    double rightMidAmps;
    double rightRearAmps;

    //lift
    double liftDutyCycle;
    double liftLeftAmps;
    double liftRightAmps;

};

#endif // TELEMETRYSTRUCT_H


