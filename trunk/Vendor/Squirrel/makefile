CC=g++
CFLAGS=-m32 -c -Wall -D_SERVER -I. -fno-rtti -fno-strict-aliasing -I../../Shared -D_LINUX
LDFLAGS=-lpthread
SOURCES=$(wildcard *.cpp)
OBJECTS=$(SOURCES:.cpp=.o)
EXECUTABLE=libsquirrel.a

all: $(SOURCES) $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS) 
	ar -rv $@ $(OBJECTS) 

.cpp.o:
	$(CC) $(CFLAGS) $< -o $@

clean:
	rm -Rf *.o $(EXECUTABLE)
