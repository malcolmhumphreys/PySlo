
set(RMAN_TARGETS)

find_package(PythonLibs ${PYTHON_VERSION} REQUIRED)
message(STATUS "Python found ${PYTHONLIBS_FOUND} ")
message(STATUS "Python include dirs ${PYTHON_INCLUDE_PATH}")
message(STATUS "Python libraries ${PYTHON_LIBRARIES}")
include_directories(${PYTHON_INCLUDE_PATH})

find_package(PRMan)
if(PRMAN_FOUND)
  include_directories(${PRMan_INCLUDE_DIR})
  set(RMAN_TARGETS ${RMAN_TARGETS} PRMan)
endif(PRMAN_FOUND)

find_package(Delight)
if(DELIGHT_FOUND)
  include_directories (${Delight_INCLUDE_DIR})
  set(RMAN_TARGETS ${RMAN_TARGETS} Delight)
endif(DELIGHT_FOUND)

find_package(Aqsis)
if(AQSIS_FOUND)
  include_directories (${Aqsis_INCLUDE_DIR})
  set(RMAN_TARGETS ${RMAN_TARGETS} Aqsis)
else(AQSIS_FOUND)
  message(STATUS "Try setting AQSISHOME to find Aqsis")
endif(AQSIS_FOUND)

set(python_srcs PySloModule.cpp)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-strict-aliasing")
if(APPLE)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -undefined dynamic_lookup")
endif()

foreach(RMAN ${RMAN_TARGETS})
  set(RMAN_TARGET ${RMAN}${${RMAN}_ABI})
  add_library(${RMAN_TARGET} SHARED ${python_srcs})
  target_link_libraries(${RMAN_TARGET}
    ${${RMAN}_LIBRARIES}
    ${CMAKE_DL_LIBS})
  set_target_properties(${RMAN_TARGET} PROPERTIES
                        OUTPUT_NAME __${RMAN}__
                        PREFIX "" SUFFIX ".so"
                        COMPILE_FLAGS ${${RMAN}_COMPILE_FLAGS})
  install(TARGETS ${RMAN_TARGET}
          DESTINATION ${CMAKE_INSTALL_PREFIX}/lib/python/PySlo/${RMAN_TARGET})
  configure_file(${CMAKE_SOURCE_DIR}/src/__init__.py.in
                 ${CMAKE_CURRENT_BINARY_DIR}/${RMAN}__init__.py @ONLY)
  install(FILES ${CMAKE_CURRENT_BINARY_DIR}/${RMAN}__init__.py
          DESTINATION ${CMAKE_INSTALL_PREFIX}/lib/python/PySlo/${RMAN_TARGET}
          RENAME __init__.py)
endforeach()
