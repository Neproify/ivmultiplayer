CC=g++
CFLAGS=-c -w -D_SERVER -D_LINUX -I../../Shared -I. -I../../Shared/Network

SOURCES=$(wildcard *.cpp)
SOURCES+=../../Shared/SharedUtility.cpp ../../Shared/CString.cpp ../../Shared/Network/CBitStream.cpp ../../Shared/Game/CControlState.cpp ../../Shared/Linux.cpp
SOURCES+=$(wildcard RakNet/*.cpp)
OBJECTS=$(SOURCES:.cpp=.o)
EXECUTABLE=../../Binary/Network.Core.so

all: $(SOURCES) $(EXECUTABLE)

pch:
	g++ $(CFLAGS) StdInc.h -o StdInc.h.gch

$(EXECUTABLE): $(OBJECTS) 
	g++ $(OBJECTS) -lpthread -ldl -shared -o $@ 

.cpp.o:
	$(CC) $(CFLAGS) $< -o $@

clean:
	rm -Rf *.o $(EXECUTABLE)
