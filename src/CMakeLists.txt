
find_package (PythonLibs ${PYTHON_VERSION} REQUIRED)

# try to find an Rman lib (set by the RMAN envvar)
set( RMAN "Delight" )
if ( NOT $ENV{RMAN} STREQUAL "" )
  set( RMAN $ENV{RMAN} )
endif ( NOT $ENV{RMAN} STREQUAL "" )
if ( ${RMAN} STREQUAL "3Delight" ) # Remap 3Delight->Delight
  set( RMAN "Delight" )
endif( ${RMAN} STREQUAL "3Delight" ) 
find_package( ${RMAN} REQUIRED )

set (Boost_ADDITIONAL_VERSIONS "1.40" "1.39" "1.38" "1.38.0" "1.37" "1.37.0" "1.34.1" "1_34_1")
find_package (Boost 1.34 REQUIRED COMPONENTS python)

set (python_srcs PySlo.cpp)

message (STATUS "Python found ${PYTHONLIBS_FOUND} ")
message (STATUS "Python include dirs ${PYTHON_INCLUDE_PATH}")
message (STATUS "Python libraries    ${PYTHON_LIBRARIES}")

include_directories (
    ${PYTHON_INCLUDE_PATH}
    ${${RMAN}_INCLUDE_DIR}
    ${Boost_INCLUDE_DIRS}
    )

add_library (PySlo SHARED ${python_srcs})

target_link_libraries (PySlo
    ${Boost_LIBRARIES}
    ${Boost_Python_LIBRARIES}
    ${PYTHON_LIBRARIES}
    ${${RMAN}_LIBRARIES}
    ${CMAKE_DL_LIBS}
    )

set_target_properties (PySlo PROPERTIES
                       OUTPUT_NAME PySlo
                       PREFIX ""
                       SUFFIX ".so"
                       COMPILE_FLAGS ${${RMAN}_COMPILE_FLAGS})

install (TARGETS PySlo DESTINATION ${CMAKE_INSTALL_PREFIX}/lib/python)
