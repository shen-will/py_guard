from distutils.core import *

py2pyx_src = ["py2pyx.c","boot.c", "bytecode_helper.c","py_module_helper.c","transform.c"]
py2pyx_name = "py2pyx"

py_guard_src = ["py_guard.c","boot.c", "bytecode_helper.c","py_module_helper.c","transform.c"]
py_guard_name = "py_guard"

modules=[
Extension(py2pyx_name,py2pyx_src),
Extension(py_guard_name,py_guard_src),

]

DEBUG = True
setup(name="py-guard",version="1.0",ext_modules=modules)