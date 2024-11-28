#define Phoenix_No_WPI // remove WPI dependencies
#include "ctre/Phoenix.h"
#include "ctre/phoenix/platform/Platform.hpp"
#include "ctre/phoenix/unmanaged/Unmanaged.h"
#include <chrono>
#include <iostream>
#include <string>
#include <cstring>
#include <thread>
#include <unistd.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_events.h>
#include <SDL2/SDL_joystick.h>
#include "phoenixthread.h"
#include <QSharedMemory>
#include "sharedmemorykeys.h"
#include <QThread>
#include "RGBControlStruct.h"


PhoenixThread::PhoenixThread():
    sharedTelemetry(SharedMemoryKeys::TELEMETRY_KEY),
    sharedRobotState(SharedMemoryKeys::CART_STATE_KEY),
    sharedCurrentLimits(SharedMemoryKeys::CURRENT_LIMITS_KEY),
    sharedRGBControl(SharedMemoryKeys::RGB_CONTROL_KEY)
{

}

void PhoenixThread::quit()
{
    drivetrain.stop();
    liftTable.Stop();
    rgbs.DisableCart();
    sleepApp(100);// let final messages clear
    sharedTelemetry.detach();
    sharedRobotState.detach();
    sharedCurrentLimits.detach();
    sharedRGBControl.detach();
    SDL_Quit();
    QThread::quit();
}

void PhoenixThread::run()
{
    sleepApp(1500); //let phoenix init
    //init joystick
    sharedTelemetry.attach();
    sharedRobotState.attach();
    sharedCurrentLimits.attach();
    sharedRGBControl.attach();
    readSharedMemory();
    drivetrain.stop();
    liftTable.Stop();
    drivetrain.setNeutralMode(currentLimits.driveBrake?NeutralMode::Brake:NeutralMode::Coast);
    liftTable.SetNeutralMode((currentLimits.liftBrake?NeutralMode::Brake:NeutralMode::Coast));
    rgbs.StartAnimationFromControlStruct(rgbControl, false);
    rgbs.DisableCart();
    updateRGBs();


    while (true) {
            /* we are looking for gamepad (first time or after disconnect),
                neutral drive until gamepad (re)connected. */


            // wait for gamepad
            printf("Waiting for gamepad...\n");
            while (true) {
                /* SDL seems somewhat fragile, shut it down and bring it up */
                SDL_Quit();
                SDL_SetHint(SDL_HINT_NO_SIGNAL_HANDLERS, "1"); //so Ctrl-C still works
                SDL_Init(SDL_INIT_JOYSTICK);

                /* poll for gamepad */
                int res = SDL_NumJoysticks();
                if (res > 0) { break; }
                if (res < 0) { printf("Err = %i\n", res); }

                /* yield for a bit */
                sleepApp(20);
            }
            printf("Waiting for gamepad...Found one\n");

            // Open the joystick for reading and store its handle in the joy variable
            SDL_Joystick *joy = SDL_JoystickOpen(0);
            if (joy == NULL) {
                /* back to top of while loop */
                continue;
            }

            // Get information about the joystick
            const char *name = SDL_JoystickName(joy);
            const int num_axes = SDL_JoystickNumAxes(joy);
            const int num_buttons = SDL_JoystickNumButtons(joy);
            const int num_hats = SDL_JoystickNumHats(joy);
            printf("Now reading from joystick '%s' with:\n"
                "%d axes\n"
                "%d buttons\n"
                "%d hats\n\n",
                name,
                num_axes,
                num_buttons,
                num_hats);
            updateRGBs();
            rgbs.DisableCart();
            // Keep reading the state of the joystick in a loop
            while (true) {
                /* poll for disconnects or bad things */
                SDL_Event event;
                if (SDL_PollEvent(&event)) {
                    if (event.type == SDL_QUIT) { break; }
                    if (event.jdevice.type == SDL_JOYDEVICEREMOVED) { break; }
                }
                readSharedMemory();

                //Update Current Limits
                if(lastCurrentLimits.DriveCurrentLimit != currentLimits.DriveCurrentLimit)
                {
                    drivetrain.SetCurrentLimit(currentLimits.DriveCurrentLimit);
                }

                if(lastCurrentLimits.LiftCurrentLimit != currentLimits.LiftCurrentLimit)
                {
                    liftTable.SetCurrentLimit(currentLimits.LiftCurrentLimit);
                }

                if(lastCurrentLimits.driveBrake != currentLimits.driveBrake)
                {
                    drivetrain.setNeutralMode(currentLimits.driveBrake?NeutralMode::Brake:NeutralMode::Coast);
                }

                if(lastCurrentLimits.liftBrake != currentLimits.liftBrake)
                {
                    liftTable.SetNeutralMode((currentLimits.liftBrake?NeutralMode::Brake:NeutralMode::Coast));
                }


                updateRGBs();


                if(robotState.currentRobotState == RobotState::Enabled)
                {
                   ctre::phoenix::unmanaged::Unmanaged::FeedEnable(100);
                }
                else if(robotState.currentRobotState == RobotState::EmergencyStop)
                {
                 liftTable.Stop();
                 drivetrain.stop();
                }


                //debug lines for figuring out buttons
//                std::cout <<"Axes: " << SDL_JoystickGetAxis(joy, 0) << " " << SDL_JoystickGetAxis(joy, 1) << " " << SDL_JoystickGetAxis(joy, 2) << " " << SDL_JoystickGetAxis(joy, 3) << std::endl;
//                std::cout <<"Buttons: ";
//                for(int i = 0; i<16; i++)
//                {
//                    std::cout << (SDL_JoystickGetButton(joy, i)>0?"1":"0") << " ";
//                }
//                std::cout << std::endl;
                //end debug lines

                /* grab some stick values */
                double y = ((double)SDL_JoystickGetAxis(joy, 1)) / -32767.0;
                double turn = ((double)SDL_JoystickGetAxis(joy, 0)) / 32767.0;
                bool quickTurn = SDL_JoystickGetButton(joy, 1) > 0;
                bool driveEnable = SDL_JoystickGetButton(joy, 0) > 0;
                bool liftUp = SDL_JoystickGetButton(joy, 6) > 0;
                bool liftDown = SDL_JoystickGetButton(joy, 7) > 0;

                y = handleDeadZone(y);
                turn = handleDeadZone(turn);

                if(driveEnable && robotState.driveEnable)
                {
                    drivetrain.CheesyDrive(y, turn, quickTurn);
                }
                else
                {
                    drivetrain.stop();
                }

                if(liftUp && robotState.liftEnable)
                {
                    liftTable.RunLift(1.0);
                }
                else if (liftDown && robotState.liftEnable)
                {
                    liftTable.RunLift(-1.0);
                }
                else
                {
                    liftTable.Stop();
                }

                //Update Telem
                telemetry.totalPowerWatts = 0.0;
                drivetrain.updateTelemetry(telemetry);
                liftTable.UpdateTelemetry(telemetry);

                writeSharedMemory();
                /* loop yield for a bit */
                sleepApp(5);
            }
            /* we've left the loop, likely due to gamepad disconnect */
            drivetrain.stop();
            SDL_JoystickClose(joy);
            printf("gamepad disconnected\n");
        }

        SDL_Quit();

    return;
}

