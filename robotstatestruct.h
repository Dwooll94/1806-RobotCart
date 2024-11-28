#pragma once
#ifndef ROBOTSTATESTRUCT_H
#define ROBOTSTATESTRUCT_H

#include <chrono>

enum RobotState{
    Enabled,
    Disabled,
    EmergencyStop,

};

struct RobotStateStruct{
    RobotState currentRobotState;
    std::time_t lastEnableTime;
    bool driveEnable;
    bool liftEnable;
};

#endif // ROBOTSTATESTRUCT_H



