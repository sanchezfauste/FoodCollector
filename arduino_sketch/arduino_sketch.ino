#include "types.h"

/********************** Accelerometer **********************/
// Pinout
// Gnd      -  GND
// 5v     -  VCC
// Analog 4 -  SDA
// Analog 5 -  SLC
 
#define G_2  0x00
#define G_4  0x01
#define G_8  0x02 
#define G_16 0x03

//Devide Address 
#define DEVICE_ADDRESS 0x53  

//Registers Address - ADXL345
#define POWER_CTL 0x2D
#define DATA_FORMAT  0x31
#define DATAX0  0x32  //X-Axis Data 0
#define DATAX1  0x33 //X-Axis Data 1
#define DATAY0  0x34 //Y-Axis Data 0
#define DATAY1  0x35 //Y-Axis Data 1
#define DATAZ0  0x36 //Z-Axis Data 0
#define DATAZ1  0x37 //Z-Axis Data 1

#include <Wire.h>

Action getAccelerometerAction() {
  AccelerometerReading reading = readAccel();
  if (reading.y > 100) return UP;
  if (reading.y < -100) return DOWN;
  if (reading.x > 100) return RIGHT;
  if (reading.x < -100) return LEFT;
  return QUIET;
}

long microsecondsToCentimeters(long microseconds)
{
  // The speed of sound is 340 m/s or 29 microseconds per centimeter.
  // The ping travels out and back, so to find the distance of the
  // object we take half of the distance travelled.
  return microseconds / 29 / 2;
}

//Funcion auxiliar de escritura
void writeTo(int device, byte address, byte val) {
  Wire.beginTransmission(device);
  Wire.write(address);
  Wire.write(val);
  Wire.endTransmission(); 
}
 
//Funcion auxiliar de lectura
void readFrom(int device, byte address, int num, byte _buff[]) {
  Wire.beginTransmission(device);
  Wire.write(address);
  Wire.endTransmission();
 
  Wire.beginTransmission(device);
  Wire.requestFrom(device, num);
 
  int i = 0;
  while(Wire.available())
  { 
    _buff[i] = Wire.read();
    i++;
  }
  Wire.endTransmission();
}

AccelerometerReading readAccel() {
  AccelerometerReading reading;
  uint8_t numBytesToRead = 6;
  byte _buff[6];

  //Reading registers
  readFrom(DEVICE_ADDRESS, DATAX0, numBytesToRead, _buff);
 
  //each axis reading comes in 10 bit resolution, ie 2 bytes
  //Least Significant byte first!!
  //thus we are converting both bytes into one int
  reading.x = (((int)_buff[1]) << 8) | _buff[0];   
  reading.y = (((int)_buff[3]) << 8) | _buff[2];
  reading.z = (((int)_buff[5]) << 8) | _buff[4];
  return reading;
}

/********************** Joystick ***************************/
#define JOYSTICK_X_PIN A0
#define JOYSTICK_Y_PIN A1
#define BUTTON_PIN 2

JoystickReading readJoystick() {
  JoystickReading reading;
  reading.x = analogRead(JOYSTICK_X_PIN);
  reading.y = analogRead(JOYSTICK_Y_PIN);
  reading.buttonState = digitalRead(BUTTON_PIN) ? OFF : ON;
  return reading;
}

Action getJoystickAction() {
  JoystickReading reading = readJoystick();
  if (reading.y < 150) return UP;
  if (reading.y > 900) return DOWN;
  if (reading.x < 150) return LEFT;
  if (reading.x > 900) return RIGHT;
  return QUIET;
}

SwitchStatus getJoystickSwitchStatus() {
  JoystickReading reading = readJoystick();
  return reading.buttonState;
}

/********************** Temperature ************************/
#include <Adafruit_Sensor.h>

// Example testing sketch for various DHT humidity/temperature sensors
// Written by ladyada, public domain

#include "DHT.h"

#define DHTPIN 3     // what digital pin we're connected to

// Uncomment whatever type you're using!
#define DHTTYPE DHT11   // DHT 11
//#define DHTTYPE DHT22   // DHT 22  (AM2302), AM2321
//#define DHTTYPE DHT21   // DHT 21 (AM2301)

