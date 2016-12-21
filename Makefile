TARGET=foodcollection
CFLAGS = -Wall -lglut -lGLU -lGL -lm -ljpeg

all : $(TARGET)

$(TARGET) : main.cpp map.cpp map.h mapgenerator.cpp mapgenerator.h graphic.cpp graphic.h particle.cpp particle.h expectimax_strategy.cpp expectimax_strategy.h tank_particle.cpp tank_particle.h texture_loader.cpp texture_loader.h strategy.cpp strategy.h
	g++ graphic.cpp main.cpp map.cpp mapgenerator.cpp particle.cpp expectimax_strategy.cpp tank_particle.cpp texture_loader.cpp strategy.cpp -o $(TARGET) $(CFLAGS)

clean:
	rm $(TARGET)
