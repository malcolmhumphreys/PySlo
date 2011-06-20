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

#include <Python.h>
#include "slo.h"

/*
DL_INTERFACE SLO_TYPE Slo_GetType();
DL_INTERFACE int Slo_HasMethod(const char *i_name);
DL_INTERFACE const char* const* Slo_GetMethodNames();
DL_INTERFACE int Slo_GetNArgs();
DL_INTERFACE SLO_VISSYMDEF *Slo_GetArgById(int i_id);
DL_INTERFACE SLO_VISSYMDEF *Slo_GetArgByName(const char *i_name);
DL_INTERFACE SLO_VISSYMDEF *Slo_GetArrayArgElement(SLO_VISSYMDEF *i_array, int i_index );
DL_INTERFACE int Slo_GetNAnnotations();
DL_INTERFACE const char *Slo_GetAnnotationKeyById(int i_id);
DL_INTERFACE const char *Slo_GetAnnotationByKey(const char *i_key);
DL_INTERFACE void Slo_EndShader();
DL_INTERFACE const char *Slo_TypetoStr(SLO_TYPE i_type);
DL_INTERFACE const char *Slo_StortoStr(SLO_STORAGE i_storage);
DL_INTERFACE const char *Slo_DetailtoStr(SLO_DETAIL i_detail);
*/

/*
PyObject* PySlo_hasMethod(PyObject* self, PyObject* args) {
    char* i_name = 0;
    if(!PyArg_ParseTuple(args,"s:i_name", &i_name))
        return NULL;
    return PyBool_FromLong(Slo_HasMethod(i_name));
}

PyObject* PySlo_getMethodNames(PyObject* self) {
    return PyString_FromString(Slo_GetMethodNames());
}

PyObject* PySlo_getPluginsNames(PyObject* self) {
    PyObject* plugins = PyList_New(0);
    if(!plugins) return 0;
#ifdef PRMAN
    const char* const* ptr = Slo_GetPlugins();
    if(ptr != NULL) {
        for (; *ptr != '\0'; ptr++) {
            PyObject *plugin = PyString_FromString(*ptr);
            PyList_Append(plugins, plugin);
        }
    }
#endif
    return plugins;
}

PyObject* PySlo_getNArgs(PyObject* self) {
    return PyInt_FromLong(Slo_GetNArgs());
}

PyObject* PySlo_getArgById(PyObject* self, PyObject* args) {
    //int i_id
    //return SLO_VISSYMDEF
}

PyObject* PySlo_getArgByName(PyObject* self, PyObject* args) {
    //const char *i_name
    //return SLO_VISSYMDEF
}

PyObject* PySlo_getArrayArgElementById(PyObject* self, PyObject* args) {
    //const char *i_name
    //return SLO_VISSYMDEF
}

PyObject* PySlo_getArrayArgElementByName(PyObject* self, PyObject* args) {
    //SLO_VISSYMDEF *i_array, int i_index
    //return SLO_VISSYMDEF
}

PyObject* PySlo_typetoStr(PyObject* self, PyObject* args) {
    //SLO_TYPE i_type
    //return const char *
}

PyObject* PySlo_stortoStr(PyObject* self, PyObject* args) {
    //SLO_STORAGE i_storage
    //return const char *
}

PyObject* PySlo_detailtoStr(PyObject* self, PyObject* args) {
    //SLO_DETAIL i_detail
    //return const char *
}

PyObject* PySlo_endShader(PyObject* self) {
    Slo_EndShader();
    Py_RETURN_NONE;
}

PyObject* PySlo_getNAnnotations(PyObject* self) {
    return PyInt_FromLong(Slo_GetNAnnotations());
}

PyObject* PySlo_getAnnotationKeyById(PyObject* self, PyObject* args) {
    int i_id = 0;
    if(!PyArg_ParseTuple(args,"i:i_id", &i_id))
        return NULL;
    return PyString_FromString(Slo_GetAnnotationKeyById(i_id));
}

PyObject* PySlo_getAnnotationByKey(PyObject* self, PyObject* args) {
    char* i_key = 0;
    if(!PyArg_ParseTuple(args,"s:i_key", &i_key))
        return NULL;
    return PyString_FromString(Slo_GetAnnotationByKey(i_key));
}

//PyObject* PySlo_getMetaData(PyObject* self, PyObject* args) {}
//PyObject* PySlo_getAllMetaData(PyObject* self) {}

PyObject* PySlo_isArray(PyObject* self, PyObject* args) {
    //return bool
}

PyObject* PySlo_rmanType(PyObject* self) {
    //return string
}

static PyMethodDef PySlo_methods[] = {
{"setPath", PySlo_setPath,
    METH_VARARGS, ""},
{"setShader", PySlo_setShader,
    METH_VARARGS, ""},
{"getName", PySlo_getName,
    METH_VARARGS, ""},
{"getType", PySlo_getType,
    METH_VARARGS, ""},
{"hasMethod", PySlo_hasMethod,
    METH_VARARGS, ""},
{"getMethodNames", PySlo_getMethodNames,
    METH_VARARGS, ""},
{"getPluginsNames", PySlo_getPluginsNames,
    METH_VARARGS, ""},
{"getNArgs", PySlo_getNArgs,
    METH_VARARGS, ""},
{"getArgById", PySlo_getArgById,
    METH_VARARGS, ""},
{"getArgByName", PySlo_getArgByName,
    METH_VARARGS, ""},
{"getArrayArgElementById", PySlo_getArrayArgElementById,
    METH_VARARGS, ""},
{"getArrayArgElementByName", PySlo_getArrayArgElementByName,
    METH_VARARGS, ""},
{"typetoStr", PySlo_typetoStr,
    METH_VARARGS, ""},
{"stortoStr", PySlo_stortoStr,
    METH_VARARGS, ""},
{"detailtoStr", PySlo_detailtoStr,
    METH_VARARGS, ""},
{"endShader", PySlo_endShader,
    METH_VARARGS, ""},
// DELIGHT
{"getNAnnotations", PySlo_getNAnnotations,
    METH_VARARGS, ""},
{"getAnnotationKeyById", PySlo_getAnnotationKeyById,
    METH_VARARGS, ""},
{"getAnnotationByKey", PySlo_getAnnotationByKey,
    METH_VARARGS, ""},
// PRMAN
//{"getMetaData", PySlo_getMetaData,
//    METH_VARARGS, ""},
//{"getAllMetaData", PySlo_getAllMetaData,
//    METH_VARARGS, ""},
// OTHER
{"isArray", PySlo_isArray,
    METH_VARARGS, ""},
{"rmanType", PySlo_rmanType,
    METH_VARARGS, ""},
{NULL, NULL, 0, NULL}
};
*/

