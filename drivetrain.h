#define Phoenix_No_WPI // remove WPI dependencies
#ifndef DRIVETRAIN_H
#define DRIVETRAIN_H

#include "ctre/Phoenix.h"
#include "telemetrystruct.h"
#include "cartmap.h"

using namespace ctre::phoenix;
using namespace ctre::phoenix::platform;
using namespace ctre::phoenix::motorcontrol;
using namespace ctre::phoenix::motorcontrol::can;

class Drivetrain
{
public:
    Drivetrain();
    void CheesyDrive(double throttle, double steering, bool turnInPlace);
    void SetCurrentLimit(double newCurrentLimit);
    void updateTelemetry(Telemetry& telemetry);
    void setNeutralMode(NeutralMode newNeutralMode);
    void stop();
private:
    TalonSRX leftFront;
    TalonSRX leftMid;
    TalonSRX leftRear;

    TalonSRX rightFront;
    TalonSRX rightMid;
    TalonSRX rightRear;

};

#endif // DRIVETRAIN_H
