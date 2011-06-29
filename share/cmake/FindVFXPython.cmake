#==========
#
# Copyright (c) 2011, Malcolm Humphreys.
# All rights reserved.
#
# Redistribution and use in source and binary forms, with or without
# modification, are permitted provided that the following conditions are
# met:
#
#     * Redistributions of source code must retain the above copyright
#       notice, this list of conditions and the following disclaimer.
#
#     * Redistributions in binary form must reproduce the above copyright
#       notice, this list of conditions and the following disclaimer in the
#       documentation and/or other materials provided with the distribution.
#
#     * Neither the name of Malcolm Humphreys nor the names of any
#       other contributors to this software may be used to endorse or
#       promote products derived from this software without specific prior
#       written permission.
#
# THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS
# IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
# THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
# PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR
# CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
# EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
# PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
# PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
# LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
# NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
# SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
#
#==========
#
# This cmake module use the same mechanism as some of the macros in the
# OpenImageIO and OpenColorIO project, but slightly modified to work as
# a find_package() module.
#
# Variables defined by this module:
#   VFXPython_FOUND
#   VFXPython_VERSION
#   VFXPython_INCLUDE_DIR
#   VFXPython_LIBRARY_DIR
#
# Usage: 
#   FIND_PACKAGE( VFXPython )
#   FIND_PACKAGE( VFXPython REQUIRED )
#
#==========

if(NOT PYTHON_EXECUTABLE)
  set(PYTHON_EXECUTABLE python)
endif()

if(CMAKE_FIRST_RUN)
  message(STATUS "Setting python executable to: ${PYTHON_EXECUTABLE}")
endif()

# find the python version
execute_process(COMMAND ${PYTHON_EXECUTABLE} -c "import distutils.sysconfig,sys; sys.stdout.write(distutils.sysconfig.get_python_version())" OUTPUT_VARIABLE VFXPython_VERSION)

# find the include dir
execute_process(COMMAND ${PYTHON_EXECUTABLE} -c "import distutils.sysconfig,sys; sys.stdout.write(distutils.sysconfig.get_python_inc())" OUTPUT_VARIABLE VFXPython_INCLUDE_DIR)

# find the library dirs
execute_process(COMMAND ${PYTHON_EXECUTABLE} -c "import distutils.sysconfig,sys; sys.stdout.write(distutils.sysconfig.get_python_lib())" OUTPUT_VARIABLE VFXPython_LIBRARY_DIR)

# did we find everything?
include(FindPackageHandleStandardArgs)
FIND_PACKAGE_HANDLE_STANDARD_ARGS("VFXPython" DEFAULT_MSG
  VFXPython_VERSION
  VFXPython_INCLUDE_DIR
  VFXPython_LIBRARY_DIR)

# try to compile something
if(VFXPYTHON_FOUND)
  try_run(VFXPython_VERSION_EXITCODE VFXPython_VERSION_COMPILED
    ${CMAKE_BINARY_DIR} ${CMAKE_MODULE_PATH}/TestPythonVersion.cxx
    CMAKE_FLAGS "-DINCLUDE_DIRECTORIES:PATH=${VFXPython_INCLUDE_DIR}"
    COMPILE_OUTPUT_VARIABLE _COMP
    RUN_OUTPUT_VARIABLE _VERSION)
  if(VFXPython_VERSION_EXITCODE EQUAL 0)
    if(NOT (VFXPython_VERSION VERSION_EQUAL _VERSION))
      message(FATAL_ERROR "The ${PYTHON_EXECUTABLE} version [${VFXPython_VERSION}] does not match the version [${_VERSION}] in Python.h from ${VFXPython_INCLUDE_DIR}")
    endif()
  else()
    message(STATUS ${_COMP})
    message(FATAL_ERROR "Could not compile simple python test app using headers from ${VFXPython_INCLUDE_DIR}")
  endif()
  unset(_VERSION)
  unset(_COMP)
endif()
