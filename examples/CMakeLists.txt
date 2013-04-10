include_directories (${NUMCPP_SOURCE_DIR}/numcpp)
#link_directories (/opt/local/lib/)

add_executable (example01 example01.cpp)
target_link_libraries (example01 ${NUMCPP_LIBS})

add_executable (example02 example02.cpp)
target_link_libraries (example02 ${NUMCPP_LIBS})

add_executable (example03 example03.cpp)
target_link_libraries (example03 ${NUMCPP_LIBS})

add_executable (example04 example04.cpp)
target_link_libraries (example04 ${NUMCPP_LIBS})

add_executable (example05 example05.cpp)
target_link_libraries (example05 ${NUMCPP_LIBS})

add_executable (example06 example06.cpp)
target_link_libraries (example06 ${NUMCPP_LIBS})

add_executable (example07 example07.cpp)
target_link_libraries (example07 ${NUMCPP_LIBS})

#INSTALL(TARGETS example01
#        RUNTIME
#        DESTINATION bin
#        PERMISSIONS OWNER_READ OWNER_EXECUTE GROUP_READ GROUP_EXECUTE WORLD_READ #WORLD_EXECUTE)
