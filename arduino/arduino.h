/*
Copyright (C) 2017 Meritxell Jordana
Copyright (C) 2017 Marc Sanchez
*/

#ifndef __ARDUINO_H
#define __ARDUINO_H

#include "../map.h"

using namespace std;

typedef struct ArduinoInfo {
    bool readOk;
    Direction acceleromerAction;
    Direction joystickAction;
    bool joystickSwitchStatus;
    int temperatureInCelcius;
    int distanceInCm;
    int heartRate;
    ArduinoInfo();
} ArduinoInfo;

typedef enum ReadStatus {
    ReadOk,
    ReadFail,
    EndOfStructure
} ReadStatus;

typedef struct IntReading {
    ReadStatus status;
    int value;
    IntReading(ReadStatus status, int value);
} IntReading;

class Arduino {

    static const char* const serialport;
    static const int bufferSize;
    static const int baudrate;
    static const char eolchar;
    static const char delimiterChar;
    static const int timeout;
    int fd;

    IntReading readInt();
    bool readArduinoInfoParameter(int *parameter);
    bool readArduinoInfoParameter(Direction *parameter);
    bool readArduinoInfoParameter(bool *parameter);

public:
    bool init();
    ArduinoInfo getArduinoInfo();

};

#endif