#include "lifttable.h"
#include "cartmap.h"
#include <iostream>

LiftTable::LiftTable():
    liftLeft(LIFT_LEFT_ID, MAIN_CAN_BUS), liftRight(LIFT_RIGHT_ID, MAIN_CAN_BUS)
{
    liftRight.Follow(liftLeft);
    SetNeutralMode(NeutralMode::Brake);
}

void LiftTable::RunLift(double power)
{
    liftLeft.Set(ControlMode::PercentOutput, power);
}

void LiftTable::SetCurrentLimit(double currentLimit)
{
    std::cout <<"Unimplimented LiftTable::SetCurrentLimit, VictorSPXes do not support current limit" << std::endl;
//    liftLeft.ConfigContinuousCurrentLimit(currentLimit, 10);
//    liftRight.ConfigContinuousCurrentLimit(currentLimit, 10);
}

void LiftTable::UpdateTelemetry(Telemetry &telemetry)
{
    telemetry.liftDutyCycle = liftLeft.GetMotorOutputPercent();
//    telemetry.liftLeftAmps = liftLeft.GetStatorCurrent();
//    telemetry.liftRightAmps = liftRight.GetStatorCurrent();
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
