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
    TServo(uint8_t store, uint8_t ix);
    void UpdatePWM();
    void SetPower(bool on) { isOn = on; }
    void SetPosition(uint16_t pos) { isAt = pos; }
    uint16_t GetPosition() { return isAt; }
    void SetTrim(uint16_t a, uint16_t b, uint16_t c) { mTrimN45 = a; mTrim0 = b; mTrim45 = c; }
    void LoadTrim();
    void SaveTrim();
    uint16_t GetTrimN45() { return mTrimN45; }
    uint16_t GetTrim0() { return mTrim0; }
    uint16_t GetTrim45() { return mTrim45; }
protected:
    uint8_t     mStore = 0;
    uint8_t     pwmIx = 255;
    bool        isOn = false;
    bool        wasOn = true;
    uint16_t    isAt = 1500;
    uint16_t    wasAt = 1499;
    uint16_t    mTrimN45 = 1000;
    uint16_t    mTrim0 = 1500;
    uint16_t    mTrim45 = 2000;

    void setServoUSec(uint8_t servo, uint32_t usec);
    void setServoPulse(uint8_t n, double pulse);
};

extern TServo Servo[];
extern int NServo;



#endif /* T_SERVO_H */
