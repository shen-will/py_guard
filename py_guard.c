//
// Created by Hasee on 2019/11/11.
//

#include "py_guard.h"
#include "py_module_helper.h"
#include "frameobject.h"
#include "bytecode_helper.h"
#include "py2pyx.h"
#include "transform.h"
#include "flags.h"


#if PY_MINOR_VERSION > 7
#define Py_BUILD_CORE
#include "internal/pycore_pystate.h"
#else
#include "pystate.h"
#endif

typedef PyObject* (*_PyFrameEvalFunction)(struct _frame *, int);

#define MODULE_NAME "py_guard"

DEFINE_MODULE(MODULE_NAME,MODULE_NAME)

static _PyFrameEvalFunction ori_eval = NULL;

#define IS_ENCODE(flags) (((flags) & CO_ENCODE) == CO_ENCODE || ((flags) & CO_BOOT) == CO_BOOT)


static PyCodeObject* default_decoder(PyCodeObject *code){

    if(!IS_ENCODE(code->co_flags))
        return code;

    if((code->co_flags & CO_ENCODE) == CO_ENCODE){
       if(default_source_transform((PyObject*)code,TYPE_DECODE)!=0)
           return NULL;
       // printf("decode source %s\n",AS_UTF8(code->co_name));

    } else if((code->co_flags & CO_BOOT) == CO_BOOT){
       // printf("decode boot %s\n",AS_UTF8(code->co_name));
        if(default_boot_transform((PyObject*)code,TYPE_DECODE) != 0)
            return NULL;
    }
    return code;
}

static PyCodeObject* default_encoder(PyCodeObject *code){

    if(!IS_ENCODE(code->co_flags))
        return code;

    if((code->co_flags & CO_ENCODE) == CO_ENCODE){
        // printf("decode source %s\n",AS_UTF8(code->co_name));
        if(default_source_transform((PyObject*)code,TYPE_ENCODE)!=0)
            return NULL;

    } else if((code->co_flags & CO_BOOT) == CO_BOOT){
        // printf("decode boot %s\n",AS_UTF8(code->co_name));
        if(default_boot_transform((PyObject*)code,TYPE_ENCODE) != 0)
            return NULL;
    }
    return code;
}


static PyObject* _Py_HOT_FUNCTION
eval_proxy(PyFrameObject *f, int throwflag){
    FAST_FAIL_RETURN(ori_eval == NULL,
            "guard must be initialized before exec",NULL);

    if(!IS_ENCODE(f->f_code->co_flags))
        return ori_eval(f,throwflag);

    PyCodeObject *code = default_decoder(f->f_code);
    if(code != NULL){
        f->f_code = code;
    }

    PyObject *ret = ori_eval(f,throwflag);

    code = default_encoder(f->f_code);
    if(code != NULL)
        f->f_code = code;

    return ret;
}


static int guard_init(){

    PyThreadState *tstate = PyThreadState_GET();
    _PyFrameEvalFunction eval = tstate->interp->eval_frame;

    FAST_FAIL_RETURN(ori_eval == eval,
            "can not init guard more thand once",-1);

    if(eval != eval_proxy){
        ori_eval = eval;
        tstate->interp->eval_frame = eval_proxy;
    }

    return 0;
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


PyObject* PY_MODULE_INIT(py_guard){


    PyMethodDef* methods[2]={
            &METHOD_INFO_set_decode,
            &METHOD_INFO_set_encode
    };

    if(regiester_methods(Module_Def,2,methods)!=2){
        FAST_FAIL_RETURN(1,"create module py_guard fail",NULL);
    }

    PyObject *m = PyModule_Create(Module_Def);

    if(m == NULL || m == Py_None || guard_init() != 0){
        Py_XDECREF(m);
        return NULL;
    }

    return m;
}

