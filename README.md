# FoodCollector
Implementation of Computer Graphics and Multimedia project.

# Preparing the environment
In order to compile and run this project you need to install some dependences on your machine. Let's see how to prepare a Debian based distribution.

## Install the OpenGL library
You can install the library runing the following commands as `root`:
```
apt-get update
apt-get install freeglut3-dev
```

## Install the JPEG library
You can download this library from: http://www.ijg.org/

Once you downloaded the library extract it, enter to the `jpeg-9b` directory and run the following commands as `root`:
```
./configure
make
make install
ranlib /usr/local/lib/libjpeg.a
ldconfig /usr/local/lib
```

# Compile and execute the game
You can compile the game using the Makefile. You only need to enter to the root directory of the project and run:
```
make
```

To run the game execute the command:
```
./foodcollection <nRows> <nCols>
```
Notice that `nRows` is the number of rows of the map and `nCols` is the number of cols of the map.

# Arduino setup
In order to run the game using the Arduino game controller you need to install the Arduino IDE and the following libraries:

- Adafruit Sensor: https://github.com/adafruit/Adafruit_Sensor
- Adafruit ADXL345: https://github.com/adafruit/Adafruit_ADXL345
- DHT sensor library: https://github.com/adafruit/DHT-sensor-library

The next step is to connect all the sensors to the Arduino board following the next specification:

## Arduino connections
### DHT
* `Signal` --> `Digital 3`
* `-` --> `GND`
* `+` --> `5V`

### ADXL345
* `SDA` -> `A4`
* `SCL` -> `A5`
* `GND` -> `GND`
* `VCC` -> `5V`

### Joystick
* `SEL` -> `Digital 2`
* `VERT` -> `A1`
* `HORZ` -> `A0`
* `GND` -> `GND`
* `VCC` -> `5V`

### Ultrasound
* `Trig` -> `Digital 4`
* `Echo` -> `Digital 5`
* `GND` -> `GND`
* `VCC` -> `5V`

### Heart Rate
* `Black` -> `GND`
* `Red` -> `5V`
* `Purple` -> `A2`

Once you have prepared the Arduino environment you can flash the arduino sketch available on the `arduino_sketch` folder of the project.
