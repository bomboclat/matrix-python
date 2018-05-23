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

static struct matrix result;

static int fastrand(int *g_seed) {
    *g_seed = (214013**g_seed+2531011);
    return (*g_seed>>16)&0x7FFF;
}

static PyObject* returnTuple(long int **arr, int *h, int *l) {

    PyObject *Tuple;
    Tuple = PyTuple_New(*h);
    int i, j;
    for(i=0; i<*h; i++) {
        PyObject *V = PyTuple_New(*l);
        //#pragma omp parallel for private (j)
        for(j=0;j<*l;j++) {
            PyTuple_SetItem(V, j, PyInt_FromLong(arr[i][j]));
        }
        PyTuple_SetItem(Tuple, i, V);
    }
    return Tuple;
    
    //return Py_None;
}

static long int** parse(PyObject* Tup) {

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
    int myseed;
    int seed = 123456789;
    for(i=0; i<h; i++) {
        arr_ptr[i] = (long int*)malloc(l * sizeof(long int));
        seed += 100 + clock();
        # pragma omp parallel private ( j, myseed ) shared(i, l, arr_ptr)
        {
            myseed = seed + __builtin_omp_get_thread_num();
            # pragma omp for
            for(j=0; j<l; j++) {
                arr_ptr[i][j] = fastrand(&myseed);
            }
        }
    }

    return returnTuple(arr_ptr, &h, &l);

}

static PyObject* calc(PyObject* self, PyObject* args) {
    PyObject* Tup1;
    PyObject* Tup2;
    Tup1 = PyTuple_GetItem(args, 0);
    Tup2 = PyTuple_GetItem(args, 1);
    int h1 = PyTuple_Size(Tup1);
    int l1 = PyTuple_Size(PyTuple_GetItem(Tup1, 0));
    int h2 = PyTuple_Size(Tup2);
    int l2 = PyTuple_Size(PyTuple_GetItem(Tup2, 0));
    long int **arr1 = parse(Tup1);
    long int **arr2 = parse(Tup2);

    result.h = h1;
    result.l = l2;
    if (l1 != h2) {
        PyErr_SetString(PyExc_ValueError, "Invalid matrix format!");
        return NULL;
    }
    result.arr = (long int**)malloc(h1 * sizeof(long int*));
    int i, f, j;
    for(i=0; i<h1; i++) {
        result.arr[i] = (long int*)malloc(l2 * sizeof(long int));
        for (f=0; f<l2; f++) {
            long int sum = 0;
            #pragma omp parallel for reduction (+: sum)
            for(j=0; j<l1; j++) {
                sum += arr1[i][j] * arr2[j][f];
            }
            result.arr[i][f] = sum;
        }
    }
    return returnTuple(result.arr, &result.h, &result.l);  
}


static PyMethodDef matrixMethods[] =
{
    {"generate", generate, METH_VARARGS, "Python C module"},
    {"calc", calc, METH_VARARGS, "Python C module"},
    {NULL, NULL, 0, NULL}
};

PyMODINIT_FUNC
initmatrix2(void)
{
    (void) Py_InitModule("matrix2", matrixMethods);
}