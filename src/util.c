/*  Some of the code in this file was extracted from the Python interpreter.
    We are therefore including the following copyright notice:

    Copyright (c) 2001, 2002, 2003, 2004, 2005, 2006, 2007, 2008, 2009, 2010,
    2011, 2012, 2013, 2014, 2015, 2016, 2017, 2018 Python Software Foundation;
    All Rights Reserved.
*/


#include "util.h"


void
gc_list_init(PyGC_Head* list) {
    list->gc.gc_prev = list;
    list->gc.gc_next = list;
}


int
gc_list_is_empty(PyGC_Head* list) {
    return (list->gc.gc_next == list);
}


void
gc_list_merge(PyGC_Head* from, PyGC_Head* to) {
    PyGC_Head* tail;
    assert(from != to);
    if (!gc_list_is_empty(from)) {
        tail = to->gc.gc_prev;
        tail->gc.gc_next = from->gc.gc_next;
        tail->gc.gc_next->gc.gc_prev = tail;
        to->gc.gc_prev = from->gc.gc_prev;
        to->gc.gc_prev->gc.gc_next = to;
    }
    gc_list_init(from);
}


Py_ssize_t
gc_list_size(PyGC_Head* list) {
    PyGC_Head* gc;
    Py_ssize_t n = 0;
    for (gc = list->gc.gc_next; gc != list; gc = gc->gc.gc_next) {
        n++;
    }
    return n;
}
