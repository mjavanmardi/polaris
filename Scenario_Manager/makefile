CC = g++
CFLAGS = -O2 -std=c++0x 
all: libjson.a
clean: 
	rm *.o
SRC=$(wildcard *.cpp)
OBJS=$(patsubst %.cpp, %.o, $(SRC))

libjson.a: $(OBJS)
	ar rcs $@ $^
%.o: %.cpp
	$(CC) -c $(CFLAGS) $<
