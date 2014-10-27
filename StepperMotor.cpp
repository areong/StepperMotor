#include "StepperMotor.h"
#include "AccelStepper.h"
#include "TimerManager.h"

StepperMotor::StepperMotor(int STP, int DIR, int stepsPerRevolution = 200) {
    motor = new AccelStepper(AccelStepper::DRIVER, STP, DIR);
    this->stepsPerRevolution = stepsPerRevolution;
    speed = 1;
    NUMTIMERCALLS_PER_STEP = 2;
    timerPeriod = 0.5;
    ithRes = 0;
    running = false;
}

StepperMotor::~StepperMotor() {
    delete motor;
}

void StepperMotor::stopAndSetSpeed(double speed) {
    stop();
    // If zero speed, do not set.
    if (speed == 0) return;

    this->speed = speed;
    double absSpeed = speed > 0 ? speed : speed * -1;
    // Max speed of AccelStepper is 1 by default, so it is necessary to overwrite
    // the max speed. Max speed should be a positive value.
    motor->setMaxSpeed(absSpeed * 2);  
    motor->setSpeed(speed); // If speed is negative, it runs in reverse direction.
    // Convert speed to timerPeriod.
    // Since at least one AccelStepper::runSpeed() should be called per one step,
    // let Timer run NUMTIMERCALLS_PER_STEP times faster, so set timerPeriod to
    // 1 / (speed * NUMTIMERCALLS_PER_STEP).
    timerPeriod = 1 / (absSpeed * NUMTIMERCALLS_PER_STEP);
}

void StepperMotor::stopAndReverseDirection() {
    stop();
    speed *= -1;
    motor->setSpeed(speed);
}

void StepperMotor::run() {
    // Stop first.
    stop();
    // If zero speed, do not run.
    if (speed == 0) return;
    // Start running.
    TimerManager::getInstance()->addTimerAndStart(ithRes, timerPeriod, -1, this);
    running = true; // Set after adding Timer.
}

void StepperMotor::rotate(double angle) {
    if (angle < 0) return;
    // Stop first.
    stop();
    // If zero speed, do not run.
    if (speed == 0)
        return;
    // Start rotating.
    // Since NUMTIMERCALLS_PER_STEP onTimerCall() per one step (see setSpeed()),
    // time numPeriods by NUMTIMERCALLS_PER_STEP at the end of this line.
    int numPeriods = (int)(angle / 360.0 * (double)stepsPerRevolution) * NUMTIMERCALLS_PER_STEP;
    TimerManager::getInstance()->addTimerAndStart(ithRes, timerPeriod, numPeriods, this);
    running = true; // Set after adding Timer.
    // "running" will be false after the Timer completed numPeriods.
}

void StepperMotor::stop() {
    if (running) {
        motor->stop();
        running = false;    // Call before stopping Timer.
        stopTimer();        
    }
    // If the motor is not running, Timer pointer is null and cannot call Timer.stop().
}

void StepperMotor::onTimerCall() {
    if (running)
        motor->runSpeed();  // Run one step.
}

void StepperMotor::onTimerStopped() {
    // Stop.
    // It won't fall into stop()/stopTimer()/onTimerStopped()/stop() loop,
    // because stop() sets "running" to false before calling stopTimer().
    // Moreover, Timer ignores the second call of Timer.stop()
    // so onTimerStopped() will not be called twice, and loop will not occur.
    if (running)
        stop(); // "running" has changed to false.
}