// Connect pin 1 (on the left) of the sensor to +5V
// NOTE: If using a board with 3.3V logic like an Arduino Due connect pin 1
// to 3.3V instead of 5V!
// Connect pin 2 of the sensor to whatever your DHTPIN is
// Connect pin 4 (on the right) of the sensor to GROUND
// Connect a 10K resistor from pin 2 (data) to pin 1 (power) of the sensor

// Initialize DHT sensor.
// Note that older versions of this library took an optional third parameter to
// tweak the timings for faster processors.  This parameter is no longer needed
// as the current DHT reading algorithm adjusts itself to work on faster procs.
DHT dht(DHTPIN, DHTTYPE);

DHTReading readDHT() {
  DHTReading reading;
  reading.humidity = dht.readHumidity();
  reading.celsius = dht.readTemperature();
  reading.fahrenheit = dht.readTemperature(true);
  return reading;
}

int getTemperatureInCelcius() {
  DHTReading reading = readDHT();
  return reading.celsius;
}

/********************** Ultrasound *************************/
/* Ping))) Sensor
  
   This sketch reads a PING))) ultrasonic rangefinder and returns the
   distance to the closest object in range. To do this, it sends a pulse
   to the sensor to initiate a reading, then listens for a pulse 
   to return.  The length of the returning pulse is proportional to 
   the distance of the object from the sensor.
     
   The circuit:
	* +V connection of the PING))) attached to +5V
	* GND connection of the PING))) attached to ground
	* SIG connection of the PING))) attached to digital pin 7

   http://www.arduino.cc/en/Tutorial/Ping
   
   created 3 Nov 2008
   by David A. Mellis
   modified 30 Aug 2011
   by Tom Igoe
 
   This example code is in the public domain.

 */

// this constant won't change.  It's the pin number
// of the sensor's output:
#define ultrasoundPingPin 4
#define ultrasoundPingEcho 5

UltrasoundReading readUltrasound() {
  UltrasoundReading reading;
  // establish variables for duration of the ping, 
  // and the distance result in inches and centimeters:
  long duration;

  // The PING))) is triggered by a HIGH pulse of 2 or more microseconds.
  // Give a short LOW pulse beforehand to ensure a clean HIGH pulse:
  digitalWrite(ultrasoundPingPin, LOW);
  delayMicroseconds(2);
  digitalWrite(ultrasoundPingPin, HIGH);
  delayMicroseconds(5);
  digitalWrite(ultrasoundPingPin, LOW);

  // The same pin is used to read the signal from the PING))): a HIGH
  // pulse whose duration is the time (in microseconds) from the sending
  // of the ping to the reception of its echo off of an object.
  duration = pulseIn(ultrasoundPingEcho, HIGH);

  // convert the time into a distance
  reading.inches = microsecondsToInches(duration);
  reading.cm = microsecondsToCentimetersUltrasound(duration);
  return reading;
}

long getDistanceInCm() {
  UltrasoundReading reading = readUltrasound();
  return reading.cm;
}

long microsecondsToInches(long microseconds)
{
  // According to Parallax's datasheet for the PING))), there are
  // 73.746 microseconds per inch (i.e. sound travels at 1130 feet per
  // second).  This gives the distance travelled by the ping, outbound
  // and return, so we divide by 2 to get the distance of the obstacle.
  // See: http://www.parallax.com/dl/docs/prod/acc/28015-PING-v1.3.pdf
  return microseconds / 74 / 2;
}

long microsecondsToCentimetersUltrasound(long microseconds)
{
  // The speed of sound is 340 m/s or 29 microseconds per centimeter.
  // The ping travels out and back, so to find the distance of the
  // object we take half of the distance travelled.
  return microseconds / 29 / 2;
}

