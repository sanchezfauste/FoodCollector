TARGET=foodcollection
CFLAGS = -Wall -lglut -lGLU -lGL -lm

all : $(TARGET)

$(TARGET) : main.cpp map.cpp map.h mapgenerator.cpp mapgenerator.h graphic.cpp graphic.h particle.cpp particle.h enemy_strategy.cpp enemy_strategy.h tank_particle.cpp tank_particle.h
	g++ graphic.cpp main.cpp map.cpp mapgenerator.cpp particle.cpp enemy_strategy.cpp tank_particle.cpp -o $(TARGET) $(CFLAGS)

clean:
	rm $(TARGET)
