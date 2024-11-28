#pragma once
#ifndef CURRENTLIMITS_H
#define CURRENTLIMITS_H

struct CurrentLimits
{
    double DriveCurrentLimit;
    double LiftCurrentLimit;
    bool driveBrake;
    bool liftBrake;
};

#endif // CURRENTLIMITS_H

