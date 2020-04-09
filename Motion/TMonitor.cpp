//
//  TMonitor.cpp
//
//  Created by Matthias Melcher on 4/5/20.
//

#include "TMonitor.h"

#include "TServo.h"

#include <HardwareSerial.h>
 

// must be in ASCII-alphabetical order
TMonitorAction TMonitor::mAction[]
{
    { "C",      TMonitor::HandleActionServoDataRequest }, // servoID
    { "M",      TMonitor::HandleActionServoPower }, // servoID, on/off
    { "beep",   TMonitor::HandleActionBeep }, // nothing
    { "s",      TMonitor::HandleActionServoPositionRaw }, // servoID, rawPosition
    { "trim",   TMonitor::HandleActionServoTrim }, // servoID, u-45, u0, u45
};

int TMonitor::mNActionList = sizeof(mAction)/sizeof(mAction[0]);


/**
 Initialize the members of the Monitor class.
 */
void TMonitor::Setup()
{
    Serial1.begin(115200);
    Serial1.print("\nboot\n");
}

void TMonitor::HandleActionBeep(const char*)
{
    tone(45, 2000, 50);
    Serial.println("->beep");
}

void TMonitor::HandleActionServoDataRequest(const char *args)
{
    int m = args[0]-'a';
    TServo &s = Servo[m];
    Serial1.print("s:");
    Serial1.print((char)('a'+m));
    Serial1.print(s.GetPosition());
    Serial1.print(',');
    Serial1.print(s.GetTrimN45());
    Serial1.print(',');
    Serial1.print(s.GetTrim0());
    Serial1.print(',');
    Serial1.print(s.GetTrim45());
    Serial1.print('\n');
}


void TMonitor::HandleActionServoTrim(const char *args)
{
    uint8_t m = args[0]-'a';
    uint16_t a = 1000, b = 1500, c = 2000;
    const char *comma = args;
    a = atoi(comma+1);
    comma = strchr(comma+1, ',');
    if (comma) {
        b = atoi(comma+1);
        comma = strchr(comma+1, ',');
        if (comma) {
            c = atoi(comma+1);
        }
    }
    Servo[m].SetTrim(a, b, c);
    Servo[m].SaveTrim();
}


void TMonitor::HandleActionServoPositionRaw(const char *args)
{
    int m = args[0]-'a';
    int v = atoi(args+1);
    Servo[m].SetPosition(v);
}

void TMonitor::HandleActionServoPower(const char *args)
{
    int m = args[0]-'a';
    Servo[m].SetPower(args[1]=='1');
}

void TMonitor::HandleAction(char *cmd)
{
    // extract the command (either ends at the first ':' or the end of the line
    char *args =strchr(cmd, ':');
    if (args) { *args = 0; args++; }

    // Binary search through available commands
    int lower = 0;
    int upper = mNActionList - 1;
    while (lower <= upper) {
        int mid = lower + (upper - lower) / 2;
        int res = strcmp(cmd, mAction[mid].cmd);
        if (res == 0)
            return mAction[mid].actionHandler(args);
        if (res > 0)
            lower = mid + 1;
        else
            upper = mid - 1;
    }
    Serial.print("Unknown Action From Bot: [");
    Serial.print(cmd);
    Serial.print("]\n");
}


void TMonitor::OnDataIn()
{
    static char serBuf[64] = { 0 };
    static int serBufN = 0;
    static const int serBufMax = sizeof(serBuf)-1;

    //printf("Data: %.*s\n", (int)size, data);
    while (Serial1.available()) {
        int s = Serial1.read();
        //Serial.write(s);
        if (s=='\n') {
            serBuf[serBufN] = 0;
            HandleAction(serBuf);
            serBufN = 0;
        } else if (serBufN<serBufMax) {
            serBuf[serBufN++] = s;
        } else {
            // cmd buffer overflow
        }
    }
#if 0
    while (Serial1.available()) {
        int s = Serial1.read();
        //Serial.write(s);
        if (s=='\n') {
            serBuf[serBufN] = 0;
            //Serial.println(serBuf);
            if (serBuf[0]=='s') { // slider
                int m = serBuf[1]-'a';
                int v = atoi(serBuf+2);
                gServoList[m].SetPosition(v);
            } else if (serBuf[0]=='C') { // Set the current motor
                int m = serBuf[1]-'a';
                gCurrentServo = m;
                Serial1.print("s:");
                Serial1.print((char)('a'+m));
                Serial1.print(gServoList[m].GetPosition());
                Serial1.print('\n');
            } else if (serBuf[0]=='M') { // Motor Power (Mxy; x = aIndex, y=0 or 1)
                int m = serBuf[1]-'a';
                bool on = (serBuf[2]=='1');
                gServoList[m].SetPower(on);
            } else if (strcmp(serBuf, "beep")==0) { // activate nothing
                tone(45, 2000, 50);
                Serial.println("->beep");
            }
            serBufN = 0;
        } else if (serBufN<serBufMax) {
            serBuf[serBufN++] = s;
        } else {
            // cmd buffer overflow
        }
    }
#endif
}

