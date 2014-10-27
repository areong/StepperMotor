#ifndef STEPPERMOTOR_STEPPERMOTOR_H_
#define STEPPERMOTOR_STEPPERMOTOR_H_

#include "TimerListener.h"

class AccelStepper;

class StepperMotor : public TimerListener {
public:
    StepperMotor(int STP, int DIR, int stepsPerRevolution);
    ~StepperMotor();
    void setTimingResolution(int ithRes) {this->ithRes = ithRes;}
    /*
    @param speed Unit is steps per second.
                 If negative, run in negative direction (usually counterclockwise).
                 If zero, do not run.
    */
    void stopAndSetSpeed(double speed);
    /*
    Reverse current direction setting.
    */
    void stopAndReverseDirection();
    /*
    Run without stopping.
    */
    void run();
    /*
    Rotate a fix amount of angle.
    If it is already running, override previous running and rotate the angle.
    @param angle Unit is degree.
                 It only accepts positive value. If you want it to rotate in
                 reverse direction, set a negative speed by stopAndSetSpeed().
                 If zero, do not run.
    */
    void rotate(double angle);
    void stop();
    /*
    Run one step if the motor is running.
    */
    void onTimerCall();
    /*
    Stop running when the Timer has stopped.
    */
    void onTimerStopped();
private:
    AccelStepper* motor;
    int stepsPerRevolution;
    double speed;
    double NUMTIMERCALLS_PER_STEP;   // Number of timer calls per step.
    double timerPeriod;
    int ithRes;
    /*
    Indicate whether the motor is running.
    Furthermore, if running is true, StepperMotor.timer exists,
    otherwise StepperMotor.timer is a null pointer.
    So running is true AFTER calling TimerManager.addTimerAndStart(),
    and is false BEFORE calling StepperMotor.stopTimer().
    */
    bool running;
};

#endif