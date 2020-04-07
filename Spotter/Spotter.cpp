//
//  Spotter.cpp
//  Spotter
//
//  Created by Matthias Melcher on 4/5/20.
//

#include "Spotter.h"
#include "SpotterUI.h"

#include "Bluetooth/TBluetoothClient.h"

#include <vector>
#include <string>
#include <stdio.h>
#include <unistd.h>
#include <sys/socket.h>

#include <FL/Fl_Button.H>


Fl_Window *gMainWindow = nullptr;
TBluetoothClient *gBluetooth;
int gCurrentServo = 0;

void ConnectBT(Fl_Button *, void *)
{
    if (gBluetooth->Connect()==0) {
        wDisconnect->activate();
        wConnect->deactivate();
        wConnectAgain->deactivate();
        printf("Connected to [%s]\n", gBluetooth->GetAddress());
    } else {
        wDisconnect->deactivate();
        wConnect->activate();
        wConnectAgain->activate();
    }
}

void ConnectSameBT(Fl_Button *, void *)
{
    if (gBluetooth->Connect("cc-50-e3-95-1e-1a")==0) {
        wDisconnect->activate();
        wConnect->deactivate();
        wConnectAgain->deactivate();
        printf("Connected to [%s]\n", gBluetooth->GetAddress());
    } else {
        wDisconnect->deactivate();
        wConnect->activate();
        wConnectAgain->activate();
    }
}

void DisconnectBT(Fl_Button *, void *)
{
    gBluetooth->Disconnect();
    wDisconnect->deactivate();
    wConnect->activate();
    wConnectAgain->activate();
}


void SendServoPosCB(Fl_Value_Slider *w, void *)
{
    gBluetooth->SendF("s%c%d\n", gCurrentServo+'a', (int)w->value());
}

void MotorOnCB(Fl_Check_Button *o, long ix)
{
    if (o->value()) {
        // switch power to this motor on
        gBluetooth->SendF("M%c1\n", ix+'a');
    } else {
        // switch power to this motor off
        gBluetooth->SendF("M%c0\n", ix+'a');
    }
}

/*
 Make the motor with the given index the current motor. This is the one motor
 that reacts to the calibration UI elements.
 Make sure that the UI stays in sync with what the robot does.
 Spot should reply with the position and state of that motor, so we can update
 the UI elements.
 */
void CurrentMotorCB(Fl_Round_Button *o, long ix)
{
    if (o->value()) {
        gCurrentServo = (int)ix;
        gBluetooth->SendF("C%c\n", ix+'a');
    }
}

void AllPowerCB(Fl_Button *o, void*)
{
    // cut all power NOW!
    gBluetooth->Send("!!!\n");
}

void DataFromBot(uint8_t *data, size_t size)
{
    static char serBuf[64] = { 0 };
    static int serBufN = 0;
    static const int serBufMax = sizeof(serBuf)-1;

    //printf("Data: %.*s\n", (int)size, data);
    int n = (int)size;
    for (int i=0; i<n; i++) {
        char s = (char)data[i];
        //Serial.write(s);
        if (s=='\n') {
            serBuf[serBufN] = 0;
            printf("DataFromBot: [%s]\n", serBuf);
            if (serBuf[0]=='s') { // set slider
                int pos = atoi(serBuf+2);
                wServoSlider->value(pos);
            } else if (strcmp(serBuf, "boot")==0) {
                for (int j=0; j<12; j++) {
                    wMotorOn[j]->value(0);
                }
            }
            serBufN = 0;
        } else if (serBufN<serBufMax) {
            serBuf[serBufN++] = s;
        } else {
            // cmd buffer overflow
        }
}
}

int main(int argc, char **argv)
{
    gBluetooth = new TBluetoothClient();
    gBluetooth->OnDisconnect(
                             []()->void
    {
        wDisconnect->deactivate();
        wConnect->activate();
    }
                             );
    gBluetooth->OnNewData(
    [](uint8_t *data, size_t size)->void
    {
        DataFromBot(data, size);
        ::free(data);
    }
                             );

    /*
    try {
    auto bt = BTSerialPortBinding::Create("CC-50-E3-95-1E-1A", 1);
    bt->Connect();
    for (;;) {
        char buf[33];
        int n = bt->Read(buf, 32);
//        BTSerialPortBinding::Write(buffer, length)
//        BTSerialPortBinding::IsDataAvailable(buffer, length)
        printf("%d: \n", n);
    }
    bt->Close();
    } catch(BluetoothException *ex) {
        puts(ex->what());
    }
*/

    gMainWindow = CreateMainWindow();
    wDisconnect->deactivate();
    gMainWindow->show(argc, argv);
    Fl::run();
}

