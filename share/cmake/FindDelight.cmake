#==========
#
# Copyright (c) 2010, Dan Bethell.
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
#     * Neither the name of Dan Bethell nor the names of any
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
# Variables defined by this module:
#   Delight_FOUND    
#   Delight_INCLUDE_DIR
#   Delight_COMPILE_FLAGS
#   Delight_LIBRARIES
#   Delight_LIBRARY_DIR
#
# Usage: 
#   FIND_PACKAGE( Delight )
#   FIND_PACKAGE( Delight REQUIRED )
#
# Note:
# You can tell the module where Delight is installed by setting
# the Delight_INSTALL_PATH (or setting the DELIGHT environment
# variable before calling FIND_PACKAGE.
# 
# E.g. 
#   SET( Delight_INSTALL_PATH "/usr/local/3delight-9.0.0/Linux-x86_64" )
#   FIND_PACKAGE( Delight REQUIRED )
#
#==========

# our includes
FIND_PATH( Delight_INCLUDE_DIR ri.h
  ${Delight_INSTALL_PATH}/include
  $ENV{DELIGHT}/include )

# our compilation flags
SET( Delight_COMPILE_FLAGS "-DDELIGHT -I${Delight_INCLUDE_DIR}" )

# our library itself
FIND_LIBRARY( Delight_LIBRARIES 3delight
  ${Delight_INSTALL_PATH}/lib
  $ENV{DELIGHT}/lib )

# our library path
GET_FILENAME_COMPONENT( Delight_LIBRARY_DIR ${Delight_LIBRARIES} PATH )

# did we find everything?
INCLUDE( FindPackageHandleStandardArgs )
FIND_PACKAGE_HANDLE_STANDARD_ARGS( "Delight" DEFAULT_MSG
  Delight_INCLUDE_DIR
  Delight_COMPILE_FLAGS
  Delight_LIBRARIES
  Delight_LIBRARY_DIR )

SET( Delight_VERSION "0.0.0" )
SET( Delight_ABI "0.0" )
IF( DELIGHT_FOUND )
  TRY_RUN(DELIGHT_VERSION_EXITCODE DELIGHT_VERSION_COMPILED
    ${CMAKE_BINARY_DIR} ${CMAKE_MODULE_PATH}/TestForRmanVersion.cxx
    CMAKE_FLAGS "-DLINK_LIBRARIES:STRING=dl"
    RUN_OUTPUT_VARIABLE Delight_VERSION
    ARGS ${Delight_LIBRARIES})
  IF(DELIGHT_VERSION_COMPILED)
    string(REGEX MATCH "^[^.]*.[^.]*" Delight_ABI ${Delight_VERSION})
    string(REPLACE "." "_" Delight_ABI ${Delight_ABI})
    message(STATUS "Delight ABI: ${Delight_ABI} Version: ${Delight_VERSION}")
  ENDIF()
ENDIF()
