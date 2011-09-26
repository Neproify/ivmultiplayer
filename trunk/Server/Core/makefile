CC=g++
CFLAGS=-c -w -D_SERVER -D_LINUX -I../../Shared -I../../Vendor/Squirrel -I../../Vendor/ -I.
SOURCES=$(wildcard *.cpp)
SOURCES+=$(wildcard ../../Vendor/tinyxml/*.cpp)
SOURCES+=$(wildcard Natives/*.cpp)
SOURCES+=$(wildcard ../../Shared/Scripting/Natives/*.cpp)
SOURCES+=../../Shared/Scripting/CScriptTimer.cpp ../../Shared/Scripting/CScriptTimerManager.cpp ../../Shared/Scripting/CScriptingManager.cpp ../../Shared/CXML.cpp ../../Shared/SharedUtility.cpp ../../Shared/Scripting/CSquirrel.cpp ../../Shared/CSQLite.cpp ../../Shared/Scripting/CSquirrelArguments.cpp ../../Shared/Game/CTrafficLights.cpp ../../Shared/Game/CTime.cpp
SOURCES+=$(wildcard ../../Shared/Network/*.cpp) ../../Shared/CLibrary.cpp ../../Shared/CString.cpp ../../Shared/Threading/CThread.cpp ../../Shared/Threading/CMutex.cpp ../../Shared/CLogFile.cpp ../../Shared/Game/CPadState.cpp
SOURCES+=$(wildcard ../../Vendor/md5/*.cpp) ../../Shared/CSettings.cpp ../../Shared/CExceptionHandler.cpp ../../Shared/Linux.cpp
OBJECTS=$(SOURCES:.cpp=.o)
EXECUTABLE=../../Binary/ivmp-svr

all: $(SOURCES) $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS) 
	gcc $(CFLAGS) ../../Vendor/mongoose/mongoose.c -o mongoose.o
	g++ $(OBJECTS) mongoose.o -lpthread -ldl ../../Vendor/sqlite/libsqlite.a ../../Vendor/Squirrel/libsquirrel.a ../../Vendor/tinyxml/libtinyxml.a -o $@ 

.cpp.o:
	$(CC) $(CFLAGS) $< -o $@

clean:
	rm -Rf $(OBJECTS) $(EXECUTABLE)
