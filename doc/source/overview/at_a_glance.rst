.. _at-a-glance:

.. todo:: Possibly remove this section in favor of a more thorough tutorial sequence.

at a glance
----------------
A brief run down of ecto is in order.

    * Simple processing node interface for building your own modules.

    .. code-block:: c++

        #include <ecto/ecto.hpp>
        using ecto::tendrils;
        struct MyCell
        {
          static void declare_params(tendrils& params);
          static void declare_io(const tendrils& params, tendrils& in, tendrils& out);
          void configure(tendrils& params, tendrils& in, tendrils& out);
          int process(tendrils& in, tendrils& out);
        };


    * Inputs, outputs and parameters are templated, and type erasing,
      giving runtime typesafety and the ability to use your own data types..

    .. code-block:: c++

        void MyCell::declare_params(tendrils& params)
        {
            params.declare<Foo>("foo","Foo is for spam. This is a doc string", Foo(3.14));
            params.declare<std::string>("str", "str is a standard string.", "default");
        }

    * Python is used as the plugin architecture of ecto.
      Exposing your modules to python is dead simple.
      The use of boost::python means that the python bindings for your data types are an
      optional powerful tool.

    .. code-block:: c++

        //ecto registration
        ECTO_DEFINE_MODULE(hello_ecto){}
        ECTO_CELL(hello_ecto, Printer, "Printer", "Prints a string input to standard output.");
        ECTO_CELL(hello_ecto, Reader, "Reader", "Reads input from standard input.");

    * ecto forces your cells to be self documenting, and allows full
      introspection from python and c++, including
      type names, docstrings and variable names.
    * The plasm (DAG) executes in compiled code.
    * Python is used for declaring the processing graph,
      or as its known to ecto, the *plasm*.

    .. code-block:: python

      import ecto #ecto core library
      import hello_ecto #a user library, that has a few ecto modules

      #instantiate a plasm, our DAG structure
      plasm = ecto.Plasm()

      #instantiate processing modules
      r = hello_ecto.Reader()

      #notice the keyword args, these get mapped
      #as parameters
      p = hello_ecto.Printer(str="default")

      #connect outputs to inputs
      plasm.connect(r["output] >> p["str"])

      #an execution loop
      print "Enter input, q to quit"
      while r.outputs.output != 'q':
        plasm.execute() #this executes the graph


    * The ecto::plasm is easily inspected using graphviz tools.

    .. ectoplot:: ../sampleplasm.py plasm

    * Cells are self documenting by design.

    .. ectocell:: hello_ecto Printer
