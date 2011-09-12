CC=g++
CFLAGS=-c -Wall -D_LINUX -w -ISDK/ -I.
SOURCES=$(wildcard *.cpp)
SOURCES+=$(wildcard SDK/*.cpp)
OBJECTS=$(SOURCES:.cpp=.o)
EXECUTABLE=ivmp_helloworld.so

all: $(SOURCES) $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS) 
	g++ $(OBJECTS) -lpthread -ldl -shared -o $@ 

.cpp.o:
	$(CC) $(CFLAGS) $< -o $@

clean:
	rm -Rf *.o $(EXECUTABLE)
