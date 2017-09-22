// Copyright (c) 2014 - 2017, The Trustees of Indiana University.
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


#ifndef PROT_DIAGONAL_HEADER_UTIL_HPP_
#define PROT_DIAGONAL_HEADER_UTIL_HPP_

#include <limits>
#include <cmath>

#include "oneptmsearch/diagonal_header.hpp"

namespace prot {

class DiagonalHeaderUtil {
 public:
  // get the header corresponding to the top left corner in the spectral grid
  static DiagonalHeaderPtr getTopLeftCornerHeader() {
    double shift = 0;
    // n_term strict; c_term nostrict; prot n_term match; prot c_term no_match
    // pep n_term no_match; pep c_term no_match
    return std::make_shared<DiagonalHeader>(shift, true, false, true, false, false, false);
  }

  static DiagonalHeaderPtr getBottomRightCornerHeader(double seq_mass,
                                                      double prec_mass) {
    double shift = prec_mass - seq_mass;
    // n term nostrict, c_term strict, prot n_term no match ; prot c_term match
    // pep n_term no match, pep c_term no match 
    return std::make_shared<DiagonalHeader>(shift, false, true, false, true, false, false);
  }

  static void addCornerDiagonals(DiagonalHeaderPtrVec &n_extend_header_ptrs,
                                 DiagonalHeaderPtrVec &c_extend_header_ptrs, 
                                 double seq_mass, double prec_mass) {
    // get top-left corner header in spectral grid (shift is 0)
    DiagonalHeaderPtr top_left_corner_header_ptr = DiagonalHeaderUtil::getTopLeftCornerHeader();
    n_extend_header_ptrs.push_back(top_left_corner_header_ptr);

    // get bottom-right corner header in the spectral grid. 
    DiagonalHeaderPtr bottom_right_corner_header_ptr 
        = DiagonalHeaderUtil::getBottomRightCornerHeader(seq_mass, prec_mass);
    c_extend_header_ptrs.push_back(bottom_right_corner_header_ptr);
  }


  static int findSimilarShiftPos(const std::vector<double> &shifts, double s) {
    int best_pos = -1;
    double best_diff = std::numeric_limits<double>::infinity();
    for(size_t i = 0; i < shifts.size();i++){
      if(std::abs(shifts[i] - s) < best_diff){
        best_pos = i;
        best_diff = std::abs(shifts[i] - s);
      }
    }
    return best_pos;
  }

  static bool isExistHeader(const DiagonalHeaderPtrVec &header_ptrs, double shift) {
    for(size_t i = 0; i < header_ptrs.size();i++){
      if(std::abs(header_ptrs[i]->getProtNTermShift()- shift) <= 0.01) {
        return true;
      }
    }
    return false;
  }
};

} 

#endif
