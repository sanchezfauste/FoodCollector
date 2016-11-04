TARGET=foodcollection
CFLAGS = -Wall -lglut -lGLU -lGL -lm

all : $(TARGET)

$(TARGET) : main.cpp map.cpp map.h mapgenerator.cpp mapgenerator.h graphic.cpp graphic.h particle.cpp particle.h
	g++ graphic.cpp main.cpp map.cpp mapgenerator.cpp particle.cpp -o $(TARGET) $(CFLAGS)

clean:
	rm $(TARGET)
