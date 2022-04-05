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
	INC_DIRS := -I./include $(shell python3-config --includes) $(shell python3 -m pybind11 --includes) -I/opt/intel/mkl/include
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

LDFLAGS?= -shared -mkl 
CPPFLAGS ?= -pedantic -O3 -Wall -Wextra -std=c++11 -MMD -MP -fPIC $(INC_DIRS)
		
$(OBJ_DIR)/%.o:$(SRC_DIR)/%.cpp
# echo "For $@ This is the obj file ${SRC_FILES}"
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c $< -o $@


$(BUILD_DIR)/$(TARGET): $(OBJ_FILES)
	@echo 'Building Targe for ${OBJ_FILES}'
	$(CXX) $(OBJ_FILES) -o $@ $(LDFLAGS)

.PHONY: clean, default, test, foundation

foundation: $(BUILD_DIR) $(SRC_DIR) $(OBJ_DIR) $(DEPS_DIR)
	@echo 'Directory Created'

$(BUILD_DIR) $(SRC_DIR) $(OBJ_DIR): #$(DEPS_DIR):
	mkdir $@

-include $(DEPENDS)

default: $(BUILD_DIR)/$(TARGET)

show_values:
	echo $(UNAME_S)
	echo 'Working under os: ${UNAME_S}'
	@echo "Value for INC_DIRS=${INC_DIRS}"

clean:
	rm -rf $(OBJ_DIR)/* $(BUILD_DIR)/* ./__pycache__ .* || true
	#rm -rf $(OBJ_DIR)/* $(BUILD_DIR)/$(TARGET) ./__pycache__ .* || true

test:
	python3 $(TESTS_DIR)/unit_test.py
