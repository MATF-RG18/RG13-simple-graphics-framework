KEYBOARD_DEMO = keyboard_demo.out 
MOUSE_DEMO = mouse_demo.out 
RESOURCE_DEMO = resource_demo.out 

CXX     = g++
CXXFLAGS = -std=c++17 #-I sgf/include
LDLIBS  = -lglut -lGLU -lGL
WFLAGS = -Wall -Wextra -Wno-unused

H_DIR_SGF = include/sgf
SRC_DIR_SGF = src/sgf
OBJ_DIR_SGF = $(SRC_DIR_SGF)/obj
H_FILES_SGF = $(wildcard $(H_DIR_SGF)/*.(h|(hpp)))
SRC_FILES_SGF = $(wildcard $(SRC_DIR_SGF)/*.cpp)
OBJ_FILES_SGF = $(patsubst $(SRC_DIR_SGF)/%.cpp,$(OBJ_DIR_SGF)/%.o,$(SRC_FILES_SGF))

H_DIR_OPENGL = include/opengl
SRC_DIR_OPENGL = src/opengl
OBJ_DIR_OPENGL = $(SRC_DIR_OPENGL)/obj
H_FILES_OPENGL = $(wildcard $(H_DIR_OPENGL)/*.(h|(hpp)))
SRC_FILES_OPENGL = $(wildcard $(SRC_DIR_OPENGL)/*.cpp)
OBJ_FILES_OPENGL = $(patsubst $(SRC_DIR_OPENGL)/%.cpp,$(OBJ_DIR_OPENGL)/%.o,$(SRC_FILES_OPENGL))

H_DIR_DEPENDENCIES = include/dependencies
SRC_DIR_DEPENDENCIES = src/dependencies
OBJ_DIR_DEPENDENCIES = $(SRC_DIR_DEPENDENCIES)/obj
H_FILES_DEPENDENCIES = $(wildcard $(H_DIR_DEPENDENCIES)/*.(h|(hpp)))
SRC_FILES_DEPENDENCIES = $(wildcard $(SRC_DIR_DEPENDENCIES)/*.cpp)
OBJ_FILES_DEPENDENCIES = $(patsubst $(SRC_DIR_DEPENDENCIES)/%.cpp,$(OBJ_DIR_DEPENDENCIES)/%.o,$(SRC_FILES_DEPENDENCIES))

H_DIR_DEPENDENCIES_ABL = include/dependencies/abl
SRC_DIR_DEPENDENCIES_ABL = src/dependencies/abl
OBJ_DIR_DEPENDENCIES_ABL = $(SRC_DIR_DEPENDENCIES_ABL)/obj
H_FILES_DEPENDENCIES_ABL = $(wildcard $(H_DIR_DEPENDENCIES_ABL)/*.(h|(hpp)))
SRC_FILES_DEPENDENCIES_ABL = $(wildcard $(SRC_DIR_DEPENDENCIES_ABL)/*.cpp)
OBJ_FILES_DEPENDENCIES_ABL = $(patsubst $(SRC_DIR_DEPENDENCIES_ABL)/%.cpp,$(OBJ_DIR_DEPENDENCIES_ABL)/%.o,$(SRC_FILES_DEPENDENCIES_ABL))

H_DIR_DEPENDENCIES_VDK = include/dependencies/vdk
SRC_DIR_DEPENDENCIES_VDK = src/dependencies/vdk
OBJ_DIR_DEPENDENCIES_VDK = $(SRC_DIR_DEPENDENCIES_VDK)/obj
H_FILES_DEPENDENCIES_VDK = $(wildcard $(H_DIR_DEPENDENCIES_VDK)/*.(h|(hpp)))
SRC_FILES_DEPENDENCIES_VDK = $(wildcard $(SRC_DIR_DEPENDENCIES_VDK)/*.cpp)
OBJ_FILES_DEPENDENCIES_VDK = $(patsubst $(SRC_DIR_DEPENDENCIES_VDK)/%.cpp,$(OBJ_DIR_DEPENDENCIES_VDK)/%.o,$(SRC_FILES_DEPENDENCIES_VDK))


default: 
	@echo "Type: 'make demos' to make demos."
	@echo "Type: 'make dist' to make framework distribution package."

.PHONY: demos dist clean

demos: $(KEYBOARD_DEMO) $(MOUSE_DEMO) $(RESOURCE_DEMO)

DISTNAME = simple_graphics_framework
dist: libs headers
	@(rm  -r $(DISTNAME); mkdir $(DISTNAME) ; mv headers $(DISTNAME) && mv $(DISTNAME)/headers $(DISTNAME)/include && mv libsgf.a $(DISTNAME)/ && tar -czvf $(DISTNAME).tar.gz $(DISTNAME) ; rm -r $(DISTNAME))

libs: libsgf.a

headers: 
	@(rm headers -r ; mkdir headers ; cp include/ headers -R ; mv headers/include headers/sgf)

$(KEYBOARD_DEMO): ./demos/keyboard_demo/main.cpp $(OBJ_FILES_SGF) $(OBJ_FILES_OPENGL) $(OBJ_FILES_DEPENDENCIES) $(OBJ_FILES_DEPENDENCIES_ABL) $(OBJ_FILES_DEPENDENCIES_VDK)
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LDLIBS) $(WFLAGS)

$(MOUSE_DEMO): ./demos/mouse_demo/main.cpp $(OBJ_FILES_SGF) $(OBJ_FILES_OPENGL) $(OBJ_FILES_DEPENDENCIES) $(OBJ_FILES_DEPENDENCIES_ABL) $(OBJ_FILES_DEPENDENCIES_VDK)
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LDLIBS) $(WFLAGS)

$(RESOURCE_DEMO): ./demos/resource_demo/main.cpp $(OBJ_FILES_SGF) $(OBJ_FILES_OPENGL) $(OBJ_FILES_DEPENDENCIES) $(OBJ_FILES_DEPENDENCIES_ABL) $(OBJ_FILES_DEPENDENCIES_VDK)
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LDLIBS) $(WFLAGS) -lstdc++fs

$(OBJ_DIR_SGF)/%.o: $(SRC_DIR_SGF)/%.cpp
	$(CXX) $(CXXFLAGS) -c -o $@ $< $(LDLIBS) $(WFLAGS)

$(OBJ_DIR_OPENGL)/%.o: $(SRC_DIR_OPENGL)/%.cpp
	$(CXX) $(CXXFLAGS) -c -o $@ $< $(LDLIBS) $(WFLAGS)

$(OBJ_DIR_DEPENDENCIES)/%.o: $(SRC_DIR_DEPENDENCIES)/%.cpp
	$(CXX) $(CXXFLAGS) -c -o $@ $< $(LDLIBS) $(WFLAGS)

$(OBJ_DIR_DEPENDENCIES_ABL)/%.o: $(SRC_DIR_DEPENDENCIES_ABL)/%.cpp
	$(CXX) $(CXXFLAGS) -c -o $@ $< $(LDLIBS) $(WFLAGS)

$(OBJ_DIR_DEPENDENCIES_VDK)/%.o: $(SRC_DIR_DEPENDENCIES_VDK)/%.cpp
	$(CXX) $(CXXFLAGS) -c -o $@ $< $(LDLIBS) $(WFLAGS)


#link library files into a static library
libsgf.a: $(OBJ_FILES_SGF) $(OBJ_FILES_OPENGL) $(OBJ_FILES_DEPENDENCIES) $(OBJ_FILES_DEPENDENCIES_ABL) $(OBJ_FILES_DEPENDENCIES_VDK)
	ar rcs $@ $^

clean:
	rm -rf $(OBJ_DIR_SGF)/*.o $(OBJ_DIR_OPENGL)/*.o $(OBJ_DIR_DEPENDENCIES)/*.o $(OBJ_DIR_DEPENDENCIES_ABL)/*.o $(OBJ_FILES_DEPENDENCIES_VDK)/*.o *.a *.out $(DISTNAME).tar.gz