PyObject* PySlo_setPath(PyObject* self, PyObject* args) {
    char* i_path = 0;
    if(!PyArg_ParseTuple(args,"s:i_path", &i_path))
        return NULL;
    return PyInt_FromLong(Slo_SetPath(i_path));
}

PyObject* PySlo_setShader(PyObject* self, PyObject* args) {
    char* i_name = 0;
    if(!PyArg_ParseTuple(args,"s:i_name", &i_name))
        return NULL;
    return PyInt_FromLong(Slo_SetPath(i_name));
}

PyObject* PySlo_getName(PyObject* self, PyObject *args) {
    return PyString_FromString(Slo_GetName());
}

PyObject* PySlo_getType(PyObject* self, PyObject *args) {
    return PyInt_FromLong(Slo_GetType());
}


static PyMethodDef PySlo_methods[] = {
    {"setPath", PySlo_setPath, METH_VARARGS, ""},
    {"setShader", PySlo_setShader, METH_VARARGS, ""},
    {"getName", PySlo_getName, METH_NOARGS, ""},
    {"getType", PySlo_getType, METH_NOARGS, ""},
    {NULL, NULL, 0, NULL}
};

static PyMethodDef PySlo_TYPE_methods[] = {{NULL, NULL, 0, NULL}};
static PyMethodDef PySlo_STORAGE_methods[] = {{NULL, NULL, 0, NULL}};
static PyMethodDef PySlo_DETAIL_methods[] = {{NULL, NULL, 0, NULL}};

extern "C"
PyMODINIT_FUNC
init__Delight__(void)
{
    PyObject *m, *mt, *ms, *md;
    m = Py_InitModule3("__Delight__",
        PySlo_methods,
        "PySlo renderman shader slo python interface");
    if (m == NULL) return;
    
    mt = Py_InitModule3("__Delight__.TYPE",
        PySlo_TYPE_methods,
        "type");
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
    PyModule_AddIntConstant(mt, "TRANSFORMATION", SLO_TYPE_TRANSFORMATION);
    PyModule_AddIntConstant(mt, "IMAGER", SLO_TYPE_IMAGER);
    PyModule_AddIntConstant(mt, "VECTOR", SLO_TYPE_VECTOR);
    PyModule_AddIntConstant(mt, "NORMAL", SLO_TYPE_NORMAL);
    PyModule_AddIntConstant(mt, "MATRIX", SLO_TYPE_MATRIX);
    PyModule_AddIntConstant(mt, "SHADER", SLO_TYPE_SHADER);
    PyModule_AddObject(m, "TYPE", mt);
    
    ms = Py_InitModule3("__Delight__.STORAGE",
        PySlo_TYPE_methods,
        "storage");
    if (ms == NULL) return;
    PyModule_AddIntConstant(ms, "UNKNOWN", SLO_STOR_UNKNOWN);
    PyModule_AddIntConstant(ms, "CONSTANT", SLO_STOR_CONSTANT);
    PyModule_AddIntConstant(ms, "VARIABLE", SLO_STOR_VARIABLE);
    PyModule_AddIntConstant(ms, "TEMPORARY", SLO_STOR_TEMPORARY);
    PyModule_AddIntConstant(ms, "PARAMETER", SLO_STOR_PARAMETER);
    PyModule_AddIntConstant(ms, "OUTPUTPARAMETER", SLO_STOR_OUTPUTPARAMETER);
    PyModule_AddIntConstant(ms, "GSTATE", SLO_STOR_GSTATE);
    PyModule_AddObject(m, "STORAGE", ms);
    
    md = Py_InitModule3("__Delight__.DETAIL",
        PySlo_TYPE_methods,
        "detail");
    if (md == NULL) return;
    PyModule_AddIntConstant(md, "UNKNOWN", SLO_DETAIL_UNKNOWN);
    PyModule_AddIntConstant(md, "VARYING", SLO_DETAIL_VARYING);
    PyModule_AddIntConstant(md, "UNIFORM", SLO_DETAIL_UNIFORM);
    PyModule_AddObject(m, "DETAIL", md);
    
}


