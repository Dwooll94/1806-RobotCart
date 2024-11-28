#define Phoenix_No_WPI // remove WPI dependencies
#ifndef RGBS_H
#define RGBS_H

#include "ctre/Phoenix.h"
#include "RGBControlStruct.h"

class RGBs
{
public:
    RGBs();
    void StartAnimationFromControlStruct(RGBControlStruct controlStruct, bool enabled);
    void EnableCart();
    void DisableCart();
    void EStopCart();

private:
    CANdle candle;
    bool override;

};

#endif // RGBS_H
