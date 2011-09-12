g++ -m32 *.cpp ../Shared/sqlite/libsqlite.a ../Shared/CRakNet.cpp \
../Shared/Scripting/CScriptingManager.cpp ../Shared/Utility.cpp \
../Shared/Scripting/CSquirrel.cpp ./Natives/*.cpp ../Shared/md5/md5.cpp \
../Shared/RakNet/libraknet.a ../Shared/Squirrel/libsquirrel.a ../Shared/tinyxml/libtinyxml.a -lpthread -ldl \
-I../Shared -Wno-write-strings \
-D_SERVER -o IVMPserver
