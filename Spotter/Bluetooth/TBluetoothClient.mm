/*
	File: ChatClientController.m
	Constains: UI for Bluetooth sample [not to be used as UI sample code]
	Author: Marco Pontil

	Copyright (c) 2002 by Apple Computer, Inc., all rights reserved.
*/

#import "TBluetoothClient.h"
#import "ChatClientController.h"

#include <cstdarg>


TBluetoothClient::TBluetoothClient()
{
    mChatClient = [[ChatClientController alloc] init];
}


TBluetoothClient::~TBluetoothClient()
{
    mChatClient = nil;
}


void TBluetoothClient::OnDisconnect(std::function<void()> inOnDisconnect)
{
    [mChatClient setOnDisconnect:inOnDisconnect];
}


void TBluetoothClient::OnNewData(std::function<void(uint8_t *data, size_t size)> inOnNewData)
{
    [mChatClient setOnNewData:inOnNewData];
}


int TBluetoothClient::Connect()
{
    int ret = [mChatClient cppActionOnConnect];
    return ret;
}


void TBluetoothClient::Disconnect()
{
    [mChatClient cppActionOnDisconnect];
}


void TBluetoothClient::Send(const char *txt)
{
    [mChatClient cppActionSend:txt];
}


void TBluetoothClient::SendF(const char *fmt, ...)
{
    va_list args;
    va_start(args, fmt);

    char *buf;
    vasprintf(&buf, fmt, args);
    Send(buf);
    ::free(buf);

    va_end(args);
}


int TBluetoothClient::Connect(const char *inAddress)
{
    int ret = [mChatClient cppActionOnConnect:inAddress];
    return ret;
}


char *TBluetoothClient::GetAddress()
{
    NSString *dAddr = [mChatClient getDeviceAddress];
    if (dAddr) {
        char *str = strdup( [dAddr UTF8String] );
        return str;
    } else {
        return nullptr;
    }
}



