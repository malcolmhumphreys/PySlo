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

__binding__ = None
__searchpath__ = None
__loaded_shader__ = None

# enums
type = None
storage = None
detail = None

def setPath(searchpath = ".:&"):
  global __searchpath__
  __searchpath__ = searchpath

def setShader(shader = None):
  global __binding__, __searchpath__, __loaded_shader__, type, storage, detail
  if __binding__ == None:
    #if ext prman
    #if ext sdl
    import __Delight__ as __binding__
    #if ext aqsis
    # update the enum pointers
    type    = __binding__.type
    storage = __binding__.storage
    detail  = __binding__.detail
  if __searchpath__ != None:
    __binding__.setPath(__searchpath__)
  __loaded_shader__ = __binding__.setShader(shader)
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

def typetoStr(type = None):
  return __binding__.typetoStr(type)

def stortoStr(storage = None):
  return __binding__.stortoStr(storage)

def detailtoStr(detail = None):
  return __binding__.detailtoStr(detail)

def endShader():
  global __binding__, __loaded_shader__
  if __binding__ == None:
    raise Exception('you need call setShader() before endShader()')
  __binding__.endShader()
  __binding__ = None
  __loaded_shader__ = None

"""
#ifdef DELIGHT
 def("getNAnnotations", Slo_GetNAnnotations);
 def("getAnnotationKeyById", Slo_GetAnnotationKeyById);
 def("getAnnotationByKey", Slo_GetAnnotationByKey);
#endif // DELIGHT
#ifdef PRMAN
 // def("getMetaData", Slo_GetMetaData ); // TODO
 // def("getAllMetaData", Slo_GetAllMetaData ); // TODO
#endif // PRMAN
"""

def isArray(id = 0):
  return __binding__.isArray(id)
