CC=g++
CFLAGS=-c -w -D_SERVER -D_LINUX -I../../Shared -I../../Shared/Squirrel -I../../Shared/Squirrel -I.
SOURCES=$(wildcard *.cpp)
SOURCES+=$(wildcard ../../Shared/tinyxml/*.cpp)
SOURCES+=$(wildcard Natives/*.cpp)
SOURCES+=$(wildcard ../../Shared/Scripting/Natives/*.cpp)
SOURCES+=../../Shared/Scripting/CScriptTimer.cpp ../../Shared/Scripting/CScriptTimerManager.cpp ../../Shared/Scripting/CScriptingManager.cpp ../../Shared/CXML.cpp ../../Shared/SharedUtility.cpp ../../Shared/Scripting/CSquirrel.cpp ../../Shared/md5/md5.cpp ../../Shared/crc32/crc32.cpp ../../Shared/Scriptfiles.cpp ../../Shared/CSQLite.cpp ../../Shared/Scripting/CSquirrelArguments.cpp ../../Shared/CArea.cpp ../../Shared/CTrafficLights.cpp ../../Shared/CTime.cpp
SOURCES+=$(wildcard ../../Shared/Network/*.cpp) ../../Shared/CLibrary.cpp ../../Shared/CString.cpp ../../Shared/CThread.cpp ../../Shared/CMutex.cpp ../../Shared/CLogFile.cpp
OBJECTS=$(SOURCES:.cpp=.o)
EXECUTABLE=../../Binary/ivmp-svr

all: $(SOURCES) $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS) 
	gcc $(CFLAGS) ../../Shared/mongoose/mongoose.c -o mongoose.o
	g++ $(OBJECTS) mongoose.o -lpthread -ldl ../../Shared/sqlite/libsqlite.a ../../Shared/Squirrel/libsquirrel.a ../../Shared/tinyxml/libtinyxml.a -o $@ 

.cpp.o:
	$(CC) $(CFLAGS) $< -o $@

clean:
	rm -Rf $(OBJECTS) $(EXECUTABLE)
