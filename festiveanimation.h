#define Phoenix_No_WPI // remove WPI dependencies
#ifndef FESTIVEANIMATION_H
#define FESTIVEANIMATION_H

#include "ctre/Phoenix.h"
#include <chrono>

class FestiveAnimation
{
public:
    FestiveAnimation(int moveSpeed, int blockSize, int ledCount, int framesPerSec);
    void update(CANdle& candle);

private:
    void setLEDsRed(int start, int count, CANdle& candle);
    void setLEDsGreen(int start, int count, CANdle& candle);
    unsigned mCounter;
    int mMoveSpeed;
    int mBlockSize;
    int mLedCount;
    int mFPS;
    std::chrono::time_point<std::chrono::system_clock> mLastUpdateTime;
    std::chrono::duration<int64_t, std::milli> mUpdateDuration;

};

#endif // FESTIVEANIMATION_H
