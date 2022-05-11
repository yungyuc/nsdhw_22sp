TARGETNAME = _matrix
TARGET = $(addprefix $(TARGETNAME),$(shell python3-config --extension-suffix))

# Directories needed for compilation
BUILD_DIR ?= ./modules
SRC_DIR ?= ./src
OBJ_DIR ?= ./build
DEPS_DIR ?= ./deps
TESTS_DIR ?= ./tests

UNAME_S := $(shell uname)
INC_DIRS := -I./include $(shell python3-config --includes) $(shell python3 -m pybind11 --includes)\
			-I/usr/include/mkl/ -I/opt/intel/mkl/include\
			-I$(shell python3 -c 'import numpy; print(numpy.get_include())')

SRC_FILES := ../mod.cpp
OBJ_FILES := $(OBJ_DIR)/mod.o 

LDFLAGS?= -shared -lblas
CPPFLAGS ?= -pedantic -O3 -Wall -Wextra -std=c++17 -MMD -MP -fPIC $(INC_DIRS)
		
.PHONY: clean, default, test, foundation

default: test

test: foundation $(BUILD_DIR)/$(TARGET)
	#python3 $(TESTS_DIR)/unit_testing.py
	#python3 -m pytest $(TESTS_DIR)/test_performance.py

$(BUILD_DIR)/$(TARGET): $(OBJ_FILES)
	$(CXX) $(OBJ_FILES) -o $@ $(LDFLAGS)
	cp $@ .
	
$(OBJ_DIR)/%.o:$(SRC_FILES)
# echo "For $@ This is the obj file ${SRC_FILES}"
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c $< -o $@
	
foundation: $(BUILD_DIR) $(SRC_DIR) $(OBJ_DIR) 

$(BUILD_DIR) $(SRC_DIR) $(OBJ_DIR): #$(DEPS_DIR):
	mkdir $@


clean:
	rm -rf $(BUILD_DIR) $(OBJ_DIR) ./*.so ./__pycache__ performance.txt || true

