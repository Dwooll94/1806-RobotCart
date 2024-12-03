#include "festiveanimation.h"

#include <chrono>

FestiveAnimation::FestiveAnimation(int moveSpeed, int blockSize, int ledCount, int framesPerSec)
{
    mCounter = 0;
    mMoveSpeed = moveSpeed;
    mBlockSize = blockSize;
    mLedCount = ledCount;
    mFPS = framesPerSec;
    mLastUpdateTime = std::chrono::system_clock::now();
    mUpdateDuration = std::chrono::milliseconds(1000/framesPerSec);
}

void FestiveAnimation::update(CANdle &candle)
{
    std::chrono::time_point now = std::chrono::system_clock::now();
    if(std::chrono::duration_cast<std::chrono::milliseconds>(now - mLastUpdateTime) > mUpdateDuration)
    {
        mLastUpdateTime = now;
        int currentLed = 0;
        int firstChange =((2*mBlockSize)-1)-(mCounter % (2 * mBlockSize));
        bool greenNext = (mCounter % (2 * mBlockSize)) >= mBlockSize;
        if(greenNext)
        {

                setLEDsGreen(0, firstChange, candle);
        }
        else
        {
                firstChange -= mBlockSize;
                setLEDsRed(0, firstChange, candle);
        }
        greenNext = !greenNext;
        currentLed = firstChange;

        while(currentLed < mLedCount)
        {
            if(greenNext)
            {
                setLEDsGreen(currentLed, mBlockSize, candle);
            }
            else
            {
                setLEDsRed(currentLed, mBlockSize, candle);
            }
            greenNext = !greenNext;
            currentLed += mBlockSize;
        }

        mCounter += mMoveSpeed;
    }

}


void FestiveAnimation::setLEDsRed(int start, int count, CANdle &candle)
{
    candle.SetLEDs(214, 0, 28, 128, start, count);
}

void FestiveAnimation::setLEDsGreen(int start, int count, CANdle &candle)
{
   candle.SetLEDs(0, 135, 62, 128, start, count);
}
