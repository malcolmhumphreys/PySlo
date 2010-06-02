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

#include <boost/python.hpp>
#include <algorithm>
#include <vector>
#include <string>
#include <iostream>
#include <slo.h>

using namespace boost::python;

list
PyGetMethodNames() {
    
    list methods;
    const char* const* ptr = Slo_GetMethodNames();
    if(ptr != NULL) {
        for (; *ptr != '\0'; ptr++)
            methods.append(*ptr);
    }
    
    return methods;
}

void
PyGetArgDefault(SLO_VISSYMDEF *param_ptr, list out) {
    
    if (param_ptr == NULL) return;
    
    list tmp;
    switch (param_ptr->svd_type) {
        case SLO_TYPE_POINT:
        case SLO_TYPE_COLOR:
        case SLO_TYPE_VECTOR:
        case SLO_TYPE_NORMAL:
            tmp.append(param_ptr->svd_default.pointval[0].xval);
            tmp.append(param_ptr->svd_default.pointval[0].yval);
            tmp.append(param_ptr->svd_default.pointval[0].zval);
            out.append(tmp);
            break;
        case SLO_TYPE_SCALAR:
            out.append(param_ptr->svd_default.scalarval[0]);
            break;
        case SLO_TYPE_STRING:
            out.append(std::string(param_ptr->svd_default.stringval));
            break;
        case SLO_TYPE_MATRIX:
            for(unsigned m = 0; m < 16; ++m)
                tmp.append(param_ptr->svd_default.matrixval[m]);
            out.append(tmp);
            break;
    }
    
    return;
}

void
PyGetArrayArgDefault(SLO_VISSYMDEF *param_ptr, list out) {
    
    if (param_ptr == NULL) return;
    
    for (int i = 0; i < param_ptr->svd_arraylen; i++) 
        PyGetArgDefault (Slo_GetArrayArgElement (param_ptr, i), out);
    
    return;
}

void
PyConvertVISSYMDEF(SLO_VISSYMDEF *param_ptr, dict out) {
    
    if (param_ptr == NULL) return;
    
    // straight copy
    out["name"] = std::string( param_ptr->svd_name );
    out["type"] = param_ptr->svd_type;
    out["storage"] = param_ptr->svd_storage;
    out["detail"] = param_ptr->svd_detail;
    out["spacename"] = std::string( param_ptr->svd_spacename );
    out["valisvalid"] = param_ptr->svd_valisvalid;
    out["arraylen"] = param_ptr->svd_arraylen;
    
    // get the default value
    list df_list;
    if (!(param_ptr->svd_arraylen > 0 || param_ptr->svd_default.scalarval == 0x0))
        PyGetArgDefault(param_ptr, df_list);
    else
        PyGetArrayArgDefault(param_ptr, df_list);
    out["default"] = df_list;
    
    return;
}

dict
PyGetArgById(int i_id) {
    
    dict arg;
    SLO_VISSYMDEF* param_ptr = Slo_GetArgById(i_id);
    PyConvertVISSYMDEF(param_ptr, arg);
    
    return arg;
}

dict
PyGetArgByName(char *i_name) {
    
    dict arg;
    SLO_VISSYMDEF* param_ptr = Slo_GetArgByName(i_name);
    PyConvertVISSYMDEF(param_ptr, arg);
    
    return arg;
}

dict
PyGetArrayArgElementById(int i_id, int i_index) {
    
    dict arg;
    SLO_VISSYMDEF* param_ptr = Slo_GetArgById(i_id);
    PyConvertVISSYMDEF(Slo_GetArrayArgElement(param_ptr, i_index), arg);
    
    return arg;
}

dict
PyGetArrayArgElementByName(char *i_name, int i_index) {
    
    dict arg;
    SLO_VISSYMDEF* param_ptr = Slo_GetArgByName(i_name);
    PyConvertVISSYMDEF(Slo_GetArrayArgElement(param_ptr, i_index), arg);
    
    return arg;
}

// wrappers for Slo_*toStr() methods because prman's Slo interface 
// returns non-const char *
std::string
PySloTypeToStr(SLO_TYPE i_type) {
    return std::string(Slo_TypetoStr(i_type));
}

