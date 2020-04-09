//
//  Spotter.h
//  Spotter
//
//  Created by Matthias Melcher on 4/5/20.
//

#ifndef T_MONITOR_H
#define T_MONITOR_H


#include <stdint.h>
#include <stddef.h>


struct TMonitorAction
{
    const char *cmd;
    void (*actionHandler)(const char *);
};


class TMonitor
{
public:
    TMonitor() = default;
    void Setup();
    void OnDataIn();
    
protected:
    static void HandleAction(char *);
    static void HandleActionServoDataRequest(const char *);
    static void HandleActionServoPositionRaw(const char *);
    static void HandleActionServoPower(const char *);
    static void HandleActionServoTrim(const char *);
    static void HandleActionBeep(const char *);

    static TMonitorAction mAction[];
    static int mNActionList;
};


#endif /* T_MONITOR_H */
