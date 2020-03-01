#pragma once

/*
 * Copyright (c) 2019, Yung-Yu Chen <yyc@solvcon.net>
 * BSD-style license; see COPYING
 */

#include <pybind11/pybind11.h> // Must be the first include.
#include <pybind11/stl.h>
#include <pybind11/numpy.h>
#define NPY_NO_DEPRECATED_API NPY_1_7_API_VERSION
#include <numpy/arrayobject.h>

#include "modmesh/modmesh.hpp"
#include "modmesh/python/common.hpp"

#ifdef __GNUG__
#  define MODMESH_PYTHON_WRAPPER_VISIBILITY __attribute__((visibility("hidden")))
#else
#  define MODMESH_PYTHON_WRAPPER_VISIBILITY
#endif

namespace modmesh
{

namespace python
{

class
MODMESH_PYTHON_WRAPPER_VISIBILITY
WrapWrapperProfilerStatus
  : public WrapBase< WrapWrapperProfilerStatus, WrapperProfilerStatus >
{

public:

    static constexpr char PYNAME[] = "WrapperProfilerStatus";
    static constexpr char PYDOC[] = "WrapperProfilerStatus";

    friend root_base_type;

protected:

    WrapWrapperProfilerStatus(pybind11::module & mod) : root_base_type(mod)
    {

        namespace py = pybind11;

        (*this)
            .def_property_readonly_static
            (
                "me"
              , [](py::object const &) -> wrapped_type& { return wrapped_type::me(); }
            )
            .def_property_readonly("enabled", &wrapped_type::enabled)
            .def("enable", &wrapped_type::enable)
            .def("disable", &wrapped_type::disable)
        ;

        mod.attr("wrapper_profiler_status") = mod.attr("WrapperProfilerStatus").attr("me");

    }

}; /* end class WrapWrapperTimerStatus */

class
MODMESH_PYTHON_WRAPPER_VISIBILITY
WrapStopWatch
  : public WrapBase< WrapStopWatch, StopWatch >
{

public:

    static constexpr char PYNAME[] = "StopWatch";
    static constexpr char PYDOC[] = "StopWatch";

    friend root_base_type;

protected:

    WrapStopWatch(pybind11::module & mod) : root_base_type(mod)
    {

        namespace py = pybind11;

        (*this)
            .def_property_readonly_static
            (
                "me"
              , [](py::object const &) -> wrapped_type& { return wrapped_type::me(); }
            )
            .def("lap", &wrapped_type::lap)
            .def_property_readonly("duration", &wrapped_type::duration)
            .def_property_readonly_static
            (
                "resolution"
              , [](py::object const &) -> double { return wrapped_type::resolution(); }
            )
        ;

        mod.attr("stop_watch") = mod.attr("StopWatch").attr("me");

    }

}; /* end class WrapStopWatch */

class
MODMESH_PYTHON_WRAPPER_VISIBILITY
WrapTimeRegistry
  : public WrapBase< WrapTimeRegistry, TimeRegistry >
{

public:

    static constexpr char PYNAME[] = "TimeRegistry";
    static constexpr char PYDOC[] = "TimeRegistry";

    friend root_base_type;

protected:

    WrapTimeRegistry(pybind11::module & mod) : root_base_type(mod)
    {

        namespace py = pybind11;

        (*this)
            .def_property_readonly_static
            (
                "me"
              , [](py::object const &) -> wrapped_type& { return wrapped_type::me(); }
            )
            .def("unset", &wrapped_type::unset)
            .def_property_readonly("names", &wrapped_type::names)
            .def("report", &wrapped_type::report)
        ;

        mod.attr("time_registry") = mod.attr("TimeRegistry").attr("me");

    }

}; /* end class WrapTimeRegistry */

class
MODMESH_PYTHON_WRAPPER_VISIBILITY
WrapConcreteBuffer
  : public WrapBase< WrapConcreteBuffer, ConcreteBuffer, std::shared_ptr<ConcreteBuffer> >
{

    friend root_base_type;

    WrapConcreteBuffer(pybind11::module & mod, char const * pyname, char const * pydoc)
      : root_base_type(mod, pyname, pydoc, pybind11::buffer_protocol())
    {

        namespace py = pybind11;

        (*this)
            .def_timed
            (
                py::init
                (
                    [](size_t nbytes)
                    {
                        return ConcreteBuffer::construct(nbytes);
                    }
                )
              , py::arg("nbytes")
            )
            .def_timed("clone", &wrapped_type::clone)
            .def_property_readonly("nbytes", &wrapped_type::nbytes)
            .def("__len__", &wrapped_type::size)
            .def_timed(
                "__getitem__"
              , [](wrapped_type const & self, size_t it) { return self.at(it); }
            )
            .def_timed(
                "__setitem__"
              , [](wrapped_type & self, size_t it, int8_t val)
                {
                    self.at(it) = val;
                }
            )
            .def_buffer
            (
                [](wrapped_type & self)
                {
                    return py::buffer_info
                    (
                        self.data() /* Pointer to buffer */
                      , sizeof(int8_t) /* Size of one scalar */
                      , py::format_descriptor<char>::format() /* Python struct-style format descriptor */
                      , 1 /* Number of dimensions */
                      , { self.size() } /* Buffer dimensions */
                      , { 1 } /* Strides (in bytes) for each index */
                    );
                }
            )
            .def_property_readonly_timed
            (
                "ndarray"
              , [](wrapped_type & self)
                {
                    namespace py = pybind11;
                    return py::array
                    (
                        py::detail::npy_format_descriptor<int8_t>::dtype() /* Numpy dtype */
                      , { self.size() } /* Buffer dimensions */
                      , { 1 } /* Strides (in bytes) for each index */
                      , self.data() /* Pointer to buffer */
                      , py::cast(self.shared_from_this()) /* Owning Python object */
                    );
                }
            )
        ;
    }

}; /* end class WrapConcreteBuffer */

template <typename T>
class
MODMESH_PYTHON_WRAPPER_VISIBILITY
WrapSimpleArray
  : public WrapBase< WrapSimpleArray<T>, SimpleArray<T> >
{

    using root_base_type = WrapBase< WrapSimpleArray<T>, SimpleArray<T> >;
    using wrapped_type = typename root_base_type::wrapped_type;
    using shape_type = typename wrapped_type::shape_type;

    friend root_base_type;

    WrapSimpleArray(pybind11::module & mod, char const * pyname, char const * pydoc)
      : root_base_type(mod, pyname, pydoc, pybind11::buffer_protocol())
    {

        namespace py = pybind11;

        (*this)
            .def_timed
            (
                py::init
                (
                    [](py::object const & shape)
                    {
                        return wrapped_type(make_shape(shape));
                    }
                )
              , py::arg("shape")
            )
            .def_buffer
            (
                [](wrapped_type & self)
                {
                    std::vector<size_t> stride;
                    for (size_t i : self.stride())
                    {
                        stride.push_back(i*sizeof(T));
                    }
                    return py::buffer_info
                    (
                        self.data() /* Pointer to buffer */
                      , sizeof(T) /* Size of one scalar */
                      , py::format_descriptor<T>::format() /* Python struct-style format descriptor */
                      , self.ndim() /* Number of dimensions */
                      , std::vector<size_t>(self.shape().begin(), self.shape().end()) /* Buffer dimensions */
                      , stride /* Strides (in bytes) for each index */
                    );
                }
            )
            .def_property_readonly_timed
            (
                "ndarray"
              , [](wrapped_type const & self)
                {
                    namespace py = pybind11;
                    std::vector<size_t> shape(self.shape().begin(), self.shape().end());
                    std::vector<size_t> stride(self.stride().begin(), self.stride().end());
                    for(size_t & v: stride) { v *= self.itemsize(); }
                    return py::array
                    (
                        py::detail::npy_format_descriptor<T>::dtype() /* Numpy dtype */
                      , shape /* Buffer dimensions */
                      , stride /* Strides (in bytes) for each index */
                      , self.data() /* Pointer to buffer */
                      , py::cast(self.buffer().shared_from_this()) /* Owning Python object */
                    );
                }
            )
            .def_property_readonly("nbytes", &wrapped_type::nbytes)
            .def_property_readonly("size", &wrapped_type::size)
            .def_property_readonly("itemsize", &wrapped_type::itemsize)
            .def_property_readonly
            (
                "shape"
              , [](wrapped_type const & self)
                {
                    py::tuple ret(self.shape().size());
                    for (size_t i=0; i<self.shape().size(); ++i)
                    {
                        ret[i] = self.shape()[i];
                    }
                    return ret;
                }
            )
            .def_property_readonly
            (
                "stride"
              , [](wrapped_type const & self)
                {
                    py::tuple ret(self.stride().size());
                    for (size_t i=0; i<self.stride().size(); ++i)
                    {
                        ret[i] = self.stride()[i];
                    }
                    return ret;
                }
            )
            .def("__len__", &wrapped_type::size)
            .def_timed
            (
                "__getitem__"
              , [](wrapped_type const & self, py::object const & key)
                {
                    try
                    {
                        size_t const it = key.cast<size_t>();
                        return self.at(it);
                    }
                    catch (const py::cast_error &)
                    {
                        shape_type const idx(key.cast<std::vector<size_t>>());
                        return self.at(idx);
                    }
                }
            )
            .def_timed
            (
                "__setitem__"
              , [](wrapped_type & self, py::object const & key, T val)
                {
                    try
                    {
                        size_t const it = key.cast<size_t>();
                        self.at(it) = val;
                    }
                    catch (const py::cast_error &)
                    {
                        shape_type const idx(key.cast<std::vector<size_t>>());
                        self.at(idx) = val;
                    }
                }
            )
            .def_timed
            (
                "reshape"
              , [](wrapped_type const & self, py::object const & shape)
                {
                    return self.reshape(make_shape(shape));
                }
            )
        ;

    }

    static shape_type make_shape(pybind11::object const & shape_in)
    {
        namespace py = pybind11;
        shape_type shape;
        try
        {
            shape.push_back(shape_in.cast<size_t>());
        }
        catch (const py::cast_error &)
        {
            shape = shape_in.cast<std::vector<size_t>>();
        }
        return shape;
    }

}; /* end class WrapSimpleArray */

template< typename Wrapper, typename GT >
class
MODMESH_PYTHON_WRAPPER_VISIBILITY
WrapStaticGridBase
  : public WrapBase< Wrapper, GT >
{

public:

    using base_type = WrapBase< Wrapper, GT >;
    using wrapped_type = typename base_type::wrapped_type;

    using serial_type = typename wrapped_type::serial_type;
    using real_type = typename wrapped_type::real_type;

    friend typename base_type::root_base_type;

protected:

    WrapStaticGridBase(pybind11::module & mod) : base_type(mod)
    {

        namespace py = pybind11;

        (*this)
            .def_property_readonly_static
            (
                "NDIM"
              , [](py::object const &) { return wrapped_type::NDIM; }
            )
        ;

    }

}; /* end class WrapStaticGridBase */

class
MODMESH_PYTHON_WRAPPER_VISIBILITY
WrapStaticGrid1d
  : public WrapStaticGridBase< WrapStaticGrid1d, StaticGrid1d >
{

public:

    static constexpr char PYNAME[] = "StaticGrid1d";
    static constexpr char PYDOC[] = "StaticGrid1d";

    friend root_base_type;

    using base_type = WrapStaticGridBase< WrapStaticGrid1d, StaticGrid1d >;

protected:

    WrapStaticGrid1d(pybind11::module & mod) : base_type(mod)
    {

        namespace py = pybind11;

        (*this)
            .def_timed
            (
                py::init
                (
                    [](serial_type nx)
                    {
                        return new StaticGrid1d(nx);
                    }
                )
              , py::arg("nx")
            )
            .def
            (
                "__len__"
              , [](wrapped_type const & self) { return self.size(); }
            )
            .def_timed
            (
                "__getitem__"
              , [](wrapped_type const & self, size_t it) { return self.at(it); }
            )
            .def_timed
            (
                "__setitem__"
              , [](wrapped_type & self, size_t it, wrapped_type::real_type val)
                {
                    self.at(it) = val;
                }
            )
            .def_property_readonly
            (
                "nx"
              , [](wrapped_type const & self) { return self.nx(); }
            )
            .def_property_timed
            (
                "coord"
              , [](wrapped_type & self)
                {
                    return py::array
                    (
                        py::detail::npy_format_descriptor<real_type>::dtype()
                      , { self.nx() }
                      , { sizeof(real_type) }
                      , self.coord()
                      , py::cast(self)
                    );
                }
              , [](wrapped_type & self, py::array_t<real_type> arr)
                {
                    for (size_t it=0 ; it < self.nx() ; ++it)
                    {
                        self.at(it) = arr.at(it);
                    }
                }
            )
            .def_timed
            (
                "fill"
              , &wrapped_type::fill
              , py::arg("value")
            )
        ;

    }

}; /* end class WrapStaticGrid1d */

class
MODMESH_PYTHON_WRAPPER_VISIBILITY
WrapStaticGrid2d
  : public WrapStaticGridBase< WrapStaticGrid2d, StaticGrid2d >
{

public:

    static constexpr char PYNAME[] = "StaticGrid2d";
    static constexpr char PYDOC[] = "StaticGrid2d";

    friend root_base_type;

    using base_type = WrapStaticGridBase< WrapStaticGrid2d, StaticGrid2d >;

protected:

    WrapStaticGrid2d(pybind11::module & mod) : base_type(mod)
    {}

}; /* end class WrapStaticGrid2d */

class
MODMESH_PYTHON_WRAPPER_VISIBILITY
WrapStaticGrid3d
  : public WrapStaticGridBase< WrapStaticGrid3d, StaticGrid3d >
{

public:

    static constexpr char PYNAME[] = "StaticGrid3d";
    static constexpr char PYDOC[] = "StaticGrid3d";

    friend root_base_type;

    using base_type = WrapStaticGridBase< WrapStaticGrid3d, StaticGrid3d >;

protected:

    WrapStaticGrid3d(pybind11::module & mod) : base_type(mod)
    {}

}; /* end class WrapStaticGrid3d */

inline void initialize(pybind11::module & mod)
{

    WrapWrapperProfilerStatus::commit(mod);
    WrapStopWatch::commit(mod);
    WrapTimeRegistry::commit(mod);

    WrapConcreteBuffer::commit(mod, "ConcreteBuffer", "ConcreteBuffer");

    WrapSimpleArray<int8_t>::commit(mod, "SimpleArrayInt8", "SimpleArrayInt8");
    WrapSimpleArray<int16_t>::commit(mod, "SimpleArrayInt16", "SimpleArrayInt16");
    WrapSimpleArray<int32_t>::commit(mod, "SimpleArrayInt32", "SimpleArrayInt32");
    WrapSimpleArray<int64_t>::commit(mod, "SimpleArrayInt64", "SimpleArrayInt64");
    WrapSimpleArray<uint8_t>::commit(mod, "SimpleArrayUint8", "SimpleArrayUint8");
    WrapSimpleArray<uint16_t>::commit(mod, "SimpleArrayUint16", "SimpleArrayUint16");
    WrapSimpleArray<uint32_t>::commit(mod, "SimpleArrayUint32", "SimpleArrayUint32");
    WrapSimpleArray<uint64_t>::commit(mod, "SimpleArrayUint64", "SimpleArrayUint64");
    WrapSimpleArray<float>::commit(mod, "SimpleArrayFloat32", "SimpleArrayFloat32");
    WrapSimpleArray<double>::commit(mod, "SimpleArrayFloat64", "SimpleArrayFloat64");

    WrapStaticGrid1d::commit(mod);
    WrapStaticGrid2d::commit(mod);
    WrapStaticGrid3d::commit(mod);

}

} /* end namespace python */

} /* end namespace modmesh */

// vim: set ff=unix fenc=utf8 et sw=4 ts=4 sts=4:
