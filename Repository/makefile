CC = g++
CFLAGS = -O2 -std=c++0x 
all: librepository.a
clean: 
	rm *.o
SRC=$(wildcard *.cpp)
OBJS=$(patsubst %.cpp, %.o, $(SRC))

librepository.a: $(OBJS)
	ar rcs $@ $^
%.o: %.cpp *.h
	$(CC) -c $(CFLAGS) $<
