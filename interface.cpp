#include <Python.h>
#include <QtCore>

#include "diff_match_patch.h"

static PyObject *
diff_match_patch_diff(PyObject *self, PyObject *args, PyObject *kwargs)
{
    const char *a, *b;
    float timelimit = 0.0;
    int checklines = 1;

    
    static char *kwlist[] = { "left_document", "right_document", "timelimit", "checklines", NULL };

    if (!PyArg_ParseTupleAndKeywords(args, kwargs, "ss|fb", kwlist,
                                     &a, &b, &timelimit, &checklines))
    return NULL;
    
    PyObject *ret = PyList_New(0);
    
    PyObject *opcodes[3];
    opcodes[DELETE] = PyString_InternFromString("-");
    opcodes[INSERT] = PyString_InternFromString("+");
    opcodes[EQUAL]  = PyString_InternFromString("=");
    
    diff_match_patch dmp = diff_match_patch();
    dmp.Diff_Timeout = timelimit;
    QList<Diff> diff = dmp.diff_main(QString(a), QString(b), checklines);
    foreach(Diff entry, diff) {
        PyObject* tuple = PyTuple_New(2);
        PyTuple_SetItem(tuple, 0, opcodes[entry.operation]);
        PyTuple_SetItem(tuple, 1, PyInt_FromLong(entry.text.length()));
    	PyList_Append(ret, tuple);
    }
    
    return ret;
}

static PyMethodDef SpamMethods[] = {
    {"diff", (PyObject* (*)(PyObject*, PyObject*))diff_match_patch_diff, METH_VARARGS|METH_KEYWORDS,
    "Compute the difference between two strings. Returns a list of tuples (OP, LEN)."},
    {NULL, NULL, 0, NULL}        /* Sentinel */
};

PyMODINIT_FUNC
initdiff_match_patch(void)
{
    (void) Py_InitModule("diff_match_patch", SpamMethods);
}

