TARGET=foodcollection
ARDUINO_TARGET=foodcollection_arduino
CFLAGS = -Wall -lglut -lGLU -lGL -lm -ljpeg

all : $(TARGET) $(ARDUINO_TARGET)

arduino : $(ARDUINO_TARGET)

$(TARGET) : main.cpp map.cpp map.h mapgenerator.cpp mapgenerator.h \
		graphic.cpp graphic.h particle.cpp particle.h expectimax_strategy.cpp \
		expectimax_strategy.h tank_particle.cpp tank_particle.h \
		texture_loader.cpp texture_loader.h strategy.cpp strategy.h
	g++ graphic.cpp main.cpp map.cpp mapgenerator.cpp particle.cpp \
		expectimax_strategy.cpp tank_particle.cpp texture_loader.cpp \
		strategy.cpp -o $(TARGET) $(CFLAGS)

$(ARDUINO_TARGET) : main.cpp map.cpp map.h mapgenerator.cpp mapgenerator.h \
		graphic.cpp graphic.h particle.cpp particle.h expectimax_strategy.cpp \
		expectimax_strategy.h tank_particle.cpp tank_particle.h \
		texture_loader.cpp texture_loader.h strategy.cpp strategy.h \
		arduino/arduino-serial-lib.h arduino/arduino-serial-lib.c \
		arduino/arduino.h arduino/arduino.cpp
	g++ graphic.cpp main.cpp map.cpp mapgenerator.cpp particle.cpp \
		expectimax_strategy.cpp tank_particle.cpp texture_loader.cpp \
		strategy.cpp arduino/arduino-serial-lib.c arduino/arduino.cpp \
		-DARDUINO -o $(ARDUINO_TARGET) $(CFLAGS)

clean:
	rm $(TARGET) $(ARDUINO_TARGET)
