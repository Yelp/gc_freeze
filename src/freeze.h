/*  Some of the code in this file was extracted from the Python interpreter.
    We are therefore including the following copyright notice:

    Copyright (c) 2001, 2002, 2003, 2004, 2005, 2006, 2007, 2008, 2009, 2010,
    2011, 2012, 2013, 2014, 2015, 2016, 2017, 2018 Python Software Foundation;
    All Rights Reserved.
*/


#ifndef FREEZE_H
#define FREEZE_H

#include "Python.h"


void
init_freeze();


PyObject*
gc_freeze(PyObject* module, PyObject* ignored);


PyObject*
gc_unfreeze(PyObject* module, PyObject* ignored);


PyObject*
gc_get_freeze_count(PyObject* module, PyObject* ignored);


PyObject*
gc_get_actual_count(PyObject* module, PyObject* ignored);


#endif
