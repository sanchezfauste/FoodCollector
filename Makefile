TARGET=foodcollection
CFLAGS = -Wall

all : $(TARGET)

$(TARGET) : main.cpp map.cpp map.h mapgenerator.cpp mapgenerator.h
	g++ $(CFLAGS) main.cpp map.cpp mapgenerator.cpp -o $(TARGET)

clean:
	rm $(TARGET)
