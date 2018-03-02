#include "btree.h"

#ifdef PYTHON_BUILD

#include <pybind11/pybind11.h>

namespace py = pybind11;

PYBIND11_MODULE(bestbst, m) {

    py::class_<BTree<int, int>>(m, "BTree")

        .def(py::init<>())

        .def("insert", py::overload_cast<const int&, const int&>(&BTree<int, int>::insert))
        .def("print", &BTree<int, int>::print)
        .def("size", &BTree<int, int>::size)
        .def("clear", &BTree<int, int>::clear)
        .def("erase", &BTree<int, int>::erase)
        .def("is_balanced", &BTree<int, int>::is_balanced)
        .def("balance", &BTree<int, int>::balance)
        .def("height", py::overload_cast<>(&BTree<int, int>::height))

        ;

}

#endif
