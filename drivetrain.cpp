#include "drivetrain.h"
#include "cartmap.h"

Drivetrain::Drivetrain():
    leftFront(LEFT_FRONT_ID),
    leftMid(LEFT_MID_ID),
    leftRear(LEFT_REAR_ID),
    rightFront(RIGHT_FRONT_ID),
    rightMid(RIGHT_MID_ID),
    rightRear(RIGHT_REAR_ID)
{
    leftMid.Follow(leftFront);
    leftRear.Follow(leftFront);

    rightMid.Follow(rightFront);
    rightRear.Follow(rightFront);

    setNeutralMode(NeutralMode::Brake);
}

void Drivetrain::CheesyDrive(double throttle, double steering, bool turnInPlace)
{
    double turnPower = steering * std::abs(throttle);
    if(turnInPlace)
    {
        turnPower = steering;
    }
    leftFront.Set(ControlMode::PercentOutput, throttle + turnPower);
    rightFront.Set(ControlMode::PercentOutput, throttle - turnPower);
}

void Drivetrain::SetCurrentLimit(double newCurrentLimit)
{
    leftFront.EnableCurrentLimit(true);
    leftMid.EnableCurrentLimit(true);
    leftRear.EnableCurrentLimit(true);

    rightFront.EnableCurrentLimit(true);
    rightMid.EnableCurrentLimit(true);
    rightRear.EnableCurrentLimit(true);

    leftFront.ConfigContinuousCurrentLimit(newCurrentLimit, 10);
    leftMid.ConfigContinuousCurrentLimit(newCurrentLimit, 10);
    leftRear.ConfigContinuousCurrentLimit(newCurrentLimit, 10);

    rightFront.ConfigContinuousCurrentLimit(newCurrentLimit, 10);
    rightMid.ConfigContinuousCurrentLimit(newCurrentLimit, 10);
    rightRear.ConfigContinuousCurrentLimit(newCurrentLimit, 10);

    rightFront.SetInverted(true);
}

void Drivetrain::updateTelemetry(Telemetry &telemetry)
{
    telemetry.leftFrontAmps = leftFront.GetStatorCurrent();
    telemetry.leftMidAmps = leftMid.GetStatorCurrent();
    telemetry.leftRearAmps = leftRear.GetStatorCurrent();
    telemetry.rightFrontAmps = rightFront.GetStatorCurrent();
    telemetry.rightMidAmps = rightMid.GetStatorCurrent();
    telemetry.rightRearAmps = rightRear.GetStatorCurrent();

    telemetry.leftDriveDutyCycle = leftFront.GetMotorOutputPercent();
    telemetry.rightDriveDutyCycle = rightFront.GetMotorOutputPercent();

    telemetry.voltage = leftFront.GetBusVoltage();


    telemetry.totalPowerWatts +=
            (telemetry.leftFrontAmps *leftFront.GetMotorOutputVoltage()
             + telemetry.leftMidAmps * leftMid.GetMotorOutputVoltage()
             + telemetry.leftRearAmps * leftRear.GetMotorOutputVoltage()
             + telemetry.rightFrontAmps * rightFront.GetMotorOutputVoltage()
             + telemetry.rightMidAmps * rightMid.GetMotorOutputVoltage()
             + telemetry.rightRearAmps * rightRear.GetMotorOutputVoltage());
}

void Drivetrain::setNeutralMode(NeutralMode newNeutralMode)
{
    leftFront.SetNeutralMode(newNeutralMode);
    leftMid.SetNeutralMode(newNeutralMode);
    leftRear.SetNeutralMode(newNeutralMode);

    rightFront.SetNeutralMode(newNeutralMode);
    rightMid.SetNeutralMode(newNeutralMode);
    rightRear.SetNeutralMode(newNeutralMode);
}

void Drivetrain::stop()
{
    leftFront.Set(ControlMode::PercentOutput, 0.0);
    rightFront.Set(ControlMode::PercentOutput, 0.0);
}
