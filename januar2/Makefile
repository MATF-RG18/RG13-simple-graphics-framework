PROGRAM1 = spaceClimbing.out
PROGRAM2 = senseFX.out
CXX     = g++
CXXFLAGS = -std=c++17 -I include
LDLIBS  = -lglut -lGLU -lGL -lstdc++fs -g
WFLAGS = -Wall -Wextra
SRC_DIR = src
OBJ_DIR = $(SRC_DIR)/obj
SRC_FILES = $(wildcard $(SRC_DIR)/*.cpp)
OBJ_FILES = $(patsubst $(SRC_DIR)/%.cpp,$(OBJ_DIR)/%.o,$(SRC_FILES))

all: $(PROGRAM1) $(PROGRAM2)

$(PROGRAM1): $(OBJ_FILES)
	$(CXX) $(CXXFLAGS) -o $(PROGRAM1) spaceClimbing.cpp $(OBJ_FILES) $(LDLIBS) $(WFLAGS)

$(PROGRAM2): $(OBJ_FILES)
	$(CXX) $(CXXFLAGS) -o $(PROGRAM2) senseFX.cpp $(OBJ_FILES) $(LDLIBS) $(WFLAGS)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	$(CXX) $(CXXFLAGS) -c -o $@ $< $(LDLIBS) $(WFLAGS)
	

.PHONY: clean dist

clean:
	-rm $(OBJ_DIR)/*.o $(PROGRAM1) $(PROGRAM2)

dist: clean
	-tar -chvj -C .. -f ../$(PROGRAM1).tar.bz2 $(PROGRAM2)