std::string
PySloStorToStr(SLO_STORAGE i_storage) {
    return std::string(Slo_StortoStr(i_storage));
}

std::string
PySloDetailToStr(SLO_DETAIL i_detail) {
    return std::string(Slo_DetailtoStr(i_detail));
}

bool
PyIsArray(int i_id) {
    SLO_VISSYMDEF* param_ptr = Slo_GetArgById(i_id);
    if (param_ptr == NULL) return false;
    return (param_ptr->svd_arraylen > 0 || param_ptr->svd_default.scalarval == 0x0);
}

std::string
PyRmanType() {
  #ifdef PRMAN
  return std::string("PRman");
  #elif DELIGHT
  return std::string("Delight");
  #elif AQSIS
  return std::string("Aqsis");
  #else
  return std::string("Unknown");
  #endif
}

#ifdef PRMAN
BOOST_PYTHON_MODULE(__PRman__)
#elif DELIGHT
BOOST_PYTHON_MODULE(__Delight__)
#elif AQSIS
BOOST_PYTHON_MODULE(__Aqsis__)
#else
BOOST_PYTHON_MODULE(__Unknown__)
#endif
{
    
    enum_<SLO_TYPE>("type")
        .value("unknown", SLO_TYPE_UNKNOWN)
        .value("point", SLO_TYPE_POINT)
        .value("color", SLO_TYPE_COLOR)
        .value("scalar", SLO_TYPE_SCALAR)
        .value("string", SLO_TYPE_STRING)
        .value("surface", SLO_TYPE_SURFACE)
        .value("light", SLO_TYPE_LIGHT)
        .value("displacement", SLO_TYPE_DISPLACEMENT)
        .value("volume", SLO_TYPE_VOLUME)
#ifndef PRMAN
        .value("transformation", SLO_TYPE_TRANSFORMATION)
#endif // PRMAN
        .value("imager", SLO_TYPE_IMAGER)
        .value("vector", SLO_TYPE_VECTOR)
        .value("normal", SLO_TYPE_NORMAL)
        .value("matrix", SLO_TYPE_MATRIX)
        .value("shader", SLO_TYPE_SHADER)
        .export_values();
        
    enum_<SLO_STORAGE>("storage")
        .value("unknown", SLO_STOR_UNKNOWN)
        .value("constant", SLO_STOR_CONSTANT)
        .value("variable", SLO_STOR_VARIABLE)
        .value("temporary", SLO_STOR_TEMPORARY)
        .value("parameter", SLO_STOR_PARAMETER)
        .value("outputparameter", SLO_STOR_OUTPUTPARAMETER)
        .value("gstate", SLO_STOR_GSTATE)
        .export_values();
    
    enum_<SLO_DETAIL>("detail")
        .value("unknown", SLO_DETAIL_UNKNOWN)
        .value("varying", SLO_DETAIL_VARYING)
        .value("uniform", SLO_DETAIL_UNIFORM)
        .export_values();
    
     def("setPath", Slo_SetPath);
     def("setShader", Slo_SetShader);
     def("getName", Slo_GetName);
     def("getType", Slo_GetType);
     def("hasMethod", Slo_HasMethod);
     def("getMethodNames", PyGetMethodNames);
     def("getNArgs", Slo_GetNArgs);
     def("getArgById", PyGetArgById);
     def("getArgByName", PyGetArgByName);
     def("getArrayArgElementById", PyGetArrayArgElementById);
     def("getArrayArgElementByName", PyGetArrayArgElementByName);
     def("typetoStr", PySloTypeToStr);
     def("stortoStr", PySloStorToStr);
     def("detailtoStr", PySloDetailToStr);
     def("endShader", Slo_EndShader);
#ifdef DELIGHT
     def("getNAnnotations", Slo_GetNAnnotations);
     def("getAnnotationKeyById", Slo_GetAnnotationKeyById);
     def("getAnnotationByKey", Slo_GetAnnotationByKey);
#endif // DELIGHT
#ifdef PRMAN
     // def("getMetaData", Slo_GetMetaData ); // TODO
     // def("getAllMetaData", Slo_GetAllMetaData ); // TODO
#endif // PRMAN
    
    // custom
    def("isArray", PyIsArray);
    def("rmanType", PyRmanType);
    
}
