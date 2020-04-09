
#include "TServo.h"

#include <Arduino.h>
#include <EEPROM.h>
// EEPROM.read(x) -> byte
// write(address, value)
// update(address, value) (only if different)
// put(address, vlaue) (implies sizeof(value))
//unsigned long eeprom_crc(void) {
//
//    const unsigned long crc_table[16] = {
//        0x00000000, 0x1db71064, 0x3b6e20c8, 0x26d930ac,
//        0x76dc4190, 0x6b6b51f4, 0x4db26158, 0x5005713c,
//        0xedb88320, 0xf00f9344, 0xd6d6a3e8, 0xcb61b38c,
//        0x9b64c2b0, 0x86d3d2d4, 0xa00ae278, 0xbdbdf21c
//    };
//
//    unsigned long crc = ~0L;
//
//    for (int index = 0 ; index < EEPROM.length()  ; ++index) {
//        crc = crc_table[(crc ^ EEPROM[index]) & 0x0f] ^ (crc >> 4);
//        crc = crc_table[(crc ^ (EEPROM[index] >> 4)) & 0x0f] ^ (crc >> 4);
//        crc = ~crc;
//    }
//    return crc;
//}
// servo angels in us: 1000=-45deg, 1500=0deg, 2000=+45deg

//var a = (x1*(y2-y3)+x2*(y3-y1)+x3*(y1-y2))/((x1-x2)*(x1-x3)*(x3-x2));
//var b = (x1*x1*(y2-y3)+x2*x2*(y3-y1)+x3*x3*(y1-y2))/((x1-x2)*(x1-x3)*(x2-x3));
//var c = (x1*x1*(x2*y3-x3*y2)+x1*(x3*x3*y2-x2*x2*y3)+x2*x3*y1*(x2-x3))/((x1-x2)*(x1-x3)*(x2-x3));


#define SERVO_FREQ (50)

TServo Servo[] {
    // the numbers correspond to the PWM output channel in hardware
    TServo( 0, 3), TServo( 1, 2), TServo( 2, 1),     // front right leg
    TServo( 3, 7), TServo( 4, 6), TServo( 5, 5),     // front left leg
    TServo( 6,11), TServo( 7,10), TServo( 8, 9),     // hind right leg
    TServo( 9,15), TServo(10,14), TServo(11,13),     // hind left leg
};

int NServo = sizeof(Servo) / sizeof(Servo[0]);


TServo::TServo(uint8_t inStore, uint8_t ix)
: mStore { inStore }
, pwmIx { ix }
{
    LoadTrim();
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

void TServo::LoadTrim()
{
    int offset = 4 + mStore*6;
    EEPROM.get(offset+0, mTrimN45);
    EEPROM.get(offset+2, mTrim0);
    EEPROM.get(offset+4, mTrim45);
}


void TServo::SaveTrim()
{
    int offset = 4 + mStore*6;
    EEPROM.put(offset+0, mTrimN45);
    EEPROM.put(offset+2, mTrim0);
    EEPROM.put(offset+4, mTrim45);
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
