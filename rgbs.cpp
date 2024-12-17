#include "rgbs.h"

#include "ctre/phoenix/led/ColorFlowAnimation.h"
#include "ctre/phoenix/led/FireAnimation.h"
#include "ctre/phoenix/led/LarsonAnimation.h"
#include "ctre/phoenix/led/RainbowAnimation.h"
#include "ctre/phoenix/led/RgbFadeAnimation.h"
#include "ctre/phoenix/led/SingleFadeAnimation.h"
#include "ctre/phoenix/led/StrobeAnimation.h"
#include "ctre/phoenix/led/TwinkleAnimation.h"
#include "ctre/phoenix/led/TwinkleOffAnimation.h"
#include <iostream>

const int LED_COUNT = 308;
TwinkleAnimation disableAnimation(25, 255, 0, 128, 0.2, LED_COUNT, TwinkleAnimation::TwinklePercent::Percent88);
ColorFlowAnimation enableAnimation(255, 255, 255, 255, 0.8, LED_COUNT, ColorFlowAnimation::Direction::Forward);
//FireAnimation enableAnimation(0.7, 0.7, LED_COUNT, 0.8, 0.6, false, 0);
StrobeAnimation estopAnimation(255, 0, 0, 128, 0.2, LED_COUNT);

RGBs::RGBs():
    candle(0), mFestiveAnimation(1, 20, LED_COUNT, 4)
{
    CANdleConfiguration configAll {};
    configAll.statusLedOffWhenActive = true;
    configAll.disableWhenLOS = false;
    configAll.stripType = LEDStripType::GRB;
    configAll.brightnessScalar = 0.1;
    configAll.vBatOutputMode = VBatOutputMode::Modulated;
    candle.ConfigAllSettings(configAll, 100);
    candle.Animate(disableAnimation, 0);

}

void RGBs::StartAnimationFromControlStruct(RGBControlStruct controlStruct, bool enabled)
{
    override = controlStruct.override;
    if(override)
    {

        switch(controlStruct.animationType)
        {
            case RGBAnimationType::ColorFlow:
            {
                ColorFlowAnimation cfa(controlStruct.red, controlStruct.green, controlStruct.blue, controlStruct.white, controlStruct.speed, LED_COUNT);
                candle.Animate(cfa, 0);
            break;
            }
            case RGBAnimationType::Larson:
            {
            LarsonAnimation la(controlStruct.red, controlStruct.green, controlStruct.blue, controlStruct.white, controlStruct.speed, LED_COUNT);
                candle.Animate(la, 0);
            break;
            }
            case RGBAnimationType::Fire:
            {
            FireAnimation fa(controlStruct.brightness, controlStruct.speed, LED_COUNT, controlStruct.sparking, controlStruct.cooling);
                candle.Animate(fa, 0);
            break;
            }
            case RGBAnimationType::Rainbow:
            {
            RainbowAnimation ra(controlStruct.brightness, controlStruct.speed, LED_COUNT);
                candle.Animate(ra, 0);
            break;
            }
            case RGBAnimationType::RGBFade:
            {
            RgbFadeAnimation rgbf(controlStruct.brightness, controlStruct.speed, LED_COUNT);
                candle.Animate(rgbf, 0);
            break;
            }
            case RGBAnimationType::SingleFade:
            {
            SingleFadeAnimation sf(controlStruct.red, controlStruct.green, controlStruct.blue, controlStruct.white, controlStruct.speed, LED_COUNT);
                candle.Animate(sf, 0);
            break;
            }
            case RGBAnimationType::Strobe:
            {
            StrobeAnimation str(StrobeAnimation(controlStruct.red, controlStruct.green, controlStruct.blue, controlStruct.white, controlStruct.speed, LED_COUNT));
                candle.Animate(str, 0);
            break;
            }
            case RGBAnimationType::Twinkle:
            {
            TwinkleAnimation::TwinklePercent percent;
                switch(controlStruct.twinklePercent)
                {
                case 6:
                        percent = TwinkleAnimation::TwinklePercent::Percent6;
                    break;
                case 18:
                    percent = TwinkleAnimation::TwinklePercent::Percent18;
                    break;
                case 30:
                    percent = TwinkleAnimation::TwinklePercent::Percent30;
                    break;
                case 42:
                    percent = TwinkleAnimation::TwinklePercent::Percent42;
                    break;
                case 64:
                    percent = TwinkleAnimation::TwinklePercent::Percent64;
                    break;
                case 76:
                    percent = TwinkleAnimation::TwinklePercent::Percent76;
                    break;
                case 88:
                    percent = TwinkleAnimation::TwinklePercent::Percent88;
                    break;
                case 100:
                    percent = TwinkleAnimation::TwinklePercent::Percent100;
                    break;
                }
                TwinkleAnimation ta(controlStruct.red, controlStruct.green, controlStruct.blue, controlStruct.white, controlStruct.speed, LED_COUNT, percent);
                candle.Animate(ta, 0);
            break;
            }
            case RGBAnimationType::TwinkleOff:
            {
            TwinkleOffAnimation::TwinkleOffPercent offPercent;
                switch(controlStruct.twinklePercent)
                {
                case 6:
                    offPercent = TwinkleOffAnimation::TwinkleOffPercent::Percent6;
                    break;
                case 18:
                    offPercent = TwinkleOffAnimation::TwinkleOffPercent::Percent18;
                    break;
                case 30:
                    offPercent = TwinkleOffAnimation::TwinkleOffPercent::Percent30;
                    break;
                case 42:
                    offPercent = TwinkleOffAnimation::TwinkleOffPercent::Percent42;
                    break;
                case 64:
                    offPercent = TwinkleOffAnimation::TwinkleOffPercent::Percent64;
                    break;
                case 76:
                    offPercent = TwinkleOffAnimation::TwinkleOffPercent::Percent76;
                    break;
                case 88:
                    offPercent = TwinkleOffAnimation::TwinkleOffPercent::Percent88;
                    break;
                case 100:
                    offPercent = TwinkleOffAnimation::TwinkleOffPercent::Percent100;
                    break;
                }
                TwinkleOffAnimation toa(controlStruct.red, controlStruct.green, controlStruct.blue, controlStruct.white, controlStruct.speed, LED_COUNT, offPercent);
                candle.Animate(toa, 0);
            break;
            }
            case RGBAnimationType::Festive:
            {
                candle.ClearAnimation(0);
                mFestiveAnimation.update(candle);
            }
        }
    }
    else
    {
        if(enabled)
        {
            EnableCart();
        }
        else
        {
            DisableCart();
        }
    }
}

void RGBs::EnableCart()
{
    if(!override)
    {
        std::cout << "RGBs enable" << std::endl;
        candle.Animate(enableAnimation, 0);
    }
}

void RGBs::DisableCart()
{
    if(!override)
    {
        std::cout << "RGBs disable"<< std::endl;
        candle.Animate(disableAnimation, 0);
    }
}

void RGBs::EStopCart()
{
    candle.Animate(estopAnimation, 0);
}

void RGBs::updateActiveCustomAnimations(RGBControlStruct controlStruct)
{
    if(controlStruct.override && (controlStruct.animationType == RGBAnimationType::Festive))
    {
        mFestiveAnimation.update(candle);
    }
}

