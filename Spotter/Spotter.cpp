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

void ConnectBT(Fl_Button *, void *)
{
    if (gBluetooth->Connect()==0) {
        wDisconnect->activate();
        wConnect->deactivate();
    } else {
        wDisconnect->deactivate();
        wConnect->activate();
    }
}

void DisconnectBT(Fl_Button *, void *)
{
    gBluetooth->Disconnect();
    wDisconnect->deactivate();
    wConnect->activate();
}


void BeepCB(Fl_Button *, void *)
{
    gBluetooth->Send("beep;");
}

void SendServoPosCB(Fl_Value_Slider *w, void *)
{
    gBluetooth->SendF("s%d;", (int)w->value());
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
        printf("Data: %.*s\n", (int)size, data);
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

