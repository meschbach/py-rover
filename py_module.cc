#include<Python.h>
#include <moduleobject.h>
#include<iostream>
#include <numpy/arrayobject.h>


#include "a_star.hpp"
#include "point.hpp"

using namespace std;
using namespace rover;

template<typename PyType>
struct PyRef {
    PyType ref;
    PyRef() : ref(NULL) {}
    PyRef( PyType aRef ) : ref(aRef) {}
    ~PyRef(){
//        Py_XDECREF(ref);
    }
    operator bool() const { return ref != NULL; }
    PyType operator *() const { return ref; }
};

extern "C" {

//TODO: Probably leaking references here, should probably audit for 'em
//TODO: Error handling
static char astar_docstring[] = "Finds path using a variant of the A* algorithm";
static PyObject* py_astar( PyObject* self, PyObject *args ){
	try {
        PyObject* terrian;
        int current_x, current_y, target_x, target_y;

        if( !PyArg_ParseTuple(args, "O(ii)(ii)", &terrian, &current_x, &current_y, &target_x, &target_y ) ){
            cout << "Invalid argument format" << endl;
            return NULL;
        }
        PyRef<PyObject*> terrian_array = PyArray_FROM_OTF(terrian, NPY_INT64, NPY_IN_ARRAY);
        if( !terrian_array ){
            return NULL;
        }
        auto dimensions = PyArray_NDIM(*terrian_array);
        if( dimensions != 2 ){
            cout << "Expected dimension == 2, got " << dimensions << endl;
            return NULL;
        }

        auto dims = PyArray_DIMS(*terrian_array);
        size_t row_count = dims[0];
        size_t column_count = dims[1];

        /*
         * Construct the board
         */
    //	cout << "setting up the board" << endl;
    //	Board board;
    //	board.width = column_count;
    //	board.height = row_count;
        BitBoard board(column_count, row_count);
        for( size_t row = 0; row < row_count; row++ ){
            for( size_t column = 0; column < column_count; column++){
                long* cell = (long*)PyArray_GETPTR2(*terrian_array, row, column);
                if( *cell ){
                    board.set_wall(column,row);
                }
            }
        }

        /*
         * Run algorithm
         */
        AStar<BitBoard> pathfinding;
        Point goal(target_x,target_y), current(current_x,current_y);
    //	cout << "Starting pathfinding from "<< current << " to " << goal << endl;
    //	cout << board << endl;

        std::vector<Point> path = pathfinding.findFor(board, current, goal);

    //	cout << "Ending pathfinding" << endl;

        //TOOD: Optimize for setting each element instead of appending
        PyObject* results = PyList_New(0);
        if( results == NULL ){
            cerr << "[EE] failed to allocate list";
            abort();
        }

    //    cout << "Resulting path: ";
        for( auto points = path.begin(); points != path.end(); points++){
            Point where = *points;
    //	    cout << " ("<<  where << ") ";
            PyObject* point = Py_BuildValue("II", where.first, where.second);

            if( point == NULL ){
                cerr << "Failed to construct point" << endl;
                abort();
            }
            Py_INCREF(point);

            int append_result = PyList_Append(results, point);
            if( append_result == -1 ){
                cerr << "Failed to append point" << endl;
                abort();
            }
        }
        Py_INCREF(results);
        return results;
	}catch(std::exception &e){
		cerr << "WARNING: Exceptional condition encountered " << e.what() << endl;
		abort();
	}catch(...){
		cerr << "FATAL: Unexpected exception encountered" << endl;
		abort();
	}
}

//#define PATHFINDER_USE_PY2
#ifdef PATHFINDER_USE_PY2 

static char module_docstring[] = "Pathfinding algorithms implemented natively";
static PyMethodDef module_methods[] = {
	{"a_star", py_astar, METH_VARARGS, astar_docstring},
	{NULL, NULL, 0, NULL}
};


PyMODINIT_FUNC init_pathfinder( void ){
//	cout << "[Pathfinder] Initializing" << endl;
	PyObject *module = Py_InitModule( "_pathfinder", module_methods );
	if( module == NULL ){ return NULL; }

	import_array();
	return module;
}

#else

static PyMethodDef SpamMethods[] = {
    {"a_star",  py_astar, METH_VARARGS, astar_docstring},
    {NULL, NULL, 0, NULL}        /* Sentinel */
};

static struct PyModuleDef pathfinder_module = {
   PyModuleDef_HEAD_INIT,
   "_pathfinder",   /* name of module */
   "Pathfinding algorithms", /* module documentation, may be NULL */
   -1,       /* size of per-interpreter state of the module,
                or -1 if the module keeps state in global variables. */
   SpamMethods
};

PyMODINIT_FUNC
PyInit__rover(void)
{
    auto module = PyModule_Create( &pathfinder_module );
		import_array()
		return module;
}

#endif

}
