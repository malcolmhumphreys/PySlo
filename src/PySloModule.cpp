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

#include <string>
#include <iostream>
#include <Python.h>
#include <slo.h>

static const std::string
PyRmanType() {
#ifdef PRMAN
  return std::string("PRMan");
#elif DELIGHT
  return std::string("Delight");
#elif AQSIS
  return std::string("Aqsis");
#else
  return std::string("Unknown");
#endif
}

void
PyGetArgDefault(SLO_VISSYMDEF *arg, PyObject* list) {
    if (arg == NULL) return;
    PyObject* tmp = PyList_New(0);
    switch (arg->svd_type) {
      case SLO_TYPE_POINT:
      case SLO_TYPE_COLOR:
      case SLO_TYPE_VECTOR:
      case SLO_TYPE_NORMAL:
        PyList_Append(tmp,
          PyFloat_FromDouble(arg->svd_default.pointval[0].xval));
        PyList_Append(tmp,
          PyFloat_FromDouble(arg->svd_default.pointval[0].yval));
        PyList_Append(tmp,
          PyFloat_FromDouble(arg->svd_default.pointval[0].zval));
        PyList_Append(list, tmp);
        break;
      case SLO_TYPE_SCALAR:
        PyList_Append(list,
          PyFloat_FromDouble(arg->svd_default.scalarval[0]));
        break;
      case SLO_TYPE_STRING:
        PyList_Append(list,
          PyString_FromString(arg->svd_default.stringval));
        break;
      case SLO_TYPE_MATRIX:
        for(unsigned m = 0; m < 16; ++m)
          PyList_Append(tmp,
            PyFloat_FromDouble(arg->svd_default.matrixval[m]));
        PyList_Append(list, tmp);
        break;
    }
    return;
}

void
PyGetArrayArgDefault(SLO_VISSYMDEF *arg, PyObject* list) {
    if (arg == NULL) return;
    for (int i = 0; i < arg->svd_arraylen; i++) 
      PyGetArgDefault(Slo_GetArrayArgElement (arg, i), list);
    return;
}

void
PyConvertVISSYMDEF(SLO_VISSYMDEF* arg, PyObject* dict) {
    if (arg == NULL || dict == NULL) return;
    PyDict_SetItem(dict, PyString_FromString("name"),
      PyString_FromString(arg->svd_name));
    PyDict_SetItem(dict, PyString_FromString("type"),
      PyInt_FromLong(arg->svd_type));
    PyDict_SetItem(dict, PyString_FromString("storage"),
      PyInt_FromLong(arg->svd_storage));
    PyDict_SetItem(dict, PyString_FromString("detail"),
      PyInt_FromLong(arg->svd_detail));
    PyDict_SetItem(dict, PyString_FromString("spacename"),
      PyString_FromString(arg->svd_spacename));
    PyDict_SetItem(dict, PyString_FromString("valisvalid"),
      PyInt_FromLong(arg->svd_valisvalid));
    PyDict_SetItem(dict, PyString_FromString("arraylen"),
      PyInt_FromLong(arg->svd_arraylen));
    PyObject* list = PyList_New(0);
    if (arg->svd_arraylen > 0 || arg->svd_default.scalarval == 0x0)
      PyGetArrayArgDefault(arg, list);
    else
      PyGetArgDefault(arg, list);
    PyDict_SetItem(dict, PyString_FromString("default"), list);
    return;
}

static PyObject*
PySlo_setPath(PyObject *self, PyObject *args, PyObject *keywds) {
  char* path = NULL;
  const char *kwlist[] = {"path", NULL};
  if (!PyArg_ParseTupleAndKeywords(args, keywds, "s",
                                   const_cast<char **>(kwlist),
                                   &path))
    return NULL;
  Slo_SetPath(path);
  Py_RETURN_NONE;
}

static PyObject*
PySlo_getPath(PyObject* self, PyObject* args) {
#ifdef PRMAN
  const char* path = Slo_GetPath();
  if(path != NULL)
    return PyString_FromString(path);
#endif
  Py_RETURN_NONE;
}

static PyObject*
PySlo_setShader(PyObject *self, PyObject *args, PyObject *keywds) {
  char* name = NULL;
  const char *kwlist[] = {"name", NULL};
  if (!PyArg_ParseTupleAndKeywords(args, keywds, "s",
                                   const_cast<char **>(kwlist),
                                   &name))
    return NULL;
  Slo_SetShader(name);
  Py_RETURN_NONE;
}

static PyObject*
PySlo_getName(PyObject* self, PyObject *args) {
  const char* name = Slo_GetName();
  if(name != NULL)
    return PyString_FromString(name);
  Py_RETURN_NONE;
}

