/*
 * Copyright (c) 2011, Willow Garage, Inc.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 *     * Redistributions of source code must retain the above copyright
 *       notice, this list of conditions and the following disclaimer.
 *     * Redistributions in binary form must reproduce the above copyright
 *       notice, this list of conditions and the following disclaimer in the
 *       documentation and/or other materials provided with the distribution.
 *     * Neither the name of the Willow Garage, Inc. nor the names of its
 *       contributors may be used to endorse or promote products derived from
 *       this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */
#include <ecto/plasm.hpp>
#include <ecto/tendril.hpp>
#include <ecto/cell.hpp>
#include <ecto/graph_types.hpp>

#include <string>
#include <map>
#include <set>
#include <utility>
#include <deque>



namespace ecto {

  namespace schedulers {
    
    struct ECTO_EXPORT singlethreaded 
    {
      singlethreaded(plasm::ptr);
      singlethreaded(plasm&);
      ~singlethreaded();

      int execute(unsigned niter=0);
      void execute_async(unsigned niter=0);

      void stop();
      void interrupt();
      bool running() const;
      void wait();

    private:

      int execute_impl(unsigned niter);

      int invoke_process(ecto::graph::graph_t::vertex_descriptor vd);
      void compute_stack();

      plasm::ptr plasm_;
      ecto::graph::graph_t& graph;
      boost::thread runthread;
      
      int last_rval;
      std::vector<ecto::graph::graph_t::vertex_descriptor> stack;
      mutable boost::mutex iface_mtx;
      mutable boost::mutex running_mtx;

    };
  }
}
