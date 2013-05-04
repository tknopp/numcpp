add_subdirectory(core)
add_subdirectory(base)
add_subdirectory(io)
add_subdirectory(fft)
add_subdirectory(solvers)
add_subdirectory(graphics)
add_subdirectory(linalg)
add_subdirectory(sparse)

add_custom_target(NumCpp SOURCES
core.h solvers.h io.h fft.h base.h graphics.h linalg.h sparse.h)

#file (GLOB _sources RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} core/*.h)


#add_library (numcpp ${_sources})
#SET_TARGET_PROPERTIES(numcpp PROPERTIES LINKER_LANGUAGE CXX)

add_library(numcpp SHARED extern.cpp)
target_link_libraries (numcpp fftw3 fftw3f)

