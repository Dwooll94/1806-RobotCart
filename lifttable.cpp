#include "lifttable.h"
#include "cartmap.h"
#include <iostream>

LiftTable::LiftTable():
    liftLeft(LIFT_LEFT_ID, MAIN_CAN_BUS), liftRight(LIFT_RIGHT_ID, MAIN_CAN_BUS)
{
    liftRight.Follow(liftLeft);
    SetNeutralMode(NeutralMode::Brake);
    liftLeft.ConfigOpenloopRamp(0.1, 10);
    liftRight.ConfigOpenloopRamp(0.1, 10);
    liftLeft.ConfigPeakCurrentLimit(0, 10);
    liftRight.ConfigPeakCurrentLimit(0, 10);
}

void LiftTable::RunLift(double power)
{
    liftLeft.Set(ControlMode::PercentOutput, power);
}

void LiftTable::SetCurrentLimit(double currentLimit)
{
    liftLeft.EnableCurrentLimit(true);
    liftRight.EnableCurrentLimit(true);
    liftLeft.ConfigContinuousCurrentLimit(currentLimit/4.0, 10);
    liftRight.ConfigContinuousCurrentLimit(currentLimit/4.0, 10);
    liftLeft.ConfigPeakCurrentLimit(currentLimit, 10);
    liftRight.ConfigPeakCurrentLimit(currentLimit, 10);
    liftLeft.ConfigPeakCurrentDuration(50, 10);
    liftRight.ConfigPeakCurrentDuration(50, 10);
}

void LiftTable::UpdateTelemetry(Telemetry &telemetry)
{
    telemetry.liftDutyCycle = liftLeft.GetMotorOutputPercent();
    telemetry.liftLeftAmps = liftLeft.GetStatorCurrent();
    telemetry.liftRightAmps = liftRight.GetStatorCurrent();
    telemetry.totalPowerWatts += (liftLeft.GetMotorOutputVoltage() * telemetry.liftLeftAmps)
                                    + (liftRight.GetMotorOutputVoltage() * telemetry.liftRightAmps);
}

void LiftTable::Stop()
{
    liftLeft.Set(ControlMode::PercentOutput, 0.0);
}

void LiftTable::SetNeutralMode(NeutralMode newMode)
{
    std::cout <<"Setting lift neutral mode" << std::endl;
    liftLeft.SetNeutralMode(newMode);
    liftRight.SetNeutralMode(newMode);
}