void PhoenixThread::sleepApp(int ms)
{
    std::this_thread::sleep_for(std::chrono::milliseconds(ms));
}

void PhoenixThread::readSharedMemory()
{
    lastRobotState = robotState;
    sharedRobotState.lock();
    std::memcpy(&robotState, sharedRobotState.data(), sizeof(robotState));
    sharedRobotState.unlock();

    lastCurrentLimits = currentLimits;
    sharedCurrentLimits.lock();
    std::memcpy(&currentLimits, sharedCurrentLimits.data(), sizeof(currentLimits));
    sharedCurrentLimits.unlock();

    lastRGBControl = rgbControl;
    sharedRGBControl.lock();
    std::memcpy(&rgbControl, sharedRGBControl.data(), sizeof(rgbControl));
    sharedRGBControl.unlock();



}

void PhoenixThread::writeSharedMemory()
{
    sharedTelemetry.lock();
    std::memcpy(sharedTelemetry.data(), &telemetry, sizeof(telemetry));
    sharedTelemetry.unlock();
}

double PhoenixThread::handleDeadZone(double input)
{
    const double deadZone = 0.1;
    const double minPower = 0.05;

    if(std::abs(input) < 0.1)
    {
        return 0.0;
    }

    double outputPower = ((std::abs(input) - deadZone) * (1.0 / (1.0 - deadZone))) * (1.0 - minPower) + minPower;
    if(input < 0)
    {
        return -outputPower;
    }
    else
    {
        return outputPower;
    }
}

bool PhoenixThread::AreRGBControlStructsEqual(RGBControlStruct one, RGBControlStruct two)
{
    return one.animationType == two.animationType
            && one.red == two.red
            && one.green == two.green
            && one.blue == two.blue
            && one.white == two.white
            && one.twinklePercent == two.twinklePercent
            && one.brightness == two.brightness
            && one.speed == two.speed
            && one.sparking == two.sparking
            && one.cooling == two.cooling
            && (one.override == two.override);
}

void PhoenixThread::updateRGBs()
{
    //update RGB
    if(!AreRGBControlStructsEqual(lastRGBControl, rgbControl))
    {
        rgbs.StartAnimationFromControlStruct(rgbControl, robotState.currentRobotState == RobotState::Enabled);
    }
    if(lastRobotState.currentRobotState != robotState.currentRobotState)
    {
        if(robotState.currentRobotState == RobotState::Enabled)
        {
            rgbs.EnableCart();
        }
        else if(robotState.currentRobotState == RobotState::Disabled)
        {
            rgbs.DisableCart();
        }
        else if (robotState.currentRobotState == RobotState::EmergencyStop)
        {
            rgbs.EStopCart();
        }
    }
}
