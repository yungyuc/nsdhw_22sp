TARGETNAME = _vector
TARGET = $(addprefix $(TARGETNAME),$(shell python3-config --extension-suffix))
#Comment
BUILD_DIR ?= ./
SRC_DIR ?= ./
# Since we will be using pybind as header library
INC_DIRS := -I./include $(shell python3-config --includes)
OBJ_DIR ?= ./
#Comment to force change in git

LDFLAGS?=
CPPFLAGS ?= -O3 -Wall -shared -std=c++11 -fPIC $(INC_DIRS)
		
$(BUILD_DIR)/$(TARGET): $(SRC_DIR)/main.cpp
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) $< -o $@ $(LDFLAGS)

.PHONY: clean, run, default
default: $(BUILD_DIR)/$(TARGET)

clean:
	rm -rf $(OBJ_DIR)/*.so $(BUILD_DIR)/$(TARGET) ./__pycache__ .* || true

#Pytest, there were no rules against just calling pytest
test: 
	python3 -m pytest -s
