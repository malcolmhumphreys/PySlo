#==========
#
# Copyright (c) 2010, Malcolm Humphreys.
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
# Variables defined by this module:
#   Aqsis_FOUND    
#   Aqsis_INCLUDE_DIR
#   Aqsis_COMPILE_FLAGS
#   Aqsis_LIBRARIES
#   Aqsis_LIBRARY_DIR
#
# Usage: 
#   FIND_PACKAGE( Aqsis )
#   FIND_PACKAGE( Aqsis REQUIRED )
#
# Note:
# You can tell the module where Aqsis is installed by setting
# the Aqsis_INSTALL_PATH (or setting the AQSISHOME environment
# variable before calling FIND_PACKAGE.
# 
# E.g. 
#   SET( Aqsis_INSTALL_PATH "/Applications/Aqsis.app/Contents/Resources" )
#   FIND_PACKAGE( Aqsis REQUIRED )
#
#==========

# our includes
FIND_PATH( Aqsis_ROOT_INCLUDE_DIR aqsis
  ${Aqsis_INSTALL_PATH}/include
  $ENV{AQSISHOME}/include )
FIND_PATH( Aqsis_RI_INCLUDE_DIR ri.h
  ${Aqsis_INSTALL_PATH}/include/aqsis/ri
  $ENV{AQSISHOME}/include/aqsis/ri )
SET( Aqsis_INCLUDE_DIR ${Aqsis_ROOT_INCLUDE_DIR} ${Aqsis_RI_INCLUDE_DIR} )

# our compilation flags
SET( Aqsis_COMPILE_FLAGS "-DAQSIS -I${Aqsis_ROOT_INCLUDE_DIR} -I${Aqsis_RI_INCLUDE_DIR}" )

# our library itself
FIND_LIBRARY( Aqsis_LIBRARIES aqsis_slxargs
  ${Aqsis_INSTALL_PATH}/lib
  $ENV{AQSISHOME}/lib )

# our library path
GET_FILENAME_COMPONENT( Aqsis_LIBRARY_DIR ${Aqsis_LIBRARIES} PATH )

# did we find everything?
INCLUDE( FindPackageHandleStandardArgs )
FIND_PACKAGE_HANDLE_STANDARD_ARGS( "Aqsis" DEFAULT_MSG
  Aqsis_INCLUDE_DIR
  Aqsis_COMPILE_FLAGS
  Aqsis_LIBRARIES
  Aqsis_LIBRARY_DIR )

SET( Aqsis_VERSION "1.6" )
SET( Aqsis_ABI "1_6" )
# TODO: get Aqsis to support RxRendererInfo so this works
#IF( AQSIS_FOUND )
#  TRY_RUN(AQSIS_VERSION_EXITCODE AQSIS_VERSION_COMPILED
#    ${CMAKE_BINARY_DIR} ${CMAKE_MODULE_PATH}/TestForRmanVersion.cxx
#    CMAKE_FLAGS "-DLINK_LIBRARIES:STRING=dl"
#    RUN_OUTPUT_VARIABLE Aqsis_VERSION
#    ARGS ${Aqsis_LIBRARIES})
#  IF(AQSIS_VERSION_COMPILED)
#    string(REGEX MATCH "^[^.]*.[^.]*" Aqsis_ABI ${Aqsis_VERSION})
#    string(REPLACE "." "_" Aqsis_ABI ${PRMan_ABI})
#    message(STATUS "Aqsis ABI: ${Aqsis_ABI} Aqsis: ${Delight_VERSION}")
#  ENDIF()
#ENDIF()
