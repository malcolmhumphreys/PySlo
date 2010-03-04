
find_package (PythonLibs ${PYTHON_VERSION} REQUIRED)

cmake_policy(SET CMP0012 OLD)
set( 3Delight_INSTALL_PATH "/Applications/Graphics/3Delight-9.0.15" )
find_package( 3Delight REQUIRED )

set (Boost_ADDITIONAL_VERSIONS "1.40" "1.39" "1.38" "1.38.0" "1.37" "1.37.0" "1.34.1" "1_34_1")
find_package (Boost 1.34 REQUIRED COMPONENTS python)

set (python_srcs PySlo.cpp)

message (STATUS "Python found ${PYTHONLIBS_FOUND} ")
message (STATUS "Python include dirs ${PYTHON_INCLUDE_PATH}")
message (STATUS "Python libraries    ${PYTHON_LIBRARIES}")

include_directories (
    ${PYTHON_INCLUDE_PATH}
    ${3Delight_INCLUDE_DIR}
    ${Boost_INCLUDE_DIRS})

add_library (PySlo SHARED ${python_srcs})

target_link_libraries (PySlo
    ${Boost_LIBRARIES}
    ${Boost_Python_LIBRARIES}
    ${PYTHON_LIBRARIES}
    ${3Delight_LIBRARIES}
    ${CMAKE_DL_LIBS})

set_target_properties (PySlo PROPERTIES
                       OUTPUT_NAME PySlo
                       PREFIX ""
                       SUFFIX ".so")

install (TARGETS PySlo DESTINATION ${CMAKE_INSTALL_PREFIX}/lib/python)
