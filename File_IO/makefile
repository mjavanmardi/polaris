CC = g++
CFLAGS = -std=c++0x 
all: libfileio.a
clean: 
	rm *.o
SRC=$(wildcard *.cpp)
OBJS=$(patsubst %.cpp, %.o, $(SRC))

libfileio.a: $(OBJS)
	ar rcs $@ $^
%.o: %.cpp
	$(CC) -c $(CFLAGS) -D LINUX $<
