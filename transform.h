//
// Created by Hasee on 2019/11/14.
//

#ifndef FFITEST_TRANSFORM_H
#define FFITEST_TRANSFORM_H

#include "Python.h"

typedef enum {
    TYPE_ENCODE =0,
    TYPE_DECODE
}TransformType;

typedef int (*Transform)(PyObject *code,TransformType type);

//user set the source encoder/decoder
void transform_set_encode(PyObject *encode);
void transform_set_decode(PyObject *decode);


int default_source_transform(PyObject *code,TransformType type);
int default_boot_transform(PyObject *code,TransformType type);

int transform_code(PyObject *code,Transform impl,TransformType type);



#endif //FFITEST_TRANSFORM_H
