CC=g++
CFLAGS=-m32 -c -Wall -g -D_SERVER -I. -I../../Shared -D_LINUX
LDFLAGS=-lpthread -ldl
SOURCES=$(wildcard *.cpp)
SOURCES+=$(wildcard ../../Shared/Threading/*.cpp) ../../Shared/CString.cpp ../../Shared/CLibrary.cpp ../../Shared/SharedUtility.cpp ../../Shared/Linux.cpp
OBJECTS=$(SOURCES:.cpp=.o)
EXECUTABLE=../../Binary/ivmp-svr

all: $(SOURCES) $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS) 
	g++ -m32 -o $@ $(OBJECTS) $(LDFLAGS)

.cpp.o:
	$(CC) $(CFLAGS) $< -o $@

clean:
	rm -Rf $(OBJECTS) $(EXECUTABLE)