static PyObject*
PySlo_getType(PyObject* self, PyObject *args) {
  return PyInt_FromLong(Slo_GetType());
}

static PyObject*
PySlo_hasMethod(PyObject* self, PyObject* args, PyObject *keywds) {
  char* name = NULL;
  const char *kwlist[] = {"name", NULL};
  if (!PyArg_ParseTupleAndKeywords(args, keywds, "s",
                                   const_cast<char **>(kwlist),
                                   &name))
    return NULL;
  return PyBool_FromLong(Slo_HasMethod(name));
}

static PyObject*
PySlo_getMethodNames(PyObject* self) {
  PyObject* methods = PyList_New(0);
  if(!methods) return 0;
  const char* const* ptr = Slo_GetMethodNames();
  if(ptr != NULL) {
    for (; *ptr != '\0'; ptr++) {
      PyObject *name = PyString_FromString(*ptr);
      PyList_Append(methods, name);
    }
  }
  return methods;
}

static PyObject*
PySlo_getNArgs(PyObject* self, PyObject *args) {
    return PyInt_FromLong(Slo_GetNArgs());
}

static PyObject*
PySlo_getArgById(PyObject* self, PyObject* args, PyObject *keywds) {
  int id = -1;
  const char *kwlist[] = {"id", NULL};
  if (!PyArg_ParseTupleAndKeywords(args, keywds, "i",
                                   const_cast<char **>(kwlist),
                                   &id))
    return NULL;
  PyObject* arg = PyDict_New();
  SLO_VISSYMDEF* arg_ptr = Slo_GetArgById(id);
  PyConvertVISSYMDEF(arg_ptr, arg);
  return arg;
}

static PyObject*
PySlo_getArgByName(PyObject* self, PyObject* args, PyObject *keywds) {
  char* name = NULL;
  const char *kwlist[] = {"name", NULL};
  if (!PyArg_ParseTupleAndKeywords(args, keywds, "s",
                                   const_cast<char **>(kwlist),
                                   &name))
    return NULL;
  PyObject* arg = PyDict_New();
  SLO_VISSYMDEF* arg_ptr = Slo_GetArgByName(name);
  PyConvertVISSYMDEF(arg_ptr, arg);
  return arg;
}

static PyObject*
PySlo_getArrayArgElementById(PyObject* self, PyObject* args, PyObject *keywds) {
  int id = -1;
  int index = -1;
  const char *kwlist[] = {"id", "index", NULL};
  if (!PyArg_ParseTupleAndKeywords(args, keywds, "ii",
                                   const_cast<char **>(kwlist),
                                   &id, &index))
    return NULL;
  PyObject* arg = PyDict_New();
  SLO_VISSYMDEF* arg_ptr = Slo_GetArgById(id);
  PyConvertVISSYMDEF(Slo_GetArrayArgElement(arg_ptr, index), arg);
  return arg;
}

static PyObject*
PySlo_getArrayArgElementByName(PyObject* self, PyObject* args, PyObject *keywds) {
  char* name = NULL;
  int index = -1;
  const char *kwlist[] = {"name", "index", NULL};
  if (!PyArg_ParseTupleAndKeywords(args, keywds, "si",
                                   const_cast<char **>(kwlist),
                                   &name, &index))
    return NULL;
  PyObject* arg = PyDict_New();
  SLO_VISSYMDEF* arg_ptr = Slo_GetArgByName(name);
  PyConvertVISSYMDEF(Slo_GetArrayArgElement(arg_ptr, index), arg);
  return arg;
}

static PyObject*
PySlo_endShader(PyObject* self, PyObject* args) {
    Slo_EndShader();
    Py_RETURN_NONE;
}

static PyObject*
PySlo_typetoStr(PyObject* self, PyObject* args, PyObject *keywds) {
  int type = SLO_TYPE_UNKNOWN;
  const char *kwlist[] = {"type", NULL};
  if (!PyArg_ParseTupleAndKeywords(args, keywds, "i",
                                   const_cast<char **>(kwlist),
                                   &type))
    return NULL;
  return PyString_FromString(Slo_TypetoStr((SLO_TYPE)type));
}

static PyObject*
PySlo_stortoStr(PyObject* self, PyObject* args, PyObject *keywds) {
  int storage = SLO_STOR_UNKNOWN;
  const char *kwlist[] = {"storage", NULL};
  if (!PyArg_ParseTupleAndKeywords(args, keywds, "i",
                                   const_cast<char **>(kwlist),
                                   &storage))
    return NULL;
  return PyString_FromString(Slo_StortoStr((SLO_STORAGE)storage));
}

static PyObject*
PySlo_detailtoStr(PyObject* self, PyObject* args, PyObject *keywds) {
  int detail = SLO_DETAIL_UNKNOWN;
  const char *kwlist[] = {"detail", NULL};
  if (!PyArg_ParseTupleAndKeywords(args, keywds, "i",
                                   const_cast<char **>(kwlist),
                                   &detail))
    return NULL;
  return PyString_FromString(Slo_DetailtoStr((SLO_DETAIL)detail));
}

static PyObject*
PySlo_getMetaData(PyObject* self, PyObject* args, PyObject *keywds) {
  char* name = NULL;
  const char *kwlist[] = {"name", NULL};
  if (!PyArg_ParseTupleAndKeywords(args, keywds, "s",
                                   const_cast<char **>(kwlist),
                                   &name))
    return NULL;
#ifdef PRMAN
  const char* value = Slo_GetMetaData(name);
  if(value != NULL) return PyString_FromString(value);
#elif DELIGHT
  const char* value = Slo_GetAnnotationByKey(name);
  if(value != NULL) return PyString_FromString(value);
#endif
  return PyString_FromString("");
}

static PyObject*
PySlo_getAllMetaData(PyObject* self) {
  PyObject* metadata = PyDict_New();
  if(!metadata) return 0;
#ifdef PRMAN
  const char* const* ptr = Slo_GetAllMetaData();
  if(ptr != NULL) {
    PyObject *key = NULL;
    PyObject *val = NULL;
    for (; *ptr != '\0'; ptr++) {
      if(key == NULL) key = PyString_FromString(*ptr);
      else val = PyString_FromString(*ptr);
      if(key != NULL && val != NULL) {
        PyDict_SetItem(metadata, key, val);
        key = NULL;
        val = NULL;
      }
    }
  }
#elif DELIGHT
  for(int id = 1; id < Slo_GetNAnnotations(); ++id) {
    const char* key = Slo_GetAnnotationKeyById(id);
    const char* value = Slo_GetAnnotationByKey(key);
    PyDict_SetItem(metadata,
        PyString_FromString(key),
        PyString_FromString(value));
  }
#endif
  return metadata;
}

static PyObject*
PySlo_getNAnnotations(PyObject* self) {
#ifdef DELIGHT
  return PyInt_FromLong(Slo_GetNAnnotations());
#else
  return PyInt_FromLong(0);
#endif
}

static PyObject*
PySlo_getAnnotationKeyById(PyObject* self, PyObject* args, PyObject *keywds) {
#ifdef DELIGHT
  int id = SLO_DETAIL_UNKNOWN;
  const char *kwlist[] = {"id", NULL};
  if (!PyArg_ParseTupleAndKeywords(args, keywds, "i",
                                   const_cast<char **>(kwlist),
                                   &id))
    return NULL;
  return PyString_FromString(Slo_GetAnnotationKeyById(id));
#else
  Py_RETURN_NONE;
#endif
}

static PyObject*
PySlo_getAnnotationByKey(PyObject* self, PyObject* args, PyObject *keywds) {
#ifdef DELIGHT
  char* key = NULL;
  const char *kwlist[] = {"key", NULL};
  if (!PyArg_ParseTupleAndKeywords(args, keywds, "s",
                                   const_cast<char **>(kwlist),
                                   &key))
    return NULL;
  return PyString_FromString(Slo_GetAnnotationByKey(key));
#else
  Py_RETURN_NONE;
#endif
}

static PyObject*
PySlo_getPluginsNames(PyObject* self) {
  PyObject* plugins = PyList_New(0);
  if(!plugins) return 0;
#ifdef PRMAN
  const char* const* ptr = Slo_GetPlugins();
  if(ptr != NULL) {
    for (; *ptr != '\0'; ptr++) {
      PyObject *name = PyString_FromString(*ptr);
      PyList_Append(plugins, name);
    }
  }
#endif
  return plugins;
}

// non slo.h convenience methods

static PyObject*
PySlo_isArray(PyObject* self, PyObject* args, PyObject *keywds) {
  int id = SLO_DETAIL_UNKNOWN;
  const char* kwlist[] = {"id", NULL};
  if (!PyArg_ParseTupleAndKeywords(args, keywds, "i",
                                   const_cast<char **>(kwlist),
                                   &id))
    return NULL;
  SLO_VISSYMDEF* param_ptr = Slo_GetArgById(id);
  if (param_ptr == NULL) return false;
  return PyBool_FromLong(param_ptr->svd_arraylen > 0 ||
                         param_ptr->svd_default.scalarval == 0x0);
}

static PyObject*
PySlo_rmanType(PyObject* self, PyObject* args) {
  return PyString_FromString(PyRmanType().c_str());
}

//

static PyMethodDef
PySlo_methods[] = {
  {"setPath",
   (PyCFunction)PySlo_setPath,
   METH_VARARGS | METH_KEYWORDS,
   ""},
  {"getPath",
   (PyCFunction)PySlo_getPath,
   METH_NOARGS,
   ""},
  {"setShader",
   (PyCFunction)PySlo_setShader,
   METH_VARARGS | METH_KEYWORDS,
   ""},
  {"getName",
   (PyCFunction)PySlo_getName,
   METH_NOARGS,
   ""},
  {"getType",
   (PyCFunction)PySlo_getType,
   METH_NOARGS,
   ""},
  {"hasMethod",
   (PyCFunction)PySlo_hasMethod,
   METH_VARARGS | METH_KEYWORDS,
   ""},
  {"getMethodNames",
   (PyCFunction)PySlo_getMethodNames,
   METH_NOARGS,
   ""},
  {"getNArgs",
   (PyCFunction)PySlo_getNArgs,
   METH_NOARGS,
   ""},
  {"getArgById",
   (PyCFunction)PySlo_getArgById,
   METH_VARARGS | METH_KEYWORDS,
   ""},
  {"getArgByName",
   (PyCFunction)PySlo_getArgByName,
   METH_VARARGS | METH_KEYWORDS,
   ""},
  {"getArrayArgElementById",
   (PyCFunction)PySlo_getArrayArgElementById,
   METH_VARARGS | METH_KEYWORDS,
   ""},
  //
  {"getArrayArgElementByName",
   (PyCFunction)PySlo_getArrayArgElementByName,
   METH_VARARGS | METH_KEYWORDS,
   ""},
  {"endShader",
   (PyCFunction)PySlo_endShader,
   METH_NOARGS,
   ""},
  {"typetoStr",
   (PyCFunction)PySlo_typetoStr,
   METH_VARARGS | METH_KEYWORDS,
   ""},
  {"stortoStr",
   (PyCFunction)PySlo_stortoStr,
   METH_VARARGS | METH_KEYWORDS,
   ""},
  {"detailtoStr",
   (PyCFunction)PySlo_detailtoStr,
   METH_VARARGS | METH_KEYWORDS,
   ""},
  {"getMetaData",
   (PyCFunction)PySlo_getMetaData,
   METH_VARARGS | METH_KEYWORDS,
   ""},
  {"getAllMetaData",
   (PyCFunction)PySlo_getAllMetaData,
   METH_NOARGS,
   ""},
  {"getNAnnotations",
   (PyCFunction)PySlo_getNAnnotations,
   METH_NOARGS,
   ""},
  {"getAnnotationKeyById",
   (PyCFunction)PySlo_getAnnotationKeyById,
   METH_VARARGS | METH_KEYWORDS,
   ""},
  {"getAnnotationByKey",
   (PyCFunction)PySlo_getAnnotationByKey,
   METH_VARARGS | METH_KEYWORDS,
   ""},
  {"getPluginsNames",
   (PyCFunction)PySlo_getPluginsNames,
   METH_NOARGS,
   ""},
  //// non slo.h convenience methods ////
  {"isArray",
   (PyCFunction)PySlo_isArray,
   METH_VARARGS | METH_KEYWORDS,
   ""},
  {"rmanType",
   (PyCFunction)PySlo_rmanType,
  METH_NOARGS,
   ""},
  {NULL, NULL, 0, NULL}
};

static PyMethodDef
PySlo_TYPE_methods[] = {{NULL, NULL, 0, NULL}};
static PyMethodDef
PySlo_STORAGE_methods[] = {{NULL, NULL, 0, NULL}};
static PyMethodDef
PySlo_DETAIL_methods[] = {{NULL, NULL, 0, NULL}};

