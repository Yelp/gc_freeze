/*  Some of the code in this file was extracted from the Python interpreter.
    We are therefore including the following copyright notice:

    Copyright (c) 2001, 2002, 2003, 2004, 2005, 2006, 2007, 2008, 2009, 2010,
    2011, 2012, 2013, 2014, 2015, 2016, 2017, 2018 Python Software Foundation;
    All Rights Reserved.
*/


#include "freeze.h"

#include "util.h"


/* this is the value in a standard Python build */
#define NUM_GENERATIONS 3

/* jumps to the head of generation n */
#define GEN_HEAD(n) (PyGC_Head*)(generations+n)

/* structure used to represent a generation */
struct gc_generation {
    PyGC_Head head;
    int threshold;
    int count;
};

/* pointer to python's gc generations */
static struct gc_generation* generations;

/* a permanent generation for frozen objects */
static struct gc_generation permanent_generation;


/* initializes some static members */
void
init_freeze() {
    /*  this line of code makes everything work. by default, the generations
        structure in the Python interpreter is static and inaccessible. we
        found this extern pointer elsewhere and it points to the head element
        of the list of the first generation. that head element is the first
        element of its parent struct (thus sharing the same address) which is
        itself the first element of its parent array (again, sharing the same
        address). we can just cast straight to an array and give ourselves
        access to this static (inaccessible) member.

        _PyGC_generation0 is declared in cpython/Include/objimpl.h
        and defined in cpython/Modules/gcmodule.c (cpython 2.7).
    */
    generations = (struct gc_generation*)_PyGC_generation0;

    struct gc_generation permanent_generation_init = {
        {{&permanent_generation.head, &permanent_generation.head}}, 0, 0
    };
    permanent_generation = permanent_generation_init;
}


/* freezes everything; moves all objects to permanent generation */
PyObject*
gc_freeze(PyObject* module, PyObject* ignored) {
    int i;
    for (i = 0; i < NUM_GENERATIONS; ++i) {
        gc_list_merge(GEN_HEAD(i), &permanent_generation.head);
        generations[i].count = 0;
    }
    Py_RETURN_NONE;
}


/* unfreezes everything; moves all objects to the oldest generation */
PyObject*
gc_unfreeze(PyObject* module, PyObject* ignored) {
    gc_list_merge(&permanent_generation.head, GEN_HEAD(NUM_GENERATIONS-1));
    Py_RETURN_NONE;
}


/* returns the count of objects in the permanent generation */
PyObject*
gc_get_freeze_count(PyObject* module, PyObject* ignored) {
    int _return_value = gc_list_size(&permanent_generation.head);
    if ((_return_value == -1) && PyErr_Occurred()) {
        return NULL;
    }

    return PyLong_FromLong((long)_return_value);
}


/* returns the count of objects in each generation */
/* NOTE: the native gc.count() method returns the `count` member
   of each generation, but this value does not necessarily match
   the actual size of the list - we will use the method below for
   testing purposes only */
PyObject*
gc_get_actual_count(PyObject* module, PyObject* ignored) {
    int i, count[NUM_GENERATIONS];
    for(i = 0; i < NUM_GENERATIONS; ++i) {
        count[i] = gc_list_size(GEN_HEAD(i));
        if(count[i] == -1 && PyErr_Occurred()) {
            return NULL;
        }
    }

    return Py_BuildValue("(iii)", count[0], count[1], count[2]);
}
