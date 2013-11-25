
#include "acid_util.hpp"
#include "trunc.hpp"

namespace prot {

Trunc::Trunc(std::string name, int trunc_len, 
                   AcidPtrVec &acid_ptr_vec, std::string acid_str) {
  name_ = name;
  trunc_len_ = trunc_len;
  shift_ = 0;
  for (unsigned int i = 0; i < acid_str.length(); i++) {
    std::string letter = acid_str.substr(i, 1);
    AcidPtr acid_ptr = getAcidPtrByOneLetter(acid_ptr_vec, letter);
    acid_ptr_str_.push_back(acid_ptr);
    shift_ = shift_ - acid_ptr->getMonoMass();
  }
}
}