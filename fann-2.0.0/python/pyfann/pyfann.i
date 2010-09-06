/* File : fann.i */
%module libfann

%include "typemaps.i"

%{
#include "fann.h"
#include "fann_io.h"
#include "fann_train.h"
#include "fann_data.h"
#include "fann_cascade.h"
#include "fann_error.h"
#include "fann_activation.h"
%}

%define CHECKED_FLOAT_ARRAY(typemap_name, expected_length)
%typemap(in) typemap_name {
  int i;
  if (!PySequence_Check($input)) {
    PyErr_SetString(PyExc_ValueError,"Expected a sequence");
    SWIG_fail;
  }
  if (PySequence_Length($input) != expected_length) {
    PyErr_SetString(PyExc_ValueError,"Sequence has wrong length");
    SWIG_fail;
  }
  $1 = (float *) malloc(PySequence_Length($input)*sizeof(float));
  for (i = 0; i < PySequence_Length($input); i++) {
    PyObject *o = PySequence_GetItem($input,i);
    if (PyNumber_Check(o)) {
      $1[i] = (float) PyFloat_AsDouble(o);
    } else {
      PyErr_SetString(PyExc_ValueError,"Sequence elements must be numbers");      
      Py_DECREF(o);
      SWIG_fail;
    }
    Py_DECREF(o);
  }
}
%typemap(freearg) typemap_name {
   if ($1) free($1);
}
%enddef

CHECKED_FLOAT_ARRAY(fann_type *input, arg1->num_input)
CHECKED_FLOAT_ARRAY(fann_type *desired_output, arg1->num_output)

%typemap(in) int[ANY] {
  int i;
  if (!PySequence_Check($input)) {
    PyErr_SetString(PyExc_ValueError,"Expected a sequence");
    SWIG_fail;
  }
  if (PySequence_Length($input) == 0) {
    PyErr_SetString(PyExc_ValueError,"Size mismatch. Expected some elements");
    SWIG_fail;
  }
  $1 = (unsigned int *) malloc(PySequence_Length($input)*sizeof(unsigned int));
  for (i = 0; i < PySequence_Length($input); i++) {
    PyObject *o = PySequence_GetItem($input,i);
    if (PyNumber_Check(o)) {
      $1[i] = (int) PyInt_AsLong(o);
    } else {
      PyErr_SetString(PyExc_ValueError,"Sequence elements must be numbers");      
      Py_DECREF(o);
      SWIG_fail;
    }
    Py_DECREF(o);
  }
}
%typemap(freearg) int[ANY] {
   if ($1) free($1);
}
%apply int[ANY] {int *, unsigned int*};

typedef double fann_type; 

%typemap(out) PyObject* {
  $result = $1;
}

// create_array is used instead
%ignore fann_create;
%ignore fann_create_shortcut;

%rename(fann_run_old) fann_run;
%rename(fann_run) fann_run2;

%rename(fann_test_old) fann_test;
%rename(fann_test) fann_test2;

#define FANN_INCLUDE
%include "../../src/include/fann.h"
%include "../../src/include/fann_data.h"
%include "../../src/include/fann_activation.h"
%include "../../src/include/fann_train.h"
%include "../../src/include/fann_io.h"
%include "../../src/include/fann_cascade.h"
%include "../../src/include/fann_error.h"

// Helper functions
PyObject* fann_run2(struct fann *ann, fann_type *input);
PyObject* fann_test2(struct fann *ann, fann_type *input, fann_type *desired_output);
PyObject* get_train_data_input(struct fann_train_data *ann, int row);
PyObject* get_train_data_output(struct fann_train_data *ann, int row);
int fann_is_NULL(struct fann *ann);
