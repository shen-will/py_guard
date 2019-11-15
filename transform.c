//
// Created by Hasee on 2019/11/14.
//

#include "transform.h"
#include "flags.h"
#include "py_module_helper.h"
#include "boot.h"

static PyObject *py_encode = NULL;
static PyObject *py_decode = NULL;

int transform_code(PyObject *code, Transform impl,TransformType type) {


    if(code == NULL || py_encode == NULL || impl == NULL)
    return -1;


    PyCodeObject *mcode =(PyCodeObject*)code;

    if(impl(code,type)!=0)
        return -1;
    Py_ssize_t count = PyObject_Length(mcode->co_consts);
    int i=0;
    for(;i< count;i++){
        PyObject *item = PyTuple_GetItem(mcode->co_consts,i);
        if(!PyObject_TypeCheck(item,&PyCode_Type))
            continue;
        if(transform_code(item,impl,type)!=0)
            return -1;
    }

    return 0;
}


static int default_transform(PyObject *code,TransformType type,int boot){


    if(type != TYPE_ENCODE && type != TYPE_DECODE)
        return -1;

    PyCodeObject *mcode =(PyCodeObject*)code;

    PyObject *ins_array = BYTES_TO_ARRAY(mcode->co_code);

    PyObject *ret = NULL;
    if(boot == 0){

        PyObject *args = PyTuple_New(1);
        Py_XINCREF(ins_array);
        PyTuple_SetItem(args,0,ins_array);
        ret = PyObject_Call(type == TYPE_ENCODE ? py_encode : py_decode,args,NULL);
        DESTROY_OBJECT(args);
    }
    else{
        ret = type == TYPE_ENCODE ? boot_encode_impl(ins_array) : boot_decode_impl(ins_array);
    }

    if(!PyObject_TypeCheck(ret,&PyByteArray_Type) || (PyByteArray_GET_SIZE(ret) % sizeof(uint16_t))!=0 ){
        DESTROY_OBJECT(ins_array);
        if(ins_array != ret)
            DESTROY_OBJECT(ret);
        return -1;
    }

    PyObject *new_inst = ARRAY_TO_BYTES(ret);
    DESTROY_OBJECT(mcode->co_code);
    mcode->co_code =new_inst;

    if(type == TYPE_ENCODE)
        mcode->co_flags |= (boot == 0 ? CO_ENCODE  : CO_BOOT);

    DESTROY_OBJECT(ins_array);
    if(ins_array != ret)
        DESTROY_OBJECT(ret);

    assert(PyBytes_Check(mcode->co_code));
    assert(PyBytes_GET_SIZE(mcode->co_code) <= INT_MAX);
    assert(PyBytes_GET_SIZE(mcode->co_code) % sizeof(_Py_CODEUNIT) == 0);
    assert(_Py_IS_ALIGNED(PyBytes_AS_STRING(mcode->co_code), sizeof(_Py_CODEUNIT)));

    return 0;
}


int default_source_transform(PyObject *code,TransformType type) {

    return default_transform(code,type,0);
}

int default_boot_transform(PyObject *code,TransformType type) {

    return default_transform(code,type,1);
}

void transform_set_encode(PyObject *encode) {

    if(CHECK_NONE(encode))
        return;

    Py_XINCREF(encode);
    Py_XDECREF(py_encode);
    py_encode = encode;
}

void transform_set_decode(PyObject *decode) {

    if(CHECK_NONE(decode))
        return;

    Py_XINCREF(decode);
    Py_XDECREF(py_decode);
    py_decode = decode;
}
