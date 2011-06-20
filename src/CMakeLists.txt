
find_package (PythonLibs ${PYTHON_VERSION} REQUIRED)

set (RMAN_TARGETS)

find_package (PRMan)
if (PRMAN_FOUND)
    include_directories (${PRMan_INCLUDE_DIR})
    set (RMAN_TARGETS ${RMAN_TARGETS} PRMan)
endif (PRMAN_FOUND)

find_package (Delight)
if (DELIGHT_FOUND)
    include_directories (${Delight_INCLUDE_DIR})
    set(RMAN_TARGETS ${RMAN_TARGETS} Delight)
endif (DELIGHT_FOUND)

find_package (Aqsis)
if (AQSIS_FOUND)
    include_directories (${Aqsis_INCLUDE_DIR})
    set (RMAN_TARGETS ${RMAN_TARGETS} Aqsis)
else (AQSIS_FOUND)
    message (STATUS "Try setting AQSISHOME to find Aqsis")
endif (AQSIS_FOUND)

message (STATUS "Python found ${PYTHONLIBS_FOUND} ")
message (STATUS "Python include dirs ${PYTHON_INCLUDE_PATH}")
message (STATUS "Python libraries ${PYTHON_LIBRARIES}")
include_directories (${PYTHON_INCLUDE_PATH})

set (python_srcs PySloModule.cpp)

foreach (RMAN ${RMAN_TARGETS})
    set (RMAN_TARGET __${RMAN}__)
    add_library (${RMAN_TARGET} SHARED ${python_srcs})
    target_link_libraries (${RMAN_TARGET}
        ${PYTHON_LIBRARIES}
        ${${RMAN}_LIBRARIES}
        ${CMAKE_DL_LIBS})
    set_target_properties (${RMAN_TARGET} PROPERTIES
                           OUTPUT_NAME ${RMAN_TARGET}
                           PREFIX ""
                           SUFFIX ".so"
                           COMPILE_FLAGS ${${RMAN}_COMPILE_FLAGS})
    install (TARGETS ${RMAN_TARGET}
        DESTINATION ${CMAKE_INSTALL_PREFIX}/lib/python/PySlo)
endforeach ()
