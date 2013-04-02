project (NUMCPP CXX)

cmake_minimum_required(VERSION 2.8)

SET(CMAKE_BUILD_TYPE RELEASE)

SET(NUMCPP_LIBS fftw3 m blas lapack hdf5 cairo)
SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=gnu++0x -O3 -msse2  -ffast-math") # -fopenmp -ftree-vectorizer-verbose=5
SET(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})
SET(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})

#INCLUDE (CheckIncludeFiles)
# usage: CHECK_INCLUDE_FILES (<header> <RESULT_VARIABLE> )

#CHECK_INCLUDE_FILES (malloc.h HAVE_MALLOC_H)
#CONFIGURE_FILE(${CMAKE_CURRENT_SOURCE_DIR}/config.h.in ${CMAKE_CURRENT_BINARY_DIR}/config.h)

#find_package (CLAPACK)
#if (CLAPACK_FOUND)
#  include_directories(${BZIP_INCLUDE_DIRS})
#  target_link_libraries (helloworld ${BZIP2_LIBRARIES})
#endif (CLAPACK_FOUND)


add_subdirectory (numcpp)
add_subdirectory (examples)
add_subdirectory (tests)
add_subdirectory (performance)