extern "C"
PyMODINIT_FUNC
#ifdef PRMAN
init__PRMan__(void)
#elif DELIGHT
init__Delight__(void)
#elif AQSIS
init__Aqsis__(void)
#else
init__Unknown__(void)
#endif
{
  PyObject *m, *mt, *ms, *md;
  std::string module_name("__"+PyRmanType()+"__");
  m = Py_InitModule3(module_name.c_str(), PySlo_methods,
  "PySlo renderman shader slo python interface");
  if (m == NULL) return;
  
  mt = Py_InitModule3((module_name+".TYPE").c_str(), PySlo_TYPE_methods,
  "SLO_TYPE enum");
  if (mt == NULL) return;
  PyModule_AddIntConstant(mt, "UNKNOWN", SLO_TYPE_UNKNOWN);
  PyModule_AddIntConstant(mt, "POINT", SLO_TYPE_POINT);
  PyModule_AddIntConstant(mt, "COLOR", SLO_TYPE_COLOR);
  PyModule_AddIntConstant(mt, "SCALAR", SLO_TYPE_SCALAR);
  PyModule_AddIntConstant(mt, "STRING", SLO_TYPE_STRING);
  PyModule_AddIntConstant(mt, "SURFACE", SLO_TYPE_SURFACE);
  PyModule_AddIntConstant(mt, "LIGHT", SLO_TYPE_LIGHT);
  PyModule_AddIntConstant(mt, "DISPLACEMENT", SLO_TYPE_DISPLACEMENT);
  PyModule_AddIntConstant(mt, "VOLUME", SLO_TYPE_VOLUME);
#ifdef PRMAN
  PyModule_AddIntConstant(mt, "TRANSFORMATION", SLO_TYPE_UNKNOWN);
  PyModule_AddIntConstant(mt, "STRUCT", SLO_TYPE_STRUCT);
#else
  PyModule_AddIntConstant(mt, "TRANSFORMATION", SLO_TYPE_TRANSFORMATION);
  PyModule_AddIntConstant(mt, "STRUCT", SLO_TYPE_UNKNOWN);
#endif
  PyModule_AddIntConstant(mt, "IMAGER", SLO_TYPE_IMAGER);
  PyModule_AddIntConstant(mt, "VECTOR", SLO_TYPE_VECTOR);
  PyModule_AddIntConstant(mt, "NORMAL", SLO_TYPE_NORMAL);
  PyModule_AddIntConstant(mt, "MATRIX", SLO_TYPE_MATRIX);
  PyModule_AddIntConstant(mt, "SHADER", SLO_TYPE_SHADER);
  PyModule_AddObject(m, "TYPE", mt);
  
  ms = Py_InitModule3((module_name+".STORAGE").c_str(), PySlo_TYPE_methods,
  "SLO_STORAGE enum");
  if (ms == NULL) return;
  PyModule_AddIntConstant(ms, "UNKNOWN", SLO_STOR_UNKNOWN);
  PyModule_AddIntConstant(ms, "CONSTANT", SLO_STOR_CONSTANT);
  PyModule_AddIntConstant(ms, "VARIABLE", SLO_STOR_VARIABLE);
  PyModule_AddIntConstant(ms, "TEMPORARY", SLO_STOR_TEMPORARY);
  PyModule_AddIntConstant(ms, "PARAMETER", SLO_STOR_PARAMETER);
  PyModule_AddIntConstant(ms, "OUTPUTPARAMETER", SLO_STOR_OUTPUTPARAMETER);
  PyModule_AddIntConstant(ms, "GSTATE", SLO_STOR_GSTATE);
#ifdef PRMAN
  PyModule_AddIntConstant(ms, "METHODINPUT", SLO_STOR_METHODINPUT);
  PyModule_AddIntConstant(ms, "METHODOUTPUT", SLO_STOR_METHODOUTPUT);
  PyModule_AddIntConstant(ms, "REFERENCE", SLO_STOR_REFERENCE);
  PyModule_AddIntConstant(ms, "CONST_REFERENCE", SLO_STOR_CONST_REFERENCE);
#else
  PyModule_AddIntConstant(ms, "METHODINPUT", SLO_STOR_UNKNOWN);
  PyModule_AddIntConstant(ms, "METHODOUTPUT", SLO_STOR_UNKNOWN);
  PyModule_AddIntConstant(ms, "REFERENCE", SLO_STOR_UNKNOWN);
  PyModule_AddIntConstant(ms, "CONST_REFERENCE", SLO_STOR_UNKNOWN);
#endif
  PyModule_AddObject(m, "STORAGE", ms);
    
  md = Py_InitModule3((module_name+".DETAIL").c_str(), PySlo_TYPE_methods,
  "SLO_DETAIL enum");
  if (md == NULL) return;
  PyModule_AddIntConstant(md, "UNKNOWN", SLO_DETAIL_UNKNOWN);
  PyModule_AddIntConstant(md, "VARYING", SLO_DETAIL_VARYING);
  PyModule_AddIntConstant(md, "UNIFORM", SLO_DETAIL_UNIFORM);
  PyModule_AddObject(m, "DETAIL", md);
  
}
