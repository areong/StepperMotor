#include "StepperMotor.h"
#include "AccelStepper.h"   // For StepperMotor
#include "TimerManager.h"   // For StepperMotor
#include "List.h"           // For StepperMotor

void setup() {
    int pinStep = 20;
    int pinDirection = 21;
    int stepsPerRevolution = 200;
    // Create a StepperMotor by "new".
    StepperMotor *motor = new StepperMotor(pinStep, pinDirection, stepsPerRevolution);

    // Set the first timing resolution to 1 ms,
    // and use the first resolution.
    TimerManager::getInstance()->setResolution(0, 1000);
    motor->setTimingResolution(0);

    // Set speed at 10 steps per second, and run.
    motor->stopAndSetSpeed(1);
    motor->run();

    delay(10000);

    // Reverse direction and run.
    motor->stopAndReverseDirection();
    motor->run();
    
    delay(10000);
    
    // Set speed at 20 with negative direction,
    // and rotate 180 degrees along negative direction.
    // In normal cases, positive and negative direction correspond to 
    // rotating clockwise and counterclockwise respectively.
    motor->stopAndSetSpeed(-2);
    motor->rotate(180); // Only accept non-negative value.
}

void loop() {

}