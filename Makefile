SOURCEFILES := gemu.c memory.c processor.c
HEADERFILES := gemu.h memory.h processor.h
SOURCES := $(addprefix src/, $(SOURCEFILES))
HEADERS := $(addprefix src/header/, $(HEADERFILES))

CC := gcc
CFLAGS := -g -Wall
OBJECTS := $(addprefix obj/, $(SOURCEFILES:.c=.o))
EXECUTABLE := gemu

all: $(SOURCES) $(EXECUTABLE) #$(HEADERS)

$(EXECUTABLE): $(OBJECTS) #$(HEADERS)
	$(CC) $(CFLAGS) $(OBJECTS) -o $@

#Force recompile if associated header changes
obj/%.o: src/%.c src/header/%.h
	$(CC) $(CFLAGS) -c $< -o $@

#Overloaded object compiler for .cpp files without headers
obj/%.o: src/%.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJECTS) $(EXECUTABLE) src/*~ src/header/*~
