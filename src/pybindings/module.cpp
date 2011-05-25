#include <ecto/module.hpp>
#include <ecto/ecto.hpp>

#include <boost/python.hpp>
#include <boost/python/raw_function.hpp>
#include <ecto/python/std_map_indexing_suite.hpp>

#include <ecto/python/raw_constructor.hpp>
#include <ecto/python/repr.hpp>

namespace bp = boost::python;

namespace ecto
{
namespace py
{
#if 0
#undef SHOW
#define SHOW() do{}while(false)
#endif

struct modwrap: module, bp::wrapper<module>
{

  void dispatch_declare_params(tendrils& params)
  {
    SHOW();
    if (bp::override init = this->get_override("declare_params"))
      init(boost::ref(params));
    else
      throw std::logic_error("declare_params is not implemented it seems");
  }

  void dispatch_declare_io(const tendrils&params, tendrils&inputs, tendrils&outputs)
  {
    SHOW();
    if (bp::override config = this->get_override("declare_io"))
      config(boost::ref(params), boost::ref(inputs), boost::ref(outputs));
    else
      throw std::logic_error("declare_io is not implemented it seems");
  }

  void dispatch_configure(tendrils& params)
   {
     SHOW();
     if (bp::override config = this->get_override("configure"))
       config(boost::ref(params));
     else
       throw std::logic_error("configure is not implemented it seems");
   }

  ReturnCode dispatch_process(const tendrils& inputs, tendrils& outputs)
  {
    SHOW();
    if (bp::override proc = this->get_override("process"))
    {
      proc(boost::ref(inputs), boost::ref(outputs));
    }
    else
      throw std::logic_error("process is not implemented it seems");
    return eOK;
  }
  void dispatch_destroy()
  {
    SHOW();
    if (bp::override dest = this->get_override("destroy"))
      dest();
    else
      throw std::logic_error("destroy is not implemented it seems");
  }
  const std::string& name() const
  {
    SHOW();
    bp::reference_existing_object::apply<modwrap*>::type converter;
    PyObject* obj = converter(this);
    bp::object real_obj = bp::object(bp::handle<>(obj));
    bp::object n = real_obj.attr("__class__").attr("__name__");
    static std::string nm = bp::extract<std::string>(n);
    return nm;
  }
  static std::string doc(modwrap* mod)
  {
    SHOW();
    bp::reference_existing_object::apply<modwrap*>::type converter;
    PyObject* obj = converter(mod);
    bp::object real_obj = bp::object(bp::handle<>(obj));
    bp::object n = real_obj.attr("__class__").attr("__doc__");
    std::string nm = bp::extract<std::string>(n);
    return nm;
  }
  boost::function<void()> finish_handler_;
};

const tendrils& inputs(module& mod)
{
  return mod.inputs;
}
tendrils& outputs(module& mod)
{
  return mod.outputs;
}
tendrils& params(module& mod)
{
  return mod.parameters;
}
void wrapModule()
{
  //use private names so that python people know these are internal
  bp::class_<module, boost::shared_ptr<module>, boost::noncopyable>("_module_cpp", bp::no_init);

  bp::class_<modwrap, boost::shared_ptr<modwrap>, boost::noncopyable> m_base("_module_base" /*bp::no_init*/);
  m_base.def("declare_params", &module::declare_params);
  m_base.def("declare_io", ((void(module::*)()) &module::declare_io));
  m_base.def("configure", ((void(module::*)()) &module::configure));
  m_base.def("process", (void(module::*)()) &module::process);
  m_base.def("destroy", &module::destroy);

  m_base.add_property("inputs", make_function(&inputs, bp::return_internal_reference<>()));
  m_base.add_property("outputs", make_function(outputs, bp::return_internal_reference<>()));
  m_base.add_property("params", make_function(params, bp::return_internal_reference<>()));
  m_base.add_property("name", make_function(params, bp::return_internal_reference<>()));
  m_base.def("doc", &modwrap::doc);
}

}
}

