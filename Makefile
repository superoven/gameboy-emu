SOURCEFILES := gemu.c memory.c processor.c loadrom.c disassembler.c
HEADERFILES := gemu.h memory.h processor.h
SOURCES := $(addprefix src/, $(SOURCEFILES))
HEADERS := $(addprefix src/header/, $(HEADERFILES))

CC := gcc
CFLAGS := -g -Wall -std=c99
OBJECTS := $(addprefix obj/, $(SOURCEFILES:.c=.o))
EXECUTABLE := gemu

#I can't seem to remember how to make it properly recompile if
#a header file changes, it is commented out for now
#If you change a header, make sure to make clean first for now

all: $(SOURCES) $(EXECUTABLE) #$(HEADERS)

$(EXECUTABLE): $(OBJECTS) #$(HEADERS)
	$(CC) $(CFLAGS) $(OBJECTS) -o $@

#Force recompile if associated header changes
obj/%.o: src/%.c src/header/%.h
	$(CC) $(CFLAGS) -c $< -o $@

#Overloaded object compiler for .c files without headers
obj/%.o: src/%.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJECTS) $(EXECUTABLE) src/*~ src/header/*~
