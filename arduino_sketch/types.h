#ifndef _TYPES_H_
#define _TYPES_H_

typedef struct ArduinoInfo {
  int acceleromerAction;
  int joystickAction;
  int joystickSwitchStatus;
  int temperatureInCelcius;
  int distanceInCm;
  int heartRate;
} ArduinoInfo;

typedef enum Action {
  UP = 0,
  RIGHT = 1,
  DOWN = 2,
  LEFT = 3,
  QUIET = 4
} Action;

typedef enum SwitchStatus {
  OFF = 0,
  ON = 1
} SwitchStatus;

typedef struct AccelerometerReading {
  int x;
  int y;
  int z;
} AccelerometerReading;

typedef struct JoystickReading {
  int x;
  int y;
  SwitchStatus buttonState;
} JoystickReading;

typedef struct DHTReading {
  float humidity;
  float celsius;
  float fahrenheit;
} DHTReading;

typedef struct UltrasoundReading {
  long cm;
  long inches;
} UltrasoundReading;

#endif
