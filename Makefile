TARGET=foodcollection
ARDUINO_TARGET=foodcollection_arduino
CFLAGS = -Wall -lglut -lGLU -lGL -lm -ljpeg

all : $(TARGET) $(ARDUINO_TARGET)

arduino : $(ARDUINO_TARGET)

$(TARGET) : main.cpp map.cpp map.h mapgenerator.cpp mapgenerator.h \
		graphic.cpp graphic.h particle.cpp particle.h expectimax_strategy.cpp \
		expectimax_strategy.h tank_particle.cpp tank_particle.h \
		texture_loader.cpp texture_loader.h strategy.cpp strategy.h \
		approximate_q_learning.cpp approximate_q_learning.h \
		expectimax_strategy_player.cpp expectimax_strategy_player.h \
		reinforcement_agent.cpp reinforcement_agent.h
	g++ graphic.cpp main.cpp map.cpp mapgenerator.cpp particle.cpp \
		expectimax_strategy.cpp tank_particle.cpp texture_loader.cpp \
		strategy.cpp approximate_q_learning.cpp reinforcement_agent.cpp \
		expectimax_strategy_player.cpp \
		-o $(TARGET) $(CFLAGS)

$(ARDUINO_TARGET) : main.cpp map.cpp map.h mapgenerator.cpp mapgenerator.h \
		graphic.cpp graphic.h particle.cpp particle.h expectimax_strategy.cpp \
		expectimax_strategy.h tank_particle.cpp tank_particle.h \
		texture_loader.cpp texture_loader.h strategy.cpp strategy.h \
		arduino/arduino-serial-lib.h arduino/arduino-serial-lib.c \
		arduino/arduino.h arduino/arduino.cpp approximate_q_learning.cpp \
		expectimax_strategy_player.cpp expectimax_strategy_player.h \
		approximate_q_learning.h reinforcement_agent.cpp reinforcement_agent.h
	g++ graphic.cpp main.cpp map.cpp mapgenerator.cpp particle.cpp \
		expectimax_strategy.cpp tank_particle.cpp texture_loader.cpp \
		strategy.cpp arduino/arduino-serial-lib.c arduino/arduino.cpp \
		approximate_q_learning.cpp reinforcement_agent.cpp -DARDUINO \
		expectimax_strategy_player.cpp \
		-o $(ARDUINO_TARGET) $(CFLAGS)

clean:
	rm $(TARGET) $(ARDUINO_TARGET)
