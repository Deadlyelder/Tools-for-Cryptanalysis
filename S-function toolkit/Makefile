CC=g++
CXXFLAGS=-c -Wall -O3 -DPRINT_MATRICES
LDFLAGS=-Wall
SOURCES=test.cc matrix-xdp.cc matrix-adp.cc matrix-mul3.cc matrix-xdp3.cc
OBJECTS=$(SOURCES:.cc=.o)
EXECUTABLE=test

all: $(SOURCES) $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
	$(CC) $(LDFLAGS) $(OBJECTS) -o $@

.cpp.o:
	$(CC) $(CFLAGS) $< -o $@
clean:
	\rm -f *.o *~ $(EXECUTABLE)
