
set(RMAN_TARGETS)

find_package(PRMan)
if(PRMAN_FOUND)
  set(RMAN_TARGETS ${RMAN_TARGETS} PRMan)
endif(PRMAN_FOUND)

find_package(Delight)
if(DELIGHT_FOUND)
  set(RMAN_TARGETS ${RMAN_TARGETS} Delight)
endif(DELIGHT_FOUND)

find_package(Aqsis)
if(AQSIS_FOUND)
  set(RMAN_TARGETS ${RMAN_TARGETS} Aqsis)
else(AQSIS_FOUND)
  message(STATUS "Try setting AQSISHOME to find Aqsis")
endif(AQSIS_FOUND)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-strict-aliasing")
if(APPLE)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -undefined dynamic_lookup")
endif()

add_library(GetRmanInfo SHARED PySloGetRmanInfo.cpp)
target_link_libraries(GetRmanInfo ${CMAKE_DL_LIBS})
set_target_properties(GetRmanInfo PROPERTIES
  OUTPUT_NAME "GetRmanInfo"
  LIBRARY_OUTPUT_DIRECTORY "BuildGetRmanInfoPy${VFXPython_VERSION}"
  PREFIX "" SUFFIX ".so")
install(TARGETS GetRmanInfo
  DESTINATION ${CMAKE_INSTALL_PREFIX}/lib/python${VFXPython_VERSION}/PySlo)

foreach(RMAN ${RMAN_TARGETS})
  set(RMAN_TARGET ${RMAN}${${RMAN}_ABI})
  add_library(${RMAN_TARGET} SHARED PySloModule.cpp)
  target_link_libraries(${RMAN_TARGET}
    ${${RMAN}_LIBRARIES}
    ${CMAKE_DL_LIBS})
  set_target_properties(${RMAN_TARGET} PROPERTIES
    OUTPUT_NAME __${RMAN}__
    LIBRARY_OUTPUT_DIRECTORY "Build${RMAN}${${RMAN}_ABI}Py${VFXPython_VERSION}"
    PREFIX "" SUFFIX ".so"
    COMPILE_FLAGS ${${RMAN}_COMPILE_FLAGS}
    INSTALL_RPATH ${${RMAN}_LIBRARY_DIR})
  install(TARGETS ${RMAN_TARGET}
    DESTINATION ${CMAKE_INSTALL_PREFIX}/lib/python${VFXPython_VERSION}/PySlo/${RMAN_TARGET})
  configure_file(${CMAKE_SOURCE_DIR}/src/__init__.py.in
    ${CMAKE_CURRENT_BINARY_DIR}/${RMAN}__init__.py @ONLY)
  install(FILES ${CMAKE_CURRENT_BINARY_DIR}/${RMAN}__init__.py
    DESTINATION ${CMAKE_INSTALL_PREFIX}/lib/python${VFXPython_VERSION}/PySlo/${RMAN_TARGET}
    RENAME __init__.py)
endforeach()
