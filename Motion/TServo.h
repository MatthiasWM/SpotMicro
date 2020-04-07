//
//  Motion.hpp
//  Motion
//
//  Created by Matthias Melcher on 4/5/20.
//

#ifndef T_SERVO_H
#define T_SERVO_H

#include <stdint.h>
#include <stddef.h>

#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>


class TServo
{
public:
    static void Setup();
protected:
    static Adafruit_PWMServoDriver pwm;

public:
    TServo(uint8_t ix);
    void UpdatePWM();
    void SetPower(bool on) { isOn = on; }
    void SetPosition(int pos) { isAt = pos; }
    int GetPosition() { return isAt; }
protected:
    bool isOn = false;
    bool wasOn = true;
    int isAt = 1500;
    int wasAt = 1499;
    uint8_t pwmIx = 255;

    void setServoUSec(uint8_t servo, uint32_t usec);
    void setServoPulse(uint8_t n, double pulse);
};

extern TServo Servo[];
extern int NServo;



#endif /* T_SERVO_H */
