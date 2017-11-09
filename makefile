CC      = g++
CPPFLAGS= -c -m64 -march=native -std=c++14 -DGCC -DRelease -O3 -DOXLABEL
LDFLAGS = -O3
SOURCES = main.cpp Util.cpp stringsim.cpp
OBJECTS = $(SOURCES:.cpp=.o)
EXECUTABLE=stringsim

all: $(SOURCES) $(EXECUTABLE)

$(EXECUTABLE) : $(OBJECTS)
	$(CC) $(LDFLAGS) $(OBJECTS) -o $@

.cpp.o : 
	$(CC) $(CPPFLAGS) $< -o $@

clean:
	rm -f *.o

