CC=g++
CFLAGS=-c -Wall -D_SERVER -D_LINUX -w -I../../Shared -I.
SOURCES=$(wildcard *.cpp)
SOURCES+=../../Shared/CString.cpp ../../Shared/CMutex.cpp ../../Shared/CLibrary.cpp ../../Shared/SharedUtility.cpp ../../Shared/CThread.cpp
OBJECTS=$(SOURCES:.cpp=.o)
EXECUTABLE=../../Binary/ivmp-svr
PCH=StdInc.h

all: $(PCH) $(SOURCES) $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS) 
        g++ $(OBJECTS) -lpthread -ldl -o $@ 

.cpp.o:
        $(CC) $(CFLAGS) $< -o $@

pch:
        $(CC) $(CFLAGS) $(PCH) -o $(PCH).gch

clean:
        rm -Rf $(OBJECTS) $(EXECUTABLE)