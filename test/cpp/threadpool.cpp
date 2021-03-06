#include <gtest/gtest.h>
#include <ecto/ecto.hpp>
#include <ecto/plasm.hpp>

#include <ecto/schedulers/threadpool.hpp>

using namespace ecto;

namespace {

  plasm::ptr makeplasm()
  {
    plasm::ptr p(new plasm);
    cell::ptr ping(registry::create("Ping"));
    cell::ptr sleep0(registry::create("Sleep"));
    cell::ptr sleep1(registry::create("Sleep"));
    sleep0->parameters["seconds"] << 0.1;
    sleep1->parameters["seconds"] << 0.1;

    p->connect(ping, "out", sleep0, "in");
    p->connect(sleep0, "out", sleep1, "in");
    return p;
  }

}
TEST(Threadpool, CreateAndDestroy)
{
  plasm::ptr p = makeplasm();
  schedulers::threadpool* tp = new schedulers::threadpool(p);
  delete tp;
}


TEST(Threadpool, DestroyWhileRunning)
{
  plasm::ptr p = makeplasm();
  schedulers::threadpool* tp = new schedulers::threadpool(p);
  tp->execute_async();
  delete tp;
}

TEST(Threadpool, DestroyAfterRunning)
{
  plasm::ptr p = makeplasm();
  schedulers::threadpool* tp = new schedulers::threadpool(p);
  tp->execute(2,2);
  EXPECT_FALSE(tp->running());
  delete tp;
}

TEST(Threadpool, WaitAndDestroy)
{
  plasm::ptr p = makeplasm();
  schedulers::threadpool* tp = new schedulers::threadpool(p);
  tp->execute_async(2,4);
  EXPECT_TRUE(tp->running());
  tp->wait();
  EXPECT_FALSE(tp->running());
  delete tp;
}

TEST(Threadpool, StopAndDestroy)
{
  plasm::ptr p = makeplasm();
  schedulers::threadpool* tp = new schedulers::threadpool(p);
  tp->execute_async(2,4);
  EXPECT_TRUE(tp->running());
  tp->stop();
  delete tp;
}