/********************** Heart rate *************************/
/*
>> Pulse Sensor Amped 1.1 <<
This code is for Pulse Sensor Amped by Joel Murphy and Yury Gitman
    www.pulsesensor.com
    >>> Pulse Sensor purple wire goes to Analog Pin 0 <<<
Pulse Sensor sample aquisition and processing happens in the background via Timer 2 interrupt. 2mS sample rate.
PWM on pins 3 and 11 will not work when using this code, because we are using Timer 2!
The following variables are automatically updated:
Signal :    int that holds the analog signal data straight from the sensor. updated every 2mS.
IBI  :      int that holds the time interval between beats. 2mS resolution.
BPM  :      int that holds the heart rate value, derived every beat, from averaging previous 10 IBI values.
QS  :       boolean that is made true whenever Pulse is found and BPM is updated. User must reset.
Pulse :     boolean that is true when a heartbeat is sensed then false in time with pin13 LED going out.

This code is designed with output serial data to Processing sketch "PulseSensorAmped_Processing-xx"
The Processing sketch is a simple data visualizer.
All the work to find the heartbeat and determine the heartrate happens in the code below.
Pin 13 LED will blink with heartbeat.
If you want to use pin 13 for something else, adjust the interrupt handler
It will also fade an LED on pin HEART_RATE_FADE_PIN with every beat. Put an LED and series resistor from HEART_RATE_FADE_PIN to GND.
Check here for detailed code walkthrough:
http://pulsesensor.myshopify.com/pages/pulse-sensor-amped-arduino-v1dot1

Code Version 02 by Joel Murphy & Yury Gitman  Fall 2012
This update changes the HRV variable name to IBI, which stands for Inter-Beat Interval, for clarity.
Switched the interrupt to Timer2.  500Hz sample rate, 2mS resolution IBI value.
Fade LED pin moved to pin 5 (use of Timer2 disables PWM on pins 3 & 11).
Tidied up inefficiencies since the last version.
*/

//  VARIABLES
#define HEART_RATE_PULSE_PIN 2    // Pulse Sensor purple wire connected to analog pin

// these variables are volatile because they are used during the interrupt service routine!
volatile int BPM;                   // used to hold the pulse rate
volatile int Signal;                // holds the incoming raw data
volatile int IBI = 600;             // holds the time between beats, the Inter-Beat Interval
volatile boolean Pulse = false;     // true when pulse wave is high, false when it's low
volatile boolean QS = false;        // becomes true when Arduoino finds a beat.

volatile int rate[10];                    // used to hold last ten IBI values
volatile unsigned long sampleCounter = 0; // used to determine pulse timing
volatile unsigned long lastBeatTime = 0;  // used to find the inter beat interval
volatile int P = 512;                     // used to find peak in pulse wave
volatile int T = 512;                     // used to find trough in pulse wave
volatile int thresh = 512;                // used to find instant moment of heart beat
volatile int amp = 100;                   // used to hold amplitude of pulse waveform
volatile boolean firstBeat = true;        // used to seed rate array so we startup with reasonable BPM
volatile boolean secondBeat = true;       // used to seed rate array so we startup with reasonable BPM

void interruptSetup() {
  // Initializes Timer2 to throw an interrupt every 2mS.
  TCCR2A = 0x02;     // DISABLE PWM ON DIGITAL PINS 3 AND 11, AND GO INTO CTC MODE
  TCCR2B = 0x06;     // DON'T FORCE COMPARE, 256 PRESCALER
  OCR2A = 0X7C;      // SET THE TOP OF THE COUNT TO 124 FOR 500Hz SAMPLE RATE
  TIMSK2 = 0x02;     // ENABLE INTERRUPT ON MATCH BETWEEN TIMER2 AND OCR2A
  sei();             // MAKE SURE GLOBAL INTERRUPTS ARE ENABLED
}

