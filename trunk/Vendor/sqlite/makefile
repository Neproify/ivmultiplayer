CC=gcc
CFLAGS=-m32 -c -Wall -D_SERVER -static
LDFLAGS=-lpthread -static
SOURCES=$(wildcard *.c)
OBJECTS=$(SOURCES:.c=.o)
EXECUTABLE=libsqlite.a

all: $(SOURCES) $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS) 
	ar -rv $@ $(OBJECTS) 

.cpp.o:
	$(CC) $(CFLAGS) $< -o $@

clean:
	rm -Rf *.o $(EXECUTABLE)
