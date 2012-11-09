CC = g++
CFLAGS = -std=c++0x 
all: libjson.a
clean: 
	rm *.o
libjson.a: json_reader.o json_value.o json_writer.o cfg_reader.o
	ar rcs libjson.a json_reader.o json_value.o json_writer.o cfg_reader.o
json_reader.o: json_reader.cpp
	$(CC) -c $(CFLAGS) json_reader.cpp
json_value.o: json_value.cpp
	$(CC) -c $(CFLAGS) json_value.cpp
json_writer.o: json_writer.cpp
	$(CC) -c $(CFLAGS) json_writer.cpp
cfg_reader.o: cfg_reader.cpp
	$(CC) -c $(CFLAGS) cfg_reader.cpp
