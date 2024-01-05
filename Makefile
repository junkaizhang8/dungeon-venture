APP = Main
COMPILER = g++
INCLUDE_PATHS = -I include
FRAMEWORKS = -framework Cocoa -framework OpenGL -framework IOKit
LIBRARY_PATHS = -L lib/GLFW
LINKER_FLAGS = -l glfw3
COMPILER_FLAGS = -std=c++11 -Wall -O0 -g -Wno-deprecated-declarations
SRC_DIR = src
BUILD_DIR = build
CPP_FILES = $(wildcard $(SRC_DIR)/*.cpp)
HPP_FILES = $(wildcard $(SRC_DIR)/*.hpp)
OBJ_FILES = $(wildcard $(BUILD_DIR)/*.o)
OBJS = $(patsubst $(SRC_DIR)/%.cpp, $(BUILD_DIR)/%.o, $(CPP_FILES))

all: $(APP)

$(APP): $(OBJS)
	$(COMPILER) $(COMPILER_FLAGS) $(INCLUDE_PATHS) $(FRAMEWORKS) $(LIBRARY_PATHS) $(LINKER_FLAGS) $^ -o $@

$(BUILD_DIR)/%.o : $(SRC_DIR)/%.cpp
	$(COMPILER) $(COMPILER_FLAGS) $(INCLUDE_PATHS) -c $< -o $@

clean:
	rm -f $(APP) $(OBJ_FILES) .depend
.PHONY: clean

.depend: $(SRC_DIR) $(CPP_FILES)
	$(COMPILER) -MM $(INCLUDE_PATHS) $(filter-out $<, $^) | sed '/\.o/ s/^/${BUILD_DIR}\//' > .depend
include .depend