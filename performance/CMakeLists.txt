#include_directories (${NUMCPP_SOURCE_DIR}/numcpp /usr/local/Cellar/eigen/3.1.2/include/eigen3/)
include_directories (${NUMCPP_SOURCE_DIR}/numcpp ${NUMCPP_SOURCE_DIR}/../)
#link_directories (/opt/local/lib/)

add_executable (performance01 performance01.cpp)
target_link_libraries (performance01 ${NUMCPP_LIBS})

add_executable (performance02 performance02.cpp)
target_link_libraries (performance02 ${NUMCPP_LIBS})

add_executable (performance03 performance03.cpp)
target_link_libraries (performance03 ${NUMCPP_LIBS})

add_executable (performance04 performance04.cpp)
target_link_libraries (performance04 ${NUMCPP_LIBS})

#add_custom_target(NumCpp SOURCES performance01.jl performance01.py)

