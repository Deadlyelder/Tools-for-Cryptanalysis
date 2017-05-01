CC = gcc
CFLAGS = -Wall -std=gnu99 -pedantic -O3
LDFLAGS = -lanf2cnf

EXE = GBsolve SATsolve looping

all: $(EXE)

clean:
	rm -f $(EXE)

dist: clean
	cd ../ ; tar -v -cz -f alpath.tar.gz alpath

.PHONY: all clean dist
