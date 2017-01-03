// Copyright (c) 2014 - 2016, The Trustees of Indiana University.
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without modification,
// are permitted provided that the following conditions are met:
//
// 1. Redistributions of source code must retain the above copyright notice, this
// list of conditions and the following disclaimer.
//
// 2. Redistributions in binary form must reproduce the above copyright notice,
// this list of conditions and the following disclaimer in the documentation and/or
// other materials provided with the distribution.
//
// Indiana University provides no reassurances that the source code provided does
// not infringe the patent or any other intellectual property rights of any other
// entity. Indiana University disclaims any liability to any recipient for claims
// brought by any other entity based on infringement of intellectual property
// rights or otherwise.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
// ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
// WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
// DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR
// ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
// (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
// LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
// ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
// (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
// SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.


#ifndef PROT_DIST_HPP_
#define PROT_DIST_HPP_

#include <utility>
#include <set>
#include <vector>

#include "graph/graph.hpp"

namespace prot {

class Dist{
 public:
  Dist(MassGraphPtr graph_ptr, int d, int i, int j) {
    graph_ptr_ = graph_ptr;
    dist_ = d;
    pair_ij_.push_back(std::pair<int, int>(i, j));
  }

  bool operator< (const Dist& d) const {
    return dist_ < d.dist_;
  }

  mutable std::vector<std::pair<int, int>> pair_ij_;

  int dist_;

  MassGraphPtr getGraphPtr() {return graph_ptr_;}

 private:
  MassGraphPtr graph_ptr_;
};

typedef std::vector<Dist> DistVec;
typedef std::vector<DistVec> DistVec2D;

inline bool distVecUp(const Dist & a, const Dist & b) {
  return a.dist_ < b.dist_;
}

void addToDistVec(MassGraphPtr graph_ptr,
                  const std::vector<std::vector<std::set<int>>> & dist_vecs,
                  int node_num, int mod_num, DistVec & dist_vec,
                  int gap);

}  // namespace prot

#endif
