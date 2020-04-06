
#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>

extern Adafruit_PWMServoDriver pwm;

extern void setupPWM();
extern void setServoUSec(uint8_t servo, uint32_t usec);
extern void setServoPulse(uint8_t n, double pulse);
