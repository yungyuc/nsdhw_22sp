TARGETNAME = _matrix
TARGET = $(addprefix $(TARGETNAME),$(shell python3-config --extension-suffix))

BUILD_DIR ?= ./modules
SRC_DIR ?= ./src
# Since we will be using pybind as header library
# I want to see if I can get rid of the need for python.h as prof says
UNAME_S := $(shell uname)
ifeq ($(UNAME_S),Linux)
	INC_DIRS := -I./include $(shell python3 -m pybind11 --includes) -I/usr/include/mkl
endif
ifeq ($(UNAME_S),Darwin)
	INC_DIRS := -I./include $(shell python3 -m pybind11 --includes) -I/opt/homebrew/opt/openblas/include
endif
#INC_DIRS := -I./include $(shell python3-config --includes)
OBJ_DIR ?= ./build

SRC_FILES := $(wildcard $(SRC_DIR)/*.cpp)
OBJ_FILES := $(patsubst $(SRC_DIR)/%.cpp,$(OBJ_DIR)/%.o,$(SRC_FILES))
#Comment to force change in git

LDFLAGS?= -shared -llapack -lblas
CPPFLAGS ?= -pedantic -O3 -Wall -Wextra -std=c++11 -fPIC $(INC_DIRS)
		
$(OBJ_DIR)/%.o:$(SRC_DIR)/%.cpp
	# echo "For $@ This is the obj file ${SRC_FILES}"
	-echo 'Building Objects ${@}'
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c $< -o $@
$(BUILD_DIR)/$(TARGET): $(OBJ_FILES)
	-echo 'Building Targe for ${OBJ_FILES}'
	$(CXX) $(OBJ_FILES) -o $@ $(LDFLAGS)

.PHONY: clean, default, test

default: $(BUILD_DIR)/$(TARGET)

show_values:
	echo $(UNAME_S)
	echo 'Working under os: ${UNAME_S}'
	-echo "Value for INC_DIRS=${INC_DIRS}"

clean:
	rm -rf $(OBJ_DIR)/* $(BUILD_DIR)/$(TARGET) ./__pycache__ .* || true

#Pytest, there were no rules against just calling pytest
test: 
	python3 -m pytest -s
