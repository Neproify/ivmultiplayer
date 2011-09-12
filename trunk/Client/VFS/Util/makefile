CC=g++
CFLAGS=-c -D_SERVER -DLINUX -I. -I../../Shared -I../../Shared/vfs

SOURCES=$(wildcard *.cpp)
SOURCES+=$(wildcard ../../Shared/*.cpp)
SOURCES+=$(wildcard ../../Shared/vfs/*.cpp)

OBJECTS=$(SOURCES:.cpp=.o)
EXECUTABLE=../../Binary/VFSModule.Util

all: $(SOURCES) $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS) 
	g++ $(OBJECTS) -lpthread -ldl -lz -o $@ 

.cpp.o:
	$(CC) $(CFLAGS) $< -o $@

clean:
	rm -Rf *.o $(EXECUTABLE)
