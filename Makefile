# Makefile for Mac arm x64 infrastructure

GAME_EDITOR = GameEditor
TEST_ALL = GameEditorTest
TEST_ALL_COMMAND = test_all
COMPILER = g++
INCLUDE_PATHS = -I include
FRAMEWORKS = -framework Cocoa -framework OpenGL -framework IOKit
LIBRARY_PATHS = -L lib/GLFW
LINKER_FLAGS = -l glfw3
TEST_LINKER_FLAG = $(LINKER_FLAGS) -l gtest -l gtest_main
COMPILER_FLAGS = -std=c++20 -Wall -O0 -g -Wno-deprecated-declarations
TEST_COMPILER_FLAGS = $(COMPILER_FLAGS) -mmacosx-version-min=14.1
SRC_DIR = src
BUILD_DIR = build
TEST_SRC_DIR = test/src
TEST_BUILD_DIR = test/build
TEST_OUTPUT_DIR = test/output
CPP_FILES = $(wildcard $(SRC_DIR)/*.cpp)
OBJ_FILES = $(wildcard $(BUILD_DIR)/*.o)
TEST_FILES = $(wildcard $(TEST_OUTPUT_DIR)/*)
TEST_CPP_FILES = $(wildcard $(TEST_SRC_DIR)/*.cpp)
TEST_OBJ_FILES = $(wildcard $(TEST_BUILD_DIR)/*.o)
OBJS = $(patsubst $(SRC_DIR)/%.cpp, $(BUILD_DIR)/%.o, $(CPP_FILES))
TEST_OBJS = $(patsubst $(TEST_SRC_DIR)/%.cpp, $(TEST_BUILD_DIR)/%.o, $(TEST_CPP_FILES))
UNIT_TESTS = $(patsubst $(TEST_SRC_DIR)/%.test.cpp, $(TEST_OUTPUT_DIR)/%.test, $(TEST_CPP_FILES))

all: $(GAME_EDITOR) $(TEST_ALL) $(UNIT_TESTS)
.PHONY : all

$(GAME_EDITOR) : $(OBJS)
	$(COMPILER) $(COMPILER_FLAGS) $(INCLUDE_PATHS) $(FRAMEWORKS) $(LIBRARY_PATHS) $(LINKER_FLAGS) $^ -o $@

$(TEST_ALL_COMMAND) : $(TEST_ALL)
.PHONY : $(TEST_ALL_COMMAND)

$(TEST_ALL) : $(TEST_OBJS) $(filter-out $(BUILD_DIR)/main.o, $(OBJS))
	$(COMPILER) $(TEST_COMPILER_FLAGS) $(INCLUDE_PATHS) $(FRAMEWORKS) $(LIBRARY_PATHS) $(TEST_LINKER_FLAG) $^ -o $@

$(BUILD_DIR)/%.o : $(SRC_DIR)/%.cpp | $(BUILD_DIR)
	$(COMPILER) $(COMPILER_FLAGS) $(INCLUDE_PATHS) -c $< -o $@

$(TEST_BUILD_DIR)/%.o : $(TEST_SRC_DIR)/%.cpp | $(TEST_BUILD_DIR)
	$(COMPILER) $(TEST_COMPILER_FLAGS) $(INCLUDE_PATHS) -c $< -o $@

# Individually compile test file into TEST_OUTPUT_DIR through their prefix name (e.g. for file x.test.cpp, we type `make x.test`)
%.test : $(TEST_BUILD_DIR)/%.test.o $(filter-out $(BUILD_DIR)/main.o, $(OBJS)) | $(TEST_OUTPUT_DIR)/%.test
	$(COMPILER) $(TEST_COMPILER_FLAGS) $(INCLUDE_PATHS) $(FRAMEWORKS) $(LIBRARY_PATHS) $(TEST_LINKER_FLAG) $^ -o $(TEST_OUTPUT_DIR)/$@
.PHONY : %.test

$(TEST_OUTPUT_DIR)/%.test : $(TEST_BUILD_DIR)/%.test.o $(filter-out $(BUILD_DIR)/main.o, $(OBJS))
	$(COMPILER) $(TEST_COMPILER_FLAGS) $(INCLUDE_PATHS) $(FRAMEWORKS) $(LIBRARY_PATHS) $(TEST_LINKER_FLAG) $^ -o $@

$(BUILD_DIR) :
	mkdir $(BUILD_DIR)

$(TEST_BUILD_DIR) :
	mkdir $(TEST_BUILD_DIR)

clean : clean_editor clean_test
.PHONY : clean

clean_editor :
	rm -f $(GAME_EDITOR) $(OBJ_FILES) .depend
.PHONY : clean_editor

clean_test :
	rm -f $(TEST_ALL) $(TEST_OBJ_FILES) $(TEST_FILES)
.PHONY : clean_test

.depend : $(SRC_DIR) $(CPP_FILES)
	$(COMPILER) -MM $(INCLUDE_PATHS) $(filter-out $<, $^) | sed '/\.o/ s/^/${BUILD_DIR}\//' > .depend
include .depend