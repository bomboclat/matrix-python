#include <Python.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <malloc.h>
#include <emmintrin.h>
//#include "vector.c"
//#include "random.c"

static struct matrix {
    long int **arr;
    int l;
    int h;
};
static struct matrix matrix1;
static struct matrix matrix2;
static struct matrix result;

inline static int fastrand(int g_seed) {
    g_seed = (214013*g_seed+2531011);
    return (g_seed>>16)&0x7FFF;
}

static PyObject* returnTuple2(long int **arr, int *l, int *h) {

    PyObject *Tuple;
    Tuple = PyTuple_New(*h);

    int i, j;
    for(i=0; i<*h; i++) {
        PyObject *V = PyTuple_New(*l);
        #pragma omp parallel for
        for(j=0;j<*l;j++) {
            PyTuple_SetItem(V, j, PyInt_FromLong(arr[i][j]));
        }
        PyTuple_SetItem(Tuple, i, V);
    }
    return Tuple;
    
    //return Py_None;
}

static PyObject* parse(PyObject* self, PyObject* args) {

    PyObject* Tup;
    PyArg_ParseTuple(args, "O!", &PyTuple_Type, &Tup);
    int h = PyTuple_Size(Tup);
    int l = PyTuple_Size(PyTuple_GetItem(Tup, 0));

    long int **arr_ptr;
    arr_ptr = (long int**)malloc(h * sizeof(long int*));

    int i, j;
    for(i=0; i<h; i++) {
        PyObject* TupIn;
        TupIn = PyTuple_GetItem(Tup, i);
        arr_ptr[i] = (long int*)malloc(l * sizeof(long int));
        #pragma omp parallel for
        for(j=0; j<l; j++) {
            PyObject* val;
            val = PyTuple_GetItem(TupIn, j);
            arr_ptr[i][j] = PyInt_AS_LONG(val);
        }
    }

    if (!matrix1.arr) {
        matrix1.arr = arr_ptr;
        matrix1.l = l;
        matrix1.h = h;
    } else if (!matrix2.arr) {
        matrix2.arr = arr_ptr;
        matrix2.l = l;
        matrix2.h = h;
    }
    return Py_None;
}

static long int** parse2(PyObject* args) {

    PyObject* Tup;
    PyArg_ParseTuple(args, "O!", &PyTuple_Type, &Tup);
    int h = PyTuple_Size(Tup);
    int l = PyTuple_Size(PyTuple_GetItem(Tup, 0));

    long int **arr_ptr;
    arr_ptr = (long int**)malloc(h * sizeof(long int*));

    int i, j;
    for(i=0; i<h; i++) {
        PyObject* TupIn;
        TupIn = PyTuple_GetItem(Tup, i);
        arr_ptr[i] = (long int*)malloc(l * sizeof(long int));
        #pragma omp parallel for
        for(j=0; j<l; j++) {
            PyObject* val;
            val = PyTuple_GetItem(TupIn, j);
            arr_ptr[i][j] = PyInt_AS_LONG(val);
        }
    }

    return arr_ptr;
}

static PyObject* generate(PyObject* self, PyObject* args) {

    int h, l;
    PyArg_ParseTuple(args, "ii", &h, &l);
    
    long int **arr_ptr;
    arr_ptr = (long int**)malloc(h * sizeof(long int*));

    int i, j;
    for(i=0; i<h; i++) {
        arr_ptr[i] = (long int*)malloc(l * sizeof(long int));
        #pragma omp parallel for
        for(j=0; j<l; j++) {
            arr_ptr[i][j] = fastrand(clock() + __builtin_omp_get_thread_num());
            printf("%d\n", arr_ptr[i][j]);
        }
    }

    //PyObject* Tuple;
    return returnTuple2(arr_ptr, &h, &l);
    /*
    if (!matrix1.arr) {
        matrix1.arr = arr_ptr;
        matrix1.l = l;
        matrix1.h = h;
    } else if (!matrix2.arr) {
        matrix2.arr = arr_ptr;
        matrix2.l = l;
        matrix2.h = h;
    }
    return Py_None;*/
}

static PyObject* calc(PyObject* self, PyObject* args) {
    result.h = matrix1.h;
    result.l = matrix2.l;
    if (matrix1.l != matrix2.h) {
        PyErr_SetString(PyExc_ValueError, "Invalid matrix format!");
        return NULL;
    }
    result.arr = (long int**)malloc(matrix1.h * sizeof(long int*));
    int i, f, j;
    for(i=0; i<matrix1.h; i++) {
        result.arr[i] = (long int*)malloc(matrix2.l * sizeof(long int));
        for (f=0; f<matrix2.l; f++) {
            long int sum = 0;
            #pragma omp parallel for reduction (+: sum)
            for(j=0; j<matrix1.l; j++) {
                sum += matrix1.arr[i][j] * matrix2.arr[j][f];
            }
            result.arr[i][f] = sum;
        }
    }
    return Py_None;
}

static PyObject* calc2(PyObject* self, PyObject* args) {
    PyObject* Tup;
    PyArg_ParseTuple(args, "O!", &PyTuple_Type, &Tup);
    int size = PyTuple_Size(Tup);
    long int **last_arr;
    int a, i, f, j;

    for(a=0; a<size; a++){
        int h = PyTuple_Size(Tup);
        int l = PyTuple_Size(PyTuple_GetItem(Tup, 0));
        if (matrix1.l != matrix2.h) {
            PyErr_SetString(PyExc_ValueError, "Invalid matrix format!");
            return NULL;
        }
        result.arr = (long int**)malloc(matrix1.h * sizeof(long int*));

        for(i=0; i<matrix1.h; i++) {
            result.arr[i] = (long int*)malloc(matrix2.l * sizeof(long int));
            for (f=0; f<matrix2.l; f++) {
                long int sum = 0;
                #pragma omp parallel for reduction (+: sum)
                for(j=0; j<matrix1.l; j++) {
                    sum += matrix1.arr[i][j] * matrix2.arr[j][f];
                }
                result.arr[i][f] = sum;
            }
        }
    }
    return Py_None;
}

static PyObject* returnTuple(PyObject* self) {

    PyObject *Tuple;
    Tuple = PyTuple_New(result.h);

    int i, j;
    for(i=0; i<result.h; i++) {
        PyObject *V = PyTuple_New(result.l);
        #pragma omp parallel for
        for(j=0;j<result.l;j++) {
            PyTuple_SetItem(V, j, PyInt_FromLong(result.arr[i][j]));
        }
        PyTuple_SetItem(Tuple, i, V);
    }
    return Tuple;
    
    //return Py_None;
}


static PyMethodDef matrixMethods[] =
{
    {"parse", parse, METH_VARARGS, "Python C module"},
    {"generate", generate, METH_VARARGS, "Python C module"},
    {"calc", calc, METH_VARARGS, "Python C module"},
    {"returnTuple", returnTuple, METH_NOARGS, "Python C module"},
    {NULL, NULL, 0, NULL}
};

PyMODINIT_FUNC
initmatrix2(void)
{
    (void) Py_InitModule("matrix2", matrixMethods);
}