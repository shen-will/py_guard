//
// Created by Hasee on 2019/10/21.
//

#ifndef FFITEST_PY_MODULE_HELPER_H
#define FFITEST_PY_MODULE_HELPER_H

#include "Python.h"

#define PY_MODULE_INIT(name) PyInit_##name()

#define DEFINE_MODULE(name,doc)\
static PyModuleDef _mod_ = {\
.m_name = name,\
.m_doc =  doc,\
.m_base = PyModuleDef_HEAD_INIT,\
.m_methods = NULL,\
.m_clear = NULL,\
.m_free = NULL,\
.m_slots = NULL,\
.m_size = 0,\
.m_traverse = NULL\
};\

#define Module_Def (&_mod_)

#define _CREATE_METHOD_INFO(name,doc)\
static PyMethodDef METHOD_INFO_##name = {#name,(PyCFunction)__##name,METH_FASTCALL,doc};

#define TYPE_CHECK_CODE(arg,t) \
if(arg->ob_type != &t && arg != Py_None){\
        error = "arg type error";\
        goto error;\
}

#define TYPE_CHECK1(argv,t) \
    TYPE_CHECK_CODE(argv[0],t)

#define TYPE_CHECK2(argv,t1,t2)\
    TYPE_CHECK_CODE(argv[1],t2)\
    TYPE_CHECK1(argv,t1)\

#define TYPE_CHECK3(argv,t1,t2,t3)\
    TYPE_CHECK_CODE(argv[2],t3)\
    TYPE_CHECK2(argv,t1,t2)\

#define TYPE_CHECK4(argv,t1,t2,t3,t4)\
    TYPE_CHECK_CODE(argv[3],t4)\
    TYPE_CHECK3(argv,t1,t2,t3)\

#define TYPE_CHECK5(argv,t1,t2,t3,t4,t5)\
    TYPE_CHECK_CODE(argv[4],t5)\
    TYPE_CHECK4(argv,t1,t2,t3,t4)\

#define MODULE_EXPORT0(name,doc)\
static PyObject* name(PyObject*);\
static PyObject* __##name(PyObject *self,PyObject **argv,Py_ssize_t argc){\
    char *error = NULL;\
    if(argc != 0){\
        error = "argc must be 0";\
        goto error;\
    }\
    return name(self);\
error:\
    PyErr_SetString(PyExc_Exception,error);\
    return NULL;\
}\
_CREATE_METHOD_INFO(name,doc)


#define MODULE_EXPORT1(name,t1,doc)\
static PyObject* name(PyObject*,PyObject*);\
static PyObject* __##name(PyObject *self,PyObject **argv,Py_ssize_t argc){\
    char *error = NULL;\
    if(argc != 1){\
        error = "argc must be 1";\
        goto error;\
    }\
    TYPE_CHECK1(argv,t1)\
    return name(self,argv[0]);\
error:\
    PyErr_SetString(PyExc_Exception,error);\
    return NULL;\
}\
_CREATE_METHOD_INFO(name,doc)


#define MODULE_EXPORT2(name,t1,t2,doc)\
static PyObject* name(PyObject*,PyObject*,PyObject*);\
static PyObject* __##name(PyObject *self,PyObject **argv,Py_ssize_t argc){\
    char *error = NULL;\
    if(argc != 2){\
        error = "argc must be 2";\
        goto error;\
    }\
    TYPE_CHECK2(argv,t1,t2)\
    return name(self,argv[0],argv[1]);\
error:\
    PyErr_SetString(PyExc_Exception,error);\
    return NULL;\
}\
_CREATE_METHOD_INFO(name,doc)


#define MODULE_EXPORT3(name,t1,t2,t3,doc)\
static PyObject* name(PyObject*,PyObject*,PyObject*,PyObject*);\
static PyObject* __##name(PyObject *self,PyObject **argv,Py_ssize_t argc){\
    char *error = NULL;\
    if(argc != 3){\
        error = "argc must be 3";\
        goto error;\
    }\
    TYPE_CHECK3(argv,t1,t2,t3)\
    return name(self,argv[0],argv[1],argv[2]);\
error:\
    PyErr_SetString(PyExc_Exception,error);\
    return NULL;\
}\
_CREATE_METHOD_INFO(name,doc)


#define FAST_RETURN(obj) \
Py_XINCREF(obj);\
return obj


#define FAST_FAIL_RETURN(cond,err,ret) \
if((cond)){PyErr_SetString(PyExc_BaseException,err);\
return ret;}\

#define CHECK_ERROR_RETURN(con,error,ret)\
if((con)){\
const char *str = error;\
if(str != NULL)\
    PyErr_SetString(PyExc_BaseException,str);\
return ret;\
}\


#define Unicode(str) PyUnicode_FromString(str)
#define AS_UTF8(obj) PyUnicode_AsUTF8(obj)


#define PY_ERROR_STR \
({\
PyThreadState *thread = PyThreadState_GET();\
const char * str_info = NULL;\
if(thread->curexc_value != NULL && PyUnicode_CheckExact(thread->curexc_value))\
    str_info = PyUnicode_AsUTF8(thread->curexc_value);\
if(str_info == NULL || strlen(str_info) == 0)\
    str_info = "no error";\
str_info;})\


#define DESTROY_OBJECT(obj) \
if(obj != NULL){\
    Py_ssize_t i = obj->ob_refcnt;\
    while(i-- > 0){\
        Py_XDECREF(obj);\
}\
}\


#define CHECK_NONE(obj) (obj == NULL || obj == Py_None)

#define BYTES_TO_ARRAY(bytes) \
PyByteArray_FromStringAndSize(PyBytes_AsString((bytes)),\
PyBytes_GET_SIZE((bytes)))\


#define ARRAY_TO_BYTES(array) \
PyBytes_FromStringAndSize(PyByteArray_AsString((array)),PyByteArray_GET_SIZE((array)))

// TEST_ADD(method) printf("%s  %x\n",#method,method)
#define TEST_ADD(method)

int regiester_methods(PyModuleDef *moduleDef,unsigned int count,PyMethodDef *infos[]);

#endif //FFITEST_PY_MODULE_HELPER_H
