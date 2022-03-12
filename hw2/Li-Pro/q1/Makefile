SHELL = /bin/bash

BUILD_DIR = build
OBJ_DIR = $(BUILD_DIR)/obj

CPP = $(wildcard *.cpp)
OBJ = $(addprefix $(OBJ_DIR)/, $(CPP:.cpp=.o))
EXE = $(OBJ_DIR)/hw2.exe

.PHONY: all run check clean

all: $(EXE)

run: $(EXE)
	$(EXE)

check: $(EXE)
	$(EXE) | tee result.txt

clean:
	rm -rf $(BUILD_DIR)

$(EXE): $(OBJ)
#	essentially `ld -o $@ $(OBJ)` but rely on GCC's collect utility
	g++ -o $@ $(OBJ)

$(BUILD_DIR) $(OBJ_DIR):
	mkdir -p $@

$(OBJ_DIR)/%.o: %.cpp | $(OBJ_DIR)
	g++ -c -o $@ $<
