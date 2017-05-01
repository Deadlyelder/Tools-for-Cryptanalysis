CC = g++
CFLAGS = -O3 -funroll-loops -Wall
INCLUDES= ./includes/
SOURCE_PATH = ./src/

LIBSRC = CodeWord.cpp CodeMatrix.cpp CodeWordFile.cpp \
		 InputHandler.cpp LowWeightSearch.cpp RandomNumberGenerator.cpp \
		 Parameters.cpp HammingWeight.cpp mtrand.cpp
LIBOBJ = $(LIBSRC:%.cpp=%.o)

LIB = libCodingTool.a
LIBDEST = ../lib/

EXAMPLE1 = sha1me
EXAMPLE2 = search
EXAMPLE3 = shortening
EXAMPLE4 = allinone

all: $(LIB) $(EXAMPLE1) $(EXAMPLE2) $(EXAMPLE3) $(EXAMPLE4)

$(LIB): $(LIBOBJ)
	cd $(SOURCE_PATH); \
	$(AR) r $(LIBDEST)$(LIB) $(LIBOBJ)

$(LIBOBJ): %.o: $(SOURCE_PATH)%.cpp
	$(CC) $(CFLAGS) -I$(INCLUDES) -c $< -o $(SOURCE_PATH)$@


$(EXAMPLE1): ./examples/$(EXAMPLE1).cpp
	$(CC) $(CFLAGS) -I$(INCLUDES) -L./lib/ $< -o ./bin/$(EXAMPLE1) -lCodingTool
	
$(EXAMPLE2): ./examples/$(EXAMPLE2).cpp
	$(CC) $(CFLAGS) -I$(INCLUDES) -L./lib/ $< -o ./bin/$(EXAMPLE2) -lCodingTool
	
$(EXAMPLE3): ./examples/$(EXAMPLE3).cpp
	$(CC) $(CFLAGS) -I$(INCLUDES) -L./lib/ $< -o ./bin/$(EXAMPLE3) -lCodingTool
	
$(EXAMPLE4): ./examples/$(EXAMPLE4).cpp
	$(CC) $(CFLAGS) -I$(INCLUDES) -L./lib/ $< -o ./bin/$(EXAMPLE4) -lCodingTool
	
clean:
	rm $(SOURCE_PATH)*.o ./lib/$(LIB) bin/$(EXAMPLE1)  bin/$(EXAMPLE2)  bin/$(EXAMPLE3) bin/$(EXAMPLE4)
