/*
Copyright (C) 2016 Meritxell Jordana
Copyright (C) 2016 Marc Sanchez
*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include "arduino-serial-lib.h"
#include "arduino.h"

using namespace std;

const char* const Arduino::serialport = "/dev/ttyACM0";
const int Arduino::bufferSize = 32;
const int Arduino::baudrate = 9600;
const char Arduino::eolchar = '\n';
const char Arduino::delimiterChar = ';';
const int Arduino::timeout = 10;

IntReading::IntReading(ReadStatus status, int value) :
    status(status), value(value) {}

ArduinoInfo::ArduinoInfo() : readOk(false) {}

bool Arduino::init() {
    fd = serialport_init(Arduino::serialport, Arduino::baudrate);
    if( fd == -1 ) {
        printf("couldn't open port %s\n", Arduino::serialport);
        return false;
    }
    printf("opened port %s\n", serialport);
    serialport_flush(fd);
    return true;
}

ArduinoInfo Arduino::getArduinoInfo() {
    ArduinoInfo ainfo;
    if (!readArduinoInfoParameter(&ainfo.acceleromerAction)) return ainfo;
    if (!readArduinoInfoParameter(&ainfo.joystickAction)) return ainfo;
    if (!readArduinoInfoParameter(&ainfo.joystickSwitchStatus)) return ainfo;
    if (!readArduinoInfoParameter(&ainfo.temperatureInCelcius)) return ainfo;
    //if (!readArduinoInfoParameter(&ainfo.distanceInCm)) return ainfo;
    if (!readArduinoInfoParameter(&ainfo.heartRate)) return ainfo;
    ainfo.readOk = true;
    return ainfo;
}

bool Arduino::readArduinoInfoParameter(int *parameter) {
    IntReading reading = readInt();
    *parameter = reading.value;
    return reading.status == ReadOk;
}

bool Arduino::readArduinoInfoParameter(Direction *parameter) {
    IntReading reading = readInt();
    *parameter = (Direction) reading.value;
    return reading.status == ReadOk;
}

bool Arduino::readArduinoInfoParameter(bool *parameter) {
    IntReading reading = readInt();
    *parameter = reading.value == 0 ? false : true;
    return reading.status == ReadOk;
}

IntReading Arduino::readInt() {
    int n;
    int i = 0;
    int timeout = Arduino::timeout;
    char c;
    char buffer[Arduino::bufferSize];
    ReadStatus status = ReadFail;
    while ((n = read(fd, &c, 1)) != -1 && i < Arduino::bufferSize && timeout > 0) {
        if(n == 0) {
            usleep( 1 * 1000 );
            timeout -= 1;
            continue;
        }
        if (c == Arduino::delimiterChar) {
            status = ReadOk;
            break;
        } else if (c == Arduino::eolchar) {
            status = EndOfStructure;
            break;
        }
        buffer[i++] = c;
    }
    buffer[i] = '\0';
    return IntReading(status, atoi(buffer));
}
