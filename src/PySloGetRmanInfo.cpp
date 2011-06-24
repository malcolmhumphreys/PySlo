/*
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
*/

#include <iostream>
#include <dlfcn.h>
#include <Python.h>

// RxRendererInfo function sig
typedef int (*RxRendererInfo)(const char* name, void *result,
                              int rl, int *rt, int *rc);

static PyObject*
GetRmanInfo_get_renderer_info(PyObject* self, PyObject* args) {
  char* lib;
  if(!PyArg_ParseTuple(args, "s", &lib))
    return NULL;
  // try and open the lib
  void* dlh = dlopen(lib, RTLD_LAZY|RTLD_GLOBAL);
  if (!dlh) // TODO: throw an exception
    return NULL;
  // try and find the RxRendererInfo symbol
  void* symbol = dlsym (dlh, "RxRendererInfo");
  if (!symbol) // TODO: throw an exception
    return NULL;
  // call the RxRendererInfo func symbol
  int type, count;
  RxRendererInfo func = (RxRendererInfo)symbol;
  char* renderer;
  func("renderer", &renderer, sizeof(char*), &type, &count);
  char* versionstring;
  func("versionstring", &versionstring, sizeof(char*), &type, &count);
  // return a dict of info
  PyObject* info = PyDict_New();
  PyDict_SetItem(info, PyString_FromString("renderer"),
		 PyString_FromString(renderer));
  PyDict_SetItem(info, PyString_FromString("versionstring"),
		 PyString_FromString(versionstring));
  // close the dlopen handle
  if(dlclose(dlh))
    return NULL;  // TODO: throw an exception
  return info;
}

static PyMethodDef
GetRmanInfo_methods[] = {
  {"get_renderer_info",
   (PyCFunction)GetRmanInfo_get_renderer_info,
   METH_VARARGS,
   ""},
  {NULL, NULL, 0, NULL}
};

extern "C"
PyMODINIT_FUNC
initGetRmanInfo(void)
{
  PyObject *m;
  m = Py_InitModule3("GetRmanInfo", GetRmanInfo_methods,
		     "PySlo renderman info module");
  if (m == NULL) return;
}
