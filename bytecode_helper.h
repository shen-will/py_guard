//
// Created by Hasee on 2019/11/12.
//

#ifndef FFITEST_BYTECODE_HELPER_H
#define FFITEST_BYTECODE_HELPER_H

#include "Python.h"
#include <stdint.h>


#define _Py_OPCODE(word) ((word) & 255)
#define _Py_OPARG(word) ((word) >> 8)

#define NEXTOPARG(next_instr,opcode,oparg)  \
        _Py_CODEUNIT word = *next_instr; \
        opcode = _Py_OPCODE(word); \
        oparg = _Py_OPARG(word); \
        next_instr++;


#define EACH_BYTECODE(obj,ins,arg,code) {\
    uint16_t  *first = ((PyBytesObject *)(obj->co_code))->ob_sval;\
    uint16_t  *next = first;\
    int ins;\
    int arg;\
    int count = ((PyBytesObject *)(obj->co_code))->ob_base.ob_size;\
    int i =0;\
    for(;i < count;i+=sizeof(uint16_t)){\
        NEXTOPARG(next,ins,arg);\
        code\
    }\
}

typedef PyObject* (*BYTECODE_TRANSFORM)(PyObject *ins);

PyObject * py2code(const char *file);
int code2pyc(PyObject *code,const char *path);




#endif //FFITEST_BYTECODE_HELPER_H
