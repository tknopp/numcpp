project (NUMCPP CXX)

cmake_minimum_required(VERSION 2.8)

SET(CMAKE_BUILD_TYPE RELEASE)

SET(NUMCPP_LIBS fftw3 m blas lapack hdf5 cairo)
SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=gnu++0x -O3 -msse2  -ffast-math") # -fopenmp -ftree-vectorizer-verbose=5
SET(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})
SET(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})
add_subdirectory (numcpp)
add_subdirectory (examples)
add_subdirectory (tests)
add_subdirectory (performance)
