//
// Created by Hasee on 2019/11/13.
//

#include "boot.h"
#include  "py_module_helper.h"
/*
 * boot_decode boot_encode for decode/encode boot python module
 * better modify it yourself
 * */

PyObject* boot_decode_impl(PyObject *ins){

    char *data = PyByteArray_AsString(ins);

    Py_ssize_t count = PyByteArray_GET_SIZE(ins);

    Py_ssize_t i =0;
    for(;i<count;i++)
        data[i]-=1;
    Py_XINCREF(ins);

    return ins;
}

PyObject* boot_encode_impl(PyObject *ins){

    char *data = PyByteArray_AsString(ins);

    Py_ssize_t count = PyByteArray_GET_SIZE(ins);

    Py_ssize_t i =0;
    for(;i<count;i++)
        data[i]+=1;

    Py_XINCREF(ins);

    return ins;
}