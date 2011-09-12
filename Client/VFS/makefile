CC=g++
CFLAGS=-c -w -D_SERVER -DLINUX -I../../Shared -I. -I../Shared -I../Vendor/fat-io/ -I../Shared/vfs

SOURCES=$(wildcard *.cpp)
OBJECTS=$(SOURCES:.cpp=.o)
EXECUTABLE=../Binary/VFSModule.so

all: $(SOURCES) $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS) 
	g++ $(OBJECTS) -lpthread -ldl -shared ../Vendor/fat-io/fat-io.a -o $@ 

.cpp.o:
	$(CC) $(CFLAGS) $< -o $@

clean:
	rm -Rf *.o $(EXECUTABLE)
