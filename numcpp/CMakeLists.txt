add_subdirectory(core)
add_subdirectory(base)
add_subdirectory(io)
add_subdirectory(transformations)
add_subdirectory(solvers)

add_custom_target(NumCpp SOURCES numcpp.h
core.h solvers.h io.h transformations.h base.h)

#file (GLOB _sources RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} core/*.h)


#add_library (numcpp ${_sources})
#SET_TARGET_PROPERTIES(numcpp PROPERTIES LINKER_LANGUAGE CXX)

