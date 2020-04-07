//
//  Spotter.cpp
//  Spotter
//
//  Created by Matthias Melcher on 4/5/20.
//

#include "Spotter.h"
#include "SpotterUI.h"

#include "Bluetooth/TBluetoothClient.h"
#include "Panels/TMonitor.h"
#include "Panels/TTrimPanel.h"

#include <vector>
#include <string>
#include <stdio.h>
#include <unistd.h>
#include <sys/socket.h>

#include <FL/Fl_Button.H>


TMonitor Monitor;


Fl_Window *gMainWindow = nullptr;
TBluetoothClient *gBluetooth;


int main(int argc, char **argv)
{
    Monitor.Setup();
    gMainWindow = CreateMainWindow();
    wDisconnect->deactivate();
    gMainWindow->show(argc, argv);
    Fl::run();
}

