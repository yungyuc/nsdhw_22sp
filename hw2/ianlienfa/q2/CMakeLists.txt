cmake_minimum_required(VERSION 3.22)
project(VectorProject)

# add_subdirectory(pybind11)
pybind11_add_module(_vector _vector.cpp)

add_custom_target(
    test
    COMMAND make all && pytest test.py 
)

set(CLEAN_FILES __pycache__ .pytest_cache)
set_directory_properties(
    PROPERTIES
    ADDITIONAL_CLEAN_FILES CLEAN_FILES
)

