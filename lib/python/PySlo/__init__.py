"""
Copyright (c) 2010, Malcolm Humphreys.
All rights reserved.
 
Redistribution and use in source and binary forms, with or without modification,
are permitted provided that the following conditions are met:
 
* Redistributions of source code must retain the above copyright notice,
this list of conditions and the following disclaimer.
 
* Redistributions in binary form must reproduce the above copyright notice,
this list of conditions and the following disclaimer in the documentation
and/or other materials provided with the distribution.
 
* Neither the names of PySlo, nor the names of its contributors may be used to
endorse or promote products derived from this software without specific prior
written permission.
 
THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR
ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
(INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
"""

import os
import os.path
import sys
import platform
import GetRmanInfo

__binding__ = None
__searchpath__ = None
__loaded_shader__ = None
__rman_modules__ = { }
__dso_suffix__ = ".dso"
if platform.system() == "Darwin":
  __dso_suffix__ = ".dylib"
else:
  raise Exception("PySlo doesn't support platform %s" % str(platform.system()))

# enum
TYPE = None
STORAGE = None
DETAIL = None

def __setup_binding__(rootpath, rman, rmanext, renderer, libname):
  global __rman_modules__
  __rman_modules__[rmanext] = { "rman": rman, "module": None, "loadable": False }
  if rootpath != None:
    lib = os.path.join(rootpath, "lib", "%s%s" % (libname, __dso_suffix__))
    if not os.path.exists(lib):
      return False
    info = None
    try:
      info = GetRmanInfo.get_renderer_info(lib)
    except:
      return False
    if info != None:
      if info["renderer"] == renderer:
        abi = info["versionstring"].split('.')
        abi = "%s.%s" % (abi[0], abi[1])
        modulename = "PySlo.%s%s" % (rman, abi.replace(".", "_"))
        try:
          __test_import = __import__(modulename)
          del __test_import
          __rman_modules__[rmanext]["module"] = modulename
          __rman_modules__[rmanext]["loadable"] = True
          return True
        except:
          return False
  return False

# setup the bindings
__setup_binding__(os.getenv('RMANTREE', None), "PRMan", ".slo",
                  "PhotoRealistic-RenderMan", "libprman")
__setup_binding__(os.getenv('DELIGHT', None), "Delight", ".sdl",
                  "3Delight", "lib3delight")
__setup_binding__(os.getenv('AQSISHOME', None), "Aqsis", ".slx",
                  "Aqsis", "libaqsis")

###############################################################################

def setPath(searchpath = ".:&"):
  global __searchpath__
  __searchpath__ = searchpath

def getPath():
  global __searchpath__
  return __searchpath__

def setShader(shader = None):
  global __binding__, __searchpath__, __loaded_shader__, __rman_modules__
  global TYPE, STORAGE, DETAIL
  # get the extension of the shader
  basename, ext = os.path.splitext(str(shader))
  # unload current binding if it does not match shader type
  if __binding__ != None:
    if ext != __rman_modules__[ext]["rman"]:
      del __binding__
      __binding__ = None
  # load binding if we haven't allready
  if __binding__ == None:
    if ext in __rman_modules__.keys():
      if not __rman_modules__[ext]["loadable"]:
        raise Exception('binding for %s not compiled' % __rman_modules__[ext]["rman"])
      try:
        __import__(__rman_modules__[ext]["module"])
        __binding__ = sys.modules[__rman_modules__[ext]["module"]]
      except ImportError:
        raise Exception('could not import module %s' % __rman_modules__[ext]["module"])
    else:
      raise Exception('unsupported shader type %s' % ext)
    # update the enum pointers
    TYPE    = __binding__.TYPE
    STORAGE = __binding__.STORAGE
    DETAIL  = __binding__.DETAIL
  # load the shader
  if __searchpath__ != None:
    __binding__.setPath(__searchpath__)
  __loaded_shader__ = __binding__.setShader(basename)
  if __loaded_shader__ == 1:
    raise Exception('could not load/find shader %s' % shader)

def getName():
  if __binding__ == None or __loaded_shader__ == 1:
    raise Exception('you need call setShader() before getName()')
  return __binding__.getName()

def getType():
  if __binding__ == None or __loaded_shader__ == 1:
    raise Exception('you need call setShader() before getType()')
  return __binding__.getType()

def hasMethod(name = ""):
  if __binding__ == None or __loaded_shader__ == 1:
    raise Exception('you need call setShader() before hasMethod()')
  return __binding__.hasMethod(name)

def getMethodNames():
  if __binding__ == None or __loaded_shader__ == 1:
    raise Exception('you need call setShader() before getMethodNames()')
  return __binding__.getMethodNames()

def getNArgs():
  if __binding__ == None or __loaded_shader__ == 1:
    raise Exception('you need call setShader() before getNArgs()')
  return __binding__.getNArgs()

def getArgById(id = 0):
  if __binding__ == None or __loaded_shader__ == 1:
    raise Exception('you need call setShader() before getArgById()')
  return __binding__.getArgById(id)

def getArgByName(name = ""):
  if __binding__ == None or __loaded_shader__ == 1:
    raise Exception('you need call setShader() before getArgByName()')
  return __binding__.getArgByName(name)

def getArrayArgElementById(id = 0):
  if __binding__ == None or __loaded_shader__ == 1:
    raise Exception('you need call setShader() before getArrayArgElementById()')
  return __binding__.getArrayArgElementById(id)

def getArrayArgElementByName(name = ""):
  if __binding__ == None or __loaded_shader__ == 1:
    raise Exception('you need call setShader() before getArrayArgElementByName()')
  return __binding__.getArrayArgElementByName(name)

def endShader():
  global __binding__, __loaded_shader__
  if __binding__ == None:
    raise Exception('you need call setShader() before endShader()')
  __binding__.endShader()
  __binding__ = None
  __loaded_shader__ = None

def typetoStr(type = None):
  if __binding__ == None:
    raise Exception('you need call setShader() before typetoStr()')
  return __binding__.typetoStr(type)

def stortoStr(storage = None):
  if __binding__ == None:
    raise Exception('you need call setShader() before stortoStr()')
  return __binding__.stortoStr(storage)

def detailtoStr(detail = None):
  if __binding__ == None:
    raise Exception('you need call setShader() before detailtoStr()')
  return __binding__.detailtoStr(detail)

def getMetaData(name = None):
  if __binding__ == None:
    raise Exception('you need call setShader() before getMetaData()')
  return __binding__.getMetaData(name)

def getAllMetaData():
  if __binding__ == None:
    raise Exception('you need call setShader() before getAllMetaData()')
  return __binding__.getAllMetaData()

def getNAnnotations():
  if __binding__ == None:
    raise Exception('you need call setShader() before getNAnnotations()')
  return __binding__.getNAnnotations()

def getAnnotationKeyById(id = 0):
  if __binding__ == None:
    raise Exception('you need call setShader() before getAnnotationKeyById()')
  return __binding__.getAnnotationKeyById(id)

def getAnnotationByKey(key = None):
  if __binding__ == None:
    raise Exception('you need call setShader() before getAnnotationByKey()')
  return __binding__.getAnnotationByKey(key)

def getPluginsNames():
  if __binding__ == None:
    raise Exception('you need call setShader() before getPluginsNames()')
  return __binding__.getPluginsNames()

def isArray(id = 0):
  if __binding__ == None:
    raise Exception('you need call setShader() before isArray()')
  return __binding__.isArray(id)

def rmanType():
  if __binding__ == None:
    return "Unknown"
  return __binding__.rmanType()
