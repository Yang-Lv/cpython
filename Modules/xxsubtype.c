#include "Python.h"
#include "structmember.h"

/* Examples showing how to subtype the builtin list and dict types from C. */

/* spamlist -- a list subtype */

typedef struct {
	PyListObject list;
	int state;
} spamlistobject;

static PyObject *
spamlist_getstate(spamlistobject *self, PyObject *args)
{
	if (!PyArg_ParseTuple(args, ":getstate"))
		return NULL;
	return PyInt_FromLong(self->state);
}

static PyObject *
spamlist_setstate(spamlistobject *self, PyObject *args)
{
	int state;

	if (!PyArg_ParseTuple(args, "i:setstate", &state))
		return NULL;
	self->state = state;
	Py_INCREF(Py_None);
	return Py_None;
}

static PyMethodDef spamlist_methods[] = {
	{"getstate", (PyCFunction)spamlist_getstate, METH_VARARGS,
	 	"getstate() -> state"},
	{"setstate", (PyCFunction)spamlist_setstate, METH_VARARGS,
	 	"setstate(state)"},
	{NULL,	NULL},
};

staticforward PyTypeObject spamlist_type;

static int
spamlist_init(spamlistobject *self, PyObject *args, PyObject *kwds)
{
	if (PyList_Type.tp_init((PyObject *)self, args, kwds) < 0)
		return -1;
	self->state = 0;
	return 0;
}

static PyObject *
spamlist_state_get(spamlistobject *self)
{
	return PyInt_FromLong(self->state);
}

static struct getsetlist spamlist_getsets[] = {
	{"state", (getter)spamlist_state_get, NULL, NULL},
	{0}
};

static PyTypeObject spamlist_type = {
	PyObject_HEAD_INIT(&PyType_Type)
	0,
	"xxsubtype.spamlist",
	sizeof(spamlistobject),
	0,
	0,					/* tp_dealloc */
	0,					/* tp_print */
	0,					/* tp_getattr */
	0,					/* tp_setattr */
	0,					/* tp_compare */
	0,					/* tp_repr */
	0,					/* tp_as_number */
	0,					/* tp_as_sequence */
	0,					/* tp_as_mapping */
	0,					/* tp_hash */
	0,					/* tp_call */
	0,					/* tp_str */
	0,					/* tp_getattro */
	0,					/* tp_setattro */
	0,					/* tp_as_buffer */
	Py_TPFLAGS_DEFAULT | Py_TPFLAGS_BASETYPE, /* tp_flags */
	0,					/* tp_doc */
	0,					/* tp_traverse */
	0,					/* tp_clear */
	0,					/* tp_richcompare */
	0,					/* tp_weaklistoffset */
	0,					/* tp_iter */
	0,					/* tp_iternext */
	spamlist_methods,			/* tp_methods */
	0,					/* tp_members */
	spamlist_getsets,			/* tp_getset */
	&PyList_Type,				/* tp_base */
	0,					/* tp_dict */
	0,					/* tp_descr_get */
	0,					/* tp_descr_set */
	0,					/* tp_dictoffset */
	(initproc)spamlist_init,		/* tp_init */
	0,					/* tp_alloc */
	0,					/* tp_new */
};

/* spamdict -- a dict subtype */

typedef struct {
	PyDictObject dict;
	int state;
} spamdictobject;

static PyObject *
spamdict_getstate(spamdictobject *self, PyObject *args)
{
	if (!PyArg_ParseTuple(args, ":getstate"))
		return NULL;
	return PyInt_FromLong(self->state);
}

static PyObject *
spamdict_setstate(spamdictobject *self, PyObject *args)
{
	int state;

	if (!PyArg_ParseTuple(args, "i:setstate", &state))
		return NULL;
	self->state = state;
	Py_INCREF(Py_None);
	return Py_None;
}

static PyMethodDef spamdict_methods[] = {
	{"getstate", (PyCFunction)spamdict_getstate, METH_VARARGS,
	 	"getstate() -> state"},
	{"setstate", (PyCFunction)spamdict_setstate, METH_VARARGS,
	 	"setstate(state)"},
	{NULL,	NULL},
};

staticforward PyTypeObject spamdict_type;

static int
spamdict_init(spamdictobject *self, PyObject *args, PyObject *kwds)
{
	if (PyDict_Type.tp_init((PyObject *)self, args, kwds) < 0)
		return -1;
	self->state = 0;
	return 0;
}

static struct memberlist spamdict_members[] = {
	{"state", T_INT, offsetof(spamdictobject, state), READONLY},
	{0}
};

static PyTypeObject spamdict_type = {
	PyObject_HEAD_INIT(&PyType_Type)
	0,
	"xxsubtype.spamdict",
	sizeof(spamdictobject),
	0,
	0,					/* tp_dealloc */
	0,					/* tp_print */
	0,					/* tp_getattr */
	0,					/* tp_setattr */
	0,					/* tp_compare */
	0,					/* tp_repr */
	0,					/* tp_as_number */
	0,					/* tp_as_sequence */
	0,					/* tp_as_mapping */
	0,					/* tp_hash */
	0,					/* tp_call */
	0,					/* tp_str */
	0,					/* tp_getattro */
	0,					/* tp_setattro */
	0,					/* tp_as_buffer */
	Py_TPFLAGS_DEFAULT | Py_TPFLAGS_BASETYPE, /* tp_flags */
	0,					/* tp_doc */
	0,					/* tp_traverse */
	0,					/* tp_clear */
	0,					/* tp_richcompare */
	0,					/* tp_weaklistoffset */
	0,					/* tp_iter */
	0,					/* tp_iternext */
	spamdict_methods,			/* tp_methods */
	spamdict_members,			/* tp_members */
	0,					/* tp_getset */
	&PyDict_Type,				/* tp_base */
	0,					/* tp_dict */
	0,					/* tp_descr_get */
	0,					/* tp_descr_set */
	0,					/* tp_dictoffset */
	(initproc)spamdict_init,		/* tp_init */
	0,					/* tp_alloc */
	0,					/* tp_new */
};

PyObject *
spam_bench(PyObject *self, PyObject *args)
{
	PyObject *obj, *name, *res;
	int n = 1000;
	time_t t0, t1;

	if (!PyArg_ParseTuple(args, "OS|i", &obj, &name, &n))
		return NULL;
	t0 = clock();
	while (--n >= 0) {
		res = PyObject_GetAttr(obj, name);
		if (res == NULL)
			return NULL;
		Py_DECREF(res);
	}
	t1 = clock();
	return PyFloat_FromDouble((double)(t1-t0) / CLOCKS_PER_SEC);
}

static PyMethodDef xxsubtype_functions[] = {
	{"bench",	spam_bench, 	METH_VARARGS},
	{NULL,		NULL}		/* sentinel */
};

DL_EXPORT(void)
initxxsubtype(void)
{
	PyObject *m, *d;

	m = Py_InitModule("xxsubtype", xxsubtype_functions);
	if (m == NULL)
		return;

	if (PyType_Ready(&spamlist_type) < 0)
		return;
	if (PyType_Ready(&spamdict_type) < 0)
		return;

	d = PyModule_GetDict(m);
	if (d == NULL)
		return;

	Py_INCREF(&spamlist_type);
	if (PyDict_SetItemString(d, "spamlist",
				 (PyObject *) &spamlist_type) < 0)
		return;

	Py_INCREF(&spamdict_type);
	if (PyDict_SetItemString(d, "spamdict",
				 (PyObject *) &spamdict_type) < 0)
		return;
}
