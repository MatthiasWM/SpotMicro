
#include "TServo.h"

// servo angels in us: 1000=-45deg, 1500=0deg, 2000=+45deg
#include <Arduino.h>

#define SERVO_FREQ (50)

TServo Servo[] {
    // the numbers correspond to the PWM output channel in hardware
    TServo( 3), TServo( 2), TServo( 1),     // front right leg
    TServo( 7), TServo( 6), TServo( 5),     // front left leg
    TServo(11), TServo(10), TServo( 9),     // hind right leg
    TServo(15), TServo(14), TServo(13),     // hind left leg
};

int NServo = sizeof(Servo) / sizeof(Servo[0]);


TServo::TServo(uint8_t ix)
: pwmIx{ix}
{
}


void TServo::UpdatePWM()
{
    // send a command only if something changed
    bool mustUpdate = false;
    if (isOn!=wasOn) {
        wasOn = isOn;
        mustUpdate = true;
    }
    if (isOn) {
        if (isAt!=wasAt) {
            wasAt = isAt;
            mustUpdate = true;
        }
        if (mustUpdate) {
            setServoUSec(pwmIx, isAt);
            Serial.print("Servo set: ");
            Serial.print(pwmIx);
            Serial.print(", ");
            Serial.println(isAt);
        }
    } else {
        if (mustUpdate) {
            pwm.setPWM(pwmIx, 0, 0);
            Serial.print("Servo off: ");
            Serial.println(pwmIx);
        }
    }
}


// static members:

Adafruit_PWMServoDriver TServo::pwm;

void TServo::Setup()
{
    pwm = Adafruit_PWMServoDriver();
    pwm.begin();
    pwm.setPWMFreq(SERVO_FREQ);  // Analog servos run at ~50 Hz updates
    pwm.setPWM(0, 0, 2048); // just some constant output that we can read back in through A12/INT23 to verify that the PWM module is still running
    delay(10);
}

void TServo::setServoPulse(uint8_t n, double pulse)
{
    double pulselength;
    pulselength = 1000000;   // 1,000,000 us per second
    pulselength /= SERVO_FREQ;   // Analog servos run at ~60 Hz updates
    Serial.print(pulselength); Serial.println(" us per period");
    pulselength /= 4096;  // 12 bits of resolution
    Serial.print(pulselength); Serial.println(" us per bit");
    pulse *= 1000000;  // convert input seconds to us
    pulse /= pulselength;
    Serial.println(pulse);
    pwm.setPWM(n, 0, pulse);
}

void TServo::setServoUSec(uint8_t servo, uint32_t usec)
{
    pwm.writeMicroseconds(servo, usec);
}
