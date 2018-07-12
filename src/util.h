/*  Some of the code in this file was extracted from the Python interpreter.
    We are therefore including the following copyright notice:

    Copyright (c) 2001, 2002, 2003, 2004, 2005, 2006, 2007, 2008, 2009, 2010,
    2011, 2012, 2013, 2014, 2015, 2016, 2017, 2018 Python Software Foundation;
    All Rights Reserved.
*/


#ifndef UTIL_H
#define UTIL_H

#include "Python.h"


void
gc_list_init(PyGC_Head* list);


int
gc_list_is_empty(PyGC_Head* list);


void
gc_list_merge(PyGC_Head* from, PyGC_Head* to);


Py_ssize_t
gc_list_size(PyGC_Head* list);


#endif