// THIS IS THE TIMER 2 INTERRUPT SERVICE ROUTINE.
// Timer 2 makes sure that we take a reading every 2 miliseconds
ISR(TIMER2_COMPA_vect) {                      // triggered when Timer2 counts to 124
  cli();                                      // disable interrupts while we do this
  Signal = analogRead(HEART_RATE_PULSE_PIN);  // read the Pulse Sensor
  sampleCounter += 2;                         // keep track of the time in mS with this variable
  int N = sampleCounter - lastBeatTime;       // monitor the time since the last beat to avoid noise

//  find the peak and trough of the pulse wave
  if(Signal < thresh && N > (IBI/5)*3) {  // avoid dichrotic noise by waiting 3/5 of last IBI
    if (Signal < T){                      // T is the trough
      T = Signal;                         // keep track of lowest point in pulse wave
    }
  }

  if(Signal > thresh && Signal > P) {      // thresh condition helps avoid noise
    P = Signal;                            // P is the peak
  }                                        // keep track of highest point in pulse wave

  //  NOW IT'S TIME TO LOOK FOR THE HEART BEAT
  // signal surges up in value every time there is a pulse
  if (N > 250) {                                   // avoid high frequency noise
    if ( (Signal > thresh) && (Pulse == false) && (N > (IBI/5)*3) ) {
      Pulse = true;                               // set the Pulse flag when we think there is a pulse
      IBI = sampleCounter - lastBeatTime;         // measure time between beats in mS
      lastBeatTime = sampleCounter;               // keep track of time for next pulse

      if(firstBeat) {                      // if it's the first time we found a beat, if firstBeat == TRUE
        firstBeat = false;                 // clear firstBeat flag
        return;                            // IBI value is unreliable so discard it
      }
      if(secondBeat) {                    // if this is the second beat, if secondBeat == TRUE
        secondBeat = false;               // clear secondBeat flag
        for(int i=0; i<=9; i++) {         // seed the running total to get a realisitic BPM at startup
          rate[i] = IBI;
        }
      }

      // keep a running total of the last 10 IBI values
      word runningTotal = 0;                   // clear the runningTotal variable

      for(int i=0; i<=8; i++) {           // shift data in the rate array
        rate[i] = rate[i+1];              // and drop the oldest IBI value
        runningTotal += rate[i];          // add up the 9 oldest IBI values
      }

      rate[9] = IBI;                          // add the latest IBI to the rate array
      runningTotal += rate[9];                // add the latest IBI to runningTotal
      runningTotal /= 10;                     // average the last 10 IBI values
      BPM = 60000/runningTotal;               // how many beats can fit into a minute? that's BPM!
      QS = true;                              // set Quantified Self flag
      // QS FLAG IS NOT CLEARED INSIDE THIS ISR
    }
  }

  if (Signal < thresh && Pulse == true) {     // when the values are going down, the beat is over
    Pulse = false;                         // reset the Pulse flag so we can do it again
    amp = P - T;                           // get amplitude of the pulse wave
    thresh = amp/2 + T;                    // set thresh at 50% of the amplitude
    P = thresh;                            // reset these for next time
    T = thresh;
  }

  if (N > 2500) {                             // if 2.5 seconds go by without a beat
    thresh = 512;                          // set thresh default
    P = 512;                               // set P default
    T = 512;                               // set T default
    lastBeatTime = sampleCounter;          // bring the lastBeatTime up to date
    firstBeat = true;                      // set these to avoid noise
    secondBeat = true;                     // when we get the heartbeat back
  }

  sei();                                     // enable interrupts when youre done!
}// end isr

int getHeartRate() {
  if (QS == true) {
    QS = false;                       // reset the Quantified Self flag for next time
    return BPM;
  } else {
    return -1;
  }
}

/********************** Main program *************************/
void setup() {
  Serial.begin(9600);

  /**** Accelerometer ****/
  Wire.begin();
  writeTo(DEVICE_ADDRESS, DATA_FORMAT, G_2);  //ADXL345 Range +- 2G
  writeTo(DEVICE_ADDRESS, POWER_CTL, 0x00);   //Reset  - Power Control
  writeTo(DEVICE_ADDRESS, POWER_CTL, 0x10);   //ADXL in Standby mode
  writeTo(DEVICE_ADDRESS, POWER_CTL, 0x08);   //ADXL in Measure mode

  /**** Joystick ****/
  pinMode(JOYSTICK_X_PIN, INPUT);
  pinMode(JOYSTICK_Y_PIN, INPUT);
  pinMode(BUTTON_PIN, INPUT_PULLUP);

  /**** Temperature ****/
  dht.begin();

  /**** Ultrasound ****/
  pinMode(ultrasoundPingPin, OUTPUT);
  pinMode(ultrasoundPingEcho, INPUT);

  /**** Heart rate ****/
  interruptSetup();
}

void loop() {
  readAccel();
  Serial.print(getAccelerometerAction());Serial.print(";");
  Serial.print(getJoystickAction());Serial.print(";");
  Serial.print(getJoystickSwitchStatus());Serial.print(";");
  Serial.print(getTemperatureInCelcius());Serial.print(";");
  //Serial.print(getDistanceInCm());Serial.print(";");
  Serial.print(getHeartRate());Serial.print(";");
  Serial.print("\n");
  delay(65);
}

