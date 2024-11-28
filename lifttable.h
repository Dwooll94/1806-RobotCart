#define Phoenix_No_WPI // remove WPI dependencies
#ifndef LIFTTABLE_H
#define LIFTTABLE_H

#include "ctre/Phoenix.h"
#include "telemetrystruct.h"
#include "cartmap.h"

using namespace ctre::phoenix;
using namespace ctre::phoenix::platform;
using namespace ctre::phoenix::motorcontrol;
using namespace ctre::phoenix::motorcontrol::can;

class LiftTable
{
public:

    LiftTable();
    void UpdateTelemetry(Telemetry& telemetry);
    void SetCurrentLimit(double currentLimit);
    void RunLift(double power);
    void Stop();
    void SetNeutralMode(NeutralMode newMode);
private:

    VictorSPX liftLeft, liftRight;
};

#endif // LIFTTABLE_H
