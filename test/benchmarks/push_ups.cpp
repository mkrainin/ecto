#include <ecto/ecto.hpp>
#include <iostream>
#include <boost/format.hpp>
#include <boost/foreach.hpp>

#include <vector>
#include <numeric>

namespace ecto_push_ups
{
  size_t big_data()
  {
    std::vector<int> data;
    data.resize(10e4);
    for (size_t i = 0; i < 10e4; i++)
      data[i] = std::rand();
    return std::accumulate(data.begin(), data.end(), 0);
  }
  int add2(int x, int y)
  {
    return x + y;
  }

  struct Add2 : ecto::module
  {
    void Config()
    {
      //SHOW();
      o().declare<int> ("out", "x+y");
      i().declare<int>("x");
      i().declare<int>("y");
    }
    void Process()
    {
      //SHOW();
      o().get<int> ("out") = add2(std::rand(),std::rand());
    }
    static void Params(tendrils_t& p)
    {
      SHOW();
    }
  };

  struct BigData : ecto::module
  {
    void Config()
    {
      SHOW();
      o().declare<int> ("out", "sum of random array");
    }
    void Process()
    {
      o().get<int> ("out") = big_data();
    }
    static void Params(tendrils_t& p)
    {
      SHOW();
    }
  };
}

BOOST_PYTHON_MODULE(push_ups)
{
  using namespace ecto_push_ups;
  ecto::wrap<Add2>("Add2", "An adder.");
  ecto::wrap<BigData>("BigData", "BigData...");
}
