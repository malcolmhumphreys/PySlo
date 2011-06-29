project (PySlo)

cmake_minimum_required(VERSION 2.6)

if(${PROJECT_SOURCE_DIR} STREQUAL ${CMAKE_BINARY_DIR})
  message (FATAL_ERROR "Not allowed to run in-source build!")
endif()

set(CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/share/cmake")

find_package(VFXPython REQUIRED)
if(VFXPYTHON_FOUND)
  message(STATUS "Python include dirs ${VFXPython_INCLUDE_DIR}")
  include_directories(${VFXPython_INCLUDE_DIR})
endif()

add_subdirectory(src)

install(FILES ${PROJECT_SOURCE_DIR}/lib/python/PySlo/__init__.py DESTINATION ${CMAKE_INSTALL_PREFIX}/lib/python${VFXPython_VERSION}/PySlo)

#install(DIRECTORY lib/ DESTINATION lib)
