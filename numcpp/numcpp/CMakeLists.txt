add_subdirectory(core)
add_subdirectory(expressions)
add_subdirectory(base)
add_subdirectory(io)
add_subdirectory(fft)
add_subdirectory(wavelet)
add_subdirectory(solvers)
add_subdirectory(graphics)
add_subdirectory(linalg)
add_subdirectory(sparse)
add_subdirectory(tomography)

add_custom_target(NumCpp SOURCES
config.h core.h solvers.h io.h fft.h base.h graphics.h linalg.h sparse.h tomography.h expressions.h wavelet.h)

#file (GLOB _sources RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} core/*.h)


#add_library (numcpp ${_sources})
#SET_TARGET_PROPERTIES(numcpp PROPERTIES LINKER_LANGUAGE CXX)

add_library(numcpp SHARED numcpp.cpp)
target_link_libraries (numcpp fftw3 fftw3f hdf5)

