#include <Python.h>

#include <stdio.h>

#include "freeze.h"


static PyMethodDef gc_freeze_methods[] = {
    {
        "freeze",
        (PyCFunction)gc_freeze,
        METH_NOARGS,
        "Freeze all the objects tracked by gc - move them to a permanent generation "
        "and ignore all the future collections. This can be used before a POSIX "
        "fork() call to make the gc copy-on-write friendly or to speed up collection. "
        "Also collection before a POSIX fork() call may free pages for future "
        "allocation which can cause copy-on-write too so it's advised to disable gc "
        "in the master process and freeze before fork and enable gc in the child process."
    },
    {
        "unfreeze",
        (PyCFunction)gc_unfreeze,
        METH_NOARGS,
        "Unfreeze the objects in the permanent generation, put them back into the "
        "oldest generation."
    },
    {
        "get_freeze_count",
        (PyCFunction)gc_get_freeze_count,
        METH_NOARGS,
        "Return the number of objects in the permanent generation."
    },
    {
        "get_actual_count",
        (PyCFunction)gc_get_actual_count,
        METH_NOARGS,
        "Return the actual number of objects in each generation as a tuple. "
        "The native gc.count() method returns the `count` value of each generation, "
        "but that value might not be equal to the actual number of elements in the list. "
        "DO NOT use this function for purposes other than testing as get_actual_count() "
        "cycles through the entire generation at each call."
    },
    {NULL, NULL, 0, NULL}
};


// python 3.x
#if PY_MAJOR_VERSION >= 3
static struct PyModuleDef moduledef = {
    PyModuleDef_HEAD_INIT,
    "gc_freeze",
    "Garbage collection freezing packaged as an extension for versions of "
    "Python before 3.7.",
    -1,
    gc_freeze_methods,
    NULL,
    NULL,
    NULL,
    NULL,
};


PyMODINIT_FUNC
PyInit_gc_freeze(void) {
    init_freeze();
    return PyModule_Create(&moduledef);
}


// python 2.x
#else
PyMODINIT_FUNC
initgc_freeze(void)
{
    init_freeze();
    Py_InitModule3(
        "gc_freeze",
        gc_freeze_methods,
        "Garbage collection freezing packaged as an extension for versions of "
        "Python before 3.7."
    );
}
#endif
