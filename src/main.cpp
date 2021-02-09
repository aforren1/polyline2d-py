#include <pybind11/pybind11.h>
#include <pybind11/numpy.h>
#include <Polyline2D.h>

namespace py = pybind11;
using namespace pybind11::literals;
using p2d = crushedpixel::Polyline2D;

struct Vec2
{
    float x;
    float y;
};

// https://github.com/pybind/pybind11/issues/1042#issuecomment-642215028
template <typename Sequence>
inline py::array_t<typename Sequence::value_type> as_pyarray(Sequence &&seq)
{
    auto size = seq.size();
    auto data = seq.data();
    std::unique_ptr<Sequence> seq_ptr = std::make_unique<Sequence>(std::move(seq));
    auto capsule = py::capsule(seq_ptr.get(), [](void *p) { std::unique_ptr<Sequence>(reinterpret_cast<Sequence *>(p)); });
    seq_ptr.release();
    return py::array(size, data, capsule);
}

py::array_t<Vec2> create(py::array_t<Vec2> points, float thickness,
                         p2d::JointStyle jointStyle, p2d::EndCapStyle endCapStyle,
                         bool allowOverlap)
{
    // blah, copies input data
    return as_pyarray(p2d::create<Vec2>(std::vector<Vec2>(points.data(), points.data() + points.size()),
                                        thickness, jointStyle, endCapStyle, allowOverlap));
}

PYBIND11_MODULE(polyline2d, m)
{
    m.doc() = "pybind11 wrapper for CrushedPixel/Polyline2D";
    py::enum_<p2d::JointStyle>(m, "JointStyle", py::arithmetic())
        .value("MITER", p2d::JointStyle::MITER)
        .value("BEVEL", p2d::JointStyle::BEVEL)
        .value("ROUND", p2d::JointStyle::ROUND);

    py::enum_<p2d::EndCapStyle>(m, "EndCapStyle", py::arithmetic())
        .value("BUTT", p2d::EndCapStyle::BUTT)
        .value("SQUARE", p2d::EndCapStyle::SQUARE)
        .value("ROUND", p2d::EndCapStyle::ROUND)
        .value("JOINT", p2d::EndCapStyle::JOINT);

    PYBIND11_NUMPY_DTYPE(Vec2, x, y);
    // https://github.com/CrushedPixel/Polyline2D/blob/master/include/Polyline2D.h#L85

    m.def("create", &create, "points"_a, "thickness"_a,
          "joint_style"_a = p2d::JointStyle::MITER,
          "end_cap_style"_a = p2d::EndCapStyle::BUTT,
          "allow_overlap"_a = false);
}