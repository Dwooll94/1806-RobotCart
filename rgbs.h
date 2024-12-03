#define Phoenix_No_WPI // remove WPI dependencies
#ifndef RGBS_H
#define RGBS_H

#include "ctre/Phoenix.h"
#include "RGBControlStruct.h"
#include "festiveanimation.h"

class RGBs
{
public:
    RGBs();
    void StartAnimationFromControlStruct(RGBControlStruct controlStruct, bool enabled);
    void EnableCart();
    void DisableCart();
    void EStopCart();

    void updateActiveCustomAnimations(RGBControlStruct controlStruct);
private:
    CANdle candle;
    bool override;

    FestiveAnimation mFestiveAnimation;


};

#endif // RGBS_H
