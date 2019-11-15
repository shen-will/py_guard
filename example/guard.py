import boot
import py2pyx
import os
import shutil
import logging
py2pyx.set_encode(boot.encode)
py2pyx.set_decode(boot.decode)

#project entry
entry = "app.py"
py_boot = "boot.py"

output_dir = "guard"

def clear_workspace():
    if os.path.exists(output_dir):
        for dirpath,dirs,filenames in os.walk(output_dir):
            for name in filenames:
                os.remove(os.path.join(dirpath,name))
    else:
        os.mkdir(output_dir)
    pass

def build():

    clear_workspace()

    # project source excluce entry
    project_src = []
    for dirpath, dirnames, filenames in os.walk("./"):
        for fname in filenames:
            if not fname.endswith(".py") or fname == entry or \
                    fname == py_boot or fname == "guard.py" or fname == "setup.py":
                continue
            project_src.append(os.path.join(dirpath, fname))

    logging.basicConfig(level=logging.DEBUG, format='%(levelname)s: %(message)s')

    logging.info("start encode....")

    for src in project_src:
        if not src.endswith(".py"):
            continue
        logging.info(src)
        pyc_path = src + "c"

        if os.path.exists(pyc_path):
            os.remove(pyc_path)

        py2pyx.py2pyx(src)

        pyc_guard = os.path.join(output_dir, os.path.basename(pyc_path))
        if os.path.exists(pyc_path):
            shutil.move(pyc_path, pyc_guard)

        if not os.path.exists(pyc_guard):
            logging.error(src + " encode fail")
            exit(-1)

    logging.info(py_boot)
    py2pyx.boot_encode(py_boot)
    pyc_boot_path = py_boot + 'c'
    if not os.path.exists(pyc_boot_path):
        logging.error(py_boot + ' encode fail')
        exit(-1)
    shutil.move(pyc_boot_path, os.path.join(output_dir, os.path.basename(pyc_boot_path)))

    logging.info(entry)
    shutil.copy(entry, os.path.join(output_dir, os.path.basename(entry)))

    # logging.info("py_guard.so")
    # shutil.copy("py_guard.so",os.path.join(output_dir,os.path.basename("py_guard.so")))

    logging.info("[success! %d file encode to %s]" % (len(project_src), output_dir))

    pass


if __name__ == "__main__":
    build()

