//
// Created by Hasee on 2019/10/21.
//

#include "Python.h"
#include "py_module_helper.h"

int regiester_methods(PyModuleDef *moduleDef,unsigned int count,PyMethodDef *infos[]){

    if(moduleDef == NULL)
        return -1;

    PyMethodDef *methods = NULL;

    if(count == 0 || infos == NULL){
        methods = calloc(sizeof(PyMethodDef),1);
        moduleDef->m_methods = methods;
        return count;
    }

    methods = calloc(sizeof(PyMethodDef) * (count+1),1);

    unsigned int i = 0;
    for (;i<count;i++){

        methods[i] = *infos[i];
    }

    moduleDef->m_methods = methods;

    return count;
}
