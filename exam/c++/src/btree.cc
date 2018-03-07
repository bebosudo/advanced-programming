#include "btree.h"

#ifdef PYTHON_BUILD

#include <pybind11/pybind11.h>

namespace py = pybind11;

PYBIND11_MODULE(bestbst, m) {
    py::class_<BTree<int, int>::iterator>(m, "iterator")

        .def(py::init<BTree<int, int> *>())

        .def("__eq__", &BTree<int, int>::iterator::operator==);

    py::class_<BTree<int, int>>(m, "BTree")

        .def(py::init<>())

        .def("insert", py::overload_cast<const int &, const int &>(&BTree<int, int>::insert))
        .def("print", &BTree<int, int>::print)
        .def("size", &BTree<int, int>::size)
        .def("clear", &BTree<int, int>::clear)
        .def("erase", &BTree<int, int>::erase)
        .def("find", &BTree<int, int>::find)
        .def("is_balanced", &BTree<int, int>::is_balanced)
        .def("balance", &BTree<int, int>::balance)
        .def("height", [](const BTree<int, int> &t) {
                return t.height();
        })

        .def("__len__", &BTree<int, int>::size)

        /*
        .def("__iter__", [](BTree<int, int> &t) {
            return py::make_key_iterator( t.begin(), t.end());
        }, py::keep_alive<0, 1>())
        */
        ;
}

#endif
