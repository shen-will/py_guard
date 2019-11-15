//
// Created by Hasee on 2019/11/12.
//

#include "py2pyx.h"
#include "py_module_helper.h"
#include "bytecode_helper.h"
#include "transform.h"

#define MODULE_NAME "py2pyx"

DEFINE_MODULE(MODULE_NAME,MODULE_NAME)

static int py2pyx_init(){


    return 0;
}

static PyObject* py_encode(PyObject *path,int boot){

    CHECK_ERROR_RETURN(path == Py_None,"py_encode path is empty",NULL);

    const char *utf8_path = AS_UTF8(path);
    size_t path_len = strlen(utf8_path);

    CHECK_ERROR_RETURN(strcmp(utf8_path+(path_len-3),".py")!=0,"py_encode path must be endwith .py",NULL);

    PyObject *code = py2code(utf8_path);

    CHECK_ERROR_RETURN(code == NULL , "py_encode compile path fail",NULL);

    if(transform_code(code,boot == 0 ? default_source_transform : default_boot_transform
            ,TYPE_ENCODE)!=0){
        DESTROY_OBJECT(code);
        FAST_FAIL_RETURN(1,"encode fail",NULL);
    }


   char *pyx_path = PyMem_Malloc(path_len+2);

    memcpy(pyx_path,utf8_path,path_len);
    pyx_path[path_len] = 'c';
    pyx_path[path_len+1] = 0;

    if(code2pyc(code,pyx_path) != 0){
        free(pyx_path);
        DESTROY_OBJECT(code);
        FAST_FAIL_RETURN(1,"py_encode write pyc file fail",NULL);
    }
    PyMem_Free(pyx_path);
    DESTROY_OBJECT(code);

    FAST_RETURN(Py_None);
}

MODULE_EXPORT1(py2pyx,PyUnicode_Type,"py2pyx")
static PyObject* py2pyx(PyObject *self,PyObject *path){

    return py_encode(path,0);
}

MODULE_EXPORT1(pyc2pyx,PyUnicode_Type,"pyc2pyx")
static PyObject* pyc2pyx(PyObject *self,PyObject *path){


    FAST_RETURN(Py_None);
}



MODULE_EXPORT1(set_encode,PyFunction_Type,"set_encode")
static PyObject* set_encode(PyObject *self,PyObject *func){

    transform_set_encode(func);
    FAST_RETURN(Py_None);
}


MODULE_EXPORT1(set_decode,PyFunction_Type,"set_decode")
static PyObject* set_decode(PyObject *self,PyObject *func){

    transform_set_decode(func);
    FAST_RETURN(Py_None);
}


MODULE_EXPORT1(boot_encode,PyUnicode_Type,"boot_encode")
static PyObject* boot_encode(PyObject *self,PyObject *path){
puts("boot encode");
    return py_encode(path,1);
}


PyObject* PY_MODULE_INIT(py2pyx){

    PyMethodDef* methods[5]={
            &METHOD_INFO_boot_encode,
            &METHOD_INFO_set_decode,
            &METHOD_INFO_set_encode,
            &METHOD_INFO_py2pyx,
            &METHOD_INFO_pyc2pyx
    };

    if(regiester_methods(Module_Def,5,methods)!=5){
        FAST_FAIL_RETURN(1,"create module py2pyx fail",NULL);
    }

    PyObject *m = PyModule_Create(Module_Def);

    if(m == NULL || m == Py_None || py2pyx_init() != 0){
        Py_XDECREF(m);
        return NULL;
    }

    return m;
}