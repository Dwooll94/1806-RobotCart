#pragma once
#ifndef RGBCONTROLSTRUCT_H
#define RGBCONTROLSTRUCT_H

#include <QString>

enum RGBAnimationType{
    ColorFlow,
    Fire,
    Larson,
    Rainbow,
    RGBFade,
    SingleFade,
    Strobe,
    Twinkle,
    TwinkleOff
};

struct RGBControlStruct
{
    RGBAnimationType animationType;
    int red;
    int green;
    int blue;
    int white;
    int twinklePercent;
    double brightness;
    double speed;
    double sparking;
    double cooling;
    bool override;
};

#endif // RGBCONTROLSTRUCT_H
