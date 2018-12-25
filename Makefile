KEYBOARD_DEMO = keyboard_demo.out 

CXX     = g++
CXXFLAGS = -std=c++17 #-I sgf/include
LDLIBS  = -lglut -lGLU -lGL
WFLAGS = -Wall -Wextra -Wno-unused

H_DIR_SGF = sgf/include/sgf
SRC_DIR_SGF = sgf/src/sgf
OBJ_DIR_SGF = $(SRC_DIR_SGF)/obj
H_FILES_SGF = $(wildcard $(H_DIR_SGF)/*.(h|(hpp)))
SRC_FILES_SGF = $(wildcard $(SRC_DIR_SGF)/*.cpp)
OBJ_FILES_SGF = $(patsubst $(SRC_DIR_SGF)/%.cpp,$(OBJ_DIR_SGF)/%.o,$(SRC_FILES_SGF))

H_DIR_OPENGL = sgf/include/opengl
SRC_DIR_OPENGL = sgf/src/opengl
OBJ_DIR_OPENGL = $(SRC_DIR_OPENGL)/obj
H_FILES_OPENGL = $(wildcard $(H_DIR_OPENGL)/*.(h|(hpp)))
SRC_FILES_OPENGL = $(wildcard $(SRC_DIR_OPENGL)/*.cpp)
OBJ_FILES_OPENGL = $(patsubst $(SRC_DIR_OPENGL)/%.cpp,$(OBJ_DIR_OPENGL)/%.o,$(SRC_FILES_OPENGL))

H_DIR_DEPENDENCIES = sgf/include/dependencies
SRC_DIR_DEPENDENCIES = sgf/src/dependencies
OBJ_DIR_DEPENDENCIES = $(SRC_DIR_DEPENDENCIES)/obj
H_FILES_DEPENDENCIES = $(wildcard $(H_DIR_DEPENDENCIES)/*.(h|(hpp)))
SRC_FILES_DEPENDENCIES = $(wildcard $(SRC_DIR_DEPENDENCIES)/*.cpp)
OBJ_FILES_DEPENDENCIES = $(patsubst $(SRC_DIR_DEPENDENCIES)/%.cpp,$(OBJ_DIR_DEPENDENCIES)/%.o,$(SRC_FILES_DEPENDENCIES))

default: 
	@echo "Type: 'make demos' to make demos."
	@echo "Type: 'make dist' to make framework distribution package."

.PHONY: demos dist clean

demos: $(KEYBOARD_DEMO)

DISTNAME = simple_graphics_framework
dist: libs headers
	@(rm  -r ; mkdir $(DISTNAME) ; mv headers $(DISTNAME) && mv $(DISTNAME)/headers $(DISTNAME)/include && mv libsgf.a $(DISTNAME)/ && tar -czvf $(DISTNAME).tar.gz $(DISTNAME) ; rm -r $(DISTNAME))

libs: libsgf.a

headers: 
	@(rm headers -r ; mkdir headers ; cp sgf/include/ headers -R ; mv headers/include headers/sgf)

$(KEYBOARD_DEMO): ./demos/keyboard_demo/main.cpp $(OBJ_FILES_SGF) $(OBJ_FILES_OPENGL) $(OBJ_FILES_DEPENDENCIES)
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LDLIBS) $(WFLAGS)

$(OBJ_DIR_SGF)/%.o: $(SRC_DIR_SGF)/%.cpp
	$(CXX) $(CXXFLAGS) -c -o $@ $< $(LDLIBS) $(WFLAGS)

$(OBJ_DIR_OPENGL)/%.o: $(SRC_DIR_OPENGL)/%.cpp
	$(CXX) $(CXXFLAGS) -c -o $@ $< $(LDLIBS) $(WFLAGS)

$(OBJ_DIR_DEPENDENCIES)/%.o: $(SRC_DIR_DEPENDENCIES)/%.cpp
	$(CXX) $(CXXFLAGS) -c -o $@ $< $(LDLIBS) $(WFLAGS)

#link library files into a static library
libsgf.a: $(OBJ_FILES_SGF) $(OBJ_FILES_OPENGL) $(OBJ_FILES_DEPENDENCIES) 	
	ar rcs $@ $^

clean:
	rm -rf $(OBJ_DIR_SGF)/*.o $(OBJ_DIR_OPENGL)/*.o $(OBJ_DIR_DEPENDENCIES)/*.o *.a *.out $(DISTNAME).tar.gz
