include_directories (${NUMCPP_SOURCE_DIR}/numcpp)
#link_directories (/opt/local/lib/)

add_executable (unitTestNumcpp core.cpp base.cpp io.cpp fft.cpp solvers.cpp linalg.cpp sparse.cpp)
target_link_libraries (unitTestNumcpp ${NUMCPP_LIBS})

#add_executable (simpleTest simpleTest.cpp)
#target_link_libraries (simpleTest ${NUMCPP_LIBS})

add_executable (externTemplateMain externTemplateMain.cpp externTemplate.cpp externTemplate.h)
target_link_libraries (externTemplateMain ${NUMCPP_LIBS})

