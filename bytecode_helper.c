//
// Created by Hasee on 2019/11/12.
//

#include "bytecode_helper.h"
#include "marshal.h"
#include "py_module_helper.h"

static size_t read_bytes(const char *path,char *buf){

    FILE *file = fopen(path,"rb");

    size_t count = 0;
    fseek(file,0,SEEK_END);
    count = ftell(file);
    fseek(file,0,SEEK_SET);

    if(buf == NULL)
        goto ok;

    if(fread(buf,1,count,file) != count){
        count = 0;
    }

ok:
    fclose(file);

    return count;
}

PyObject * py2code(const char *file)
{
    PyObject *blt_name = Unicode("builtins");
    PyObject *blt = PyImport_GetModule(blt_name);
    DESTROY_OBJECT(blt_name);

    CHECK_ERROR_RETURN(blt == NULL,"builtin moudle not import",NULL);

    PyObject *dict = PyModule_GetDict(blt);
    PyObject *builtin_compile = PyDict_GetItemString(dict,"compile");

    size_t count = read_bytes(file,NULL);
    char *buf = PyMem_Malloc(count+1);
    read_bytes(file,buf);
    buf[count] =0;
    PyObject *code_str = PyBytes_FromString(buf);
    PyObject *path = Unicode(file);
    PyObject *mode = Unicode("exec");

    PyObject *args = PyTuple_New(3);
    PyTuple_SetItem(args,0,code_str);
    PyTuple_SetItem(args,1,path);
    PyTuple_SetItem(args,2,mode);

    PyObject *ret  =PyObject_Call(builtin_compile,args,NULL);

    if(!PyObject_TypeCheck(ret,&PyCode_Type)){
        Py_XDECREF(ret);
        ret = NULL;
    }

    Py_XDECREF(args);
    Py_XDECREF(blt);

    PyMem_Free(buf);
    return ret;

}


int code2pyc(PyObject *code,const char *path){

    if(code == NULL || path == NULL)
        return -1;

    FILE *file = fopen(path,"wb+");

    //char magic[4]={0x42,0x0d,0x0d,0x0a};
    long magic = PyImport_GetMagicNumber();
    char empty[4] = {0};

    fwrite(&magic,1,sizeof(long),file);
    fwrite(empty,1,4,file);
    fwrite(empty,1,4,file);
    fwrite(empty,1,4,file);


    PyMarshal_WriteObjectToFile(code,file,1);

    if(PyErr_Occurred()){
        fclose(file);
        return -1;
    }

    fflush(file);
    fclose(file);

    return 0;
}

