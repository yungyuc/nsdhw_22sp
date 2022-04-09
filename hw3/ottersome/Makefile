TARGETNAME = _matrix
TARGET = $(addprefix $(TARGETNAME),$(shell python3-config --extension-suffix))

# Directories needed for compilation
BUILD_DIR ?= ./modules
SRC_DIR ?= ./src
OBJ_DIR ?= ./build
DEPS_DIR ?= ./deps
TESTS_DIR ?= ./tests


# Since we will be using pybind as header library
# I want to see if I can get rid of the need for python.h as prof says
UNAME_S := $(shell uname)
ifeq ($(UNAME_S),Linux)
	INC_DIRS := -I./include $(shell python3-config --includes) $(shell python3 -m pybind11 --includes) -I/usr/include/mkl/ -I/opt/intel/mkl/include
	# INC_DIRS := -I./include $(shell python3 -m pybind11 --includes) -I/usr/include/mkl
endif
ifeq ($(UNAME_S),Darwin)
	INC_DIRS := -I./include $(shell python3 -m pybind11 --includes) -I/opt/homebrew/opt/openblas/include
endif
#INC_DIRS := -I./include $(shell python3-config --includes)

SRC_FILES := $(wildcard $(SRC_DIR)/*.cpp)
OBJ_FILES := $(patsubst $(SRC_DIR)/%.cpp,$(OBJ_DIR)/%.o,$(SRC_FILES))
DEPENDS := $(OBJ_FILES:.o=.d)
#Comment to force change in git

LDFLAGS?= -shared -lblas
CPPFLAGS ?= -pedantic -O3 -Wall -Wextra -std=c++11 -MMD -MP -fPIC $(INC_DIRS)
		
.PHONY: clean, default, test, foundation

default: test

test: foundation $(BUILD_DIR)/$(TARGET)
	python3 $(TESTS_DIR)/unit_testing.py
	python3 -m pytest $(TESTS_DIR)/test_performance.py

$(BUILD_DIR)/$(TARGET): $(OBJ_FILES)
	@echo 'Building Targe for ${OBJ_FILES}'
	$(CXX) $(OBJ_FILES) -o $@ $(LDFLAGS)
	cp $@ .
	
$(OBJ_DIR)/%.o:$(SRC_DIR)/%.cpp
# echo "For $@ This is the obj file ${SRC_FILES}"
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c $< -o $@
	
#
# Helpers
#
foundation: $(BUILD_DIR) $(SRC_DIR) $(OBJ_DIR) 

$(BUILD_DIR) $(SRC_DIR) $(OBJ_DIR): #$(DEPS_DIR):
	mkdir $@

-include $(DEPENDS)

clean:
	rm -rf $(BUILD_DIR) $(OBJ_DIR) ./*.so ./__pycache__ performance.txt || true

