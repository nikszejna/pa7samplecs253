
CC=g++
CFLAGS=-Wall -std=c++11 -O2
LINK=-lgtest
DEPS=ImageHandler.h Image.h Morpher.h Mapper.h Coordinate.h KeyPoint.h
OBJS=ImageHandler.o Image.o Morpher.o Mapper.o KeyPoint.o main.o 
all: | pa7

clean:
	-rm -f pa7

%.o: %.cpp $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS) $(LINK)

pa7: $(OBJS)
	$(CC) -o $@ $^ $(CFLAGS) $(LINK)

.PHONY: all clean
