#ifndef PROT_FEATURE_MSALIGN_WRITER_HPP_
#define PROT_FEATURE_MSALIGN_WRITER_HPP_

#include <iostream>
#include <fstream>

#include "spec/ms_header.hpp"
#include "feature/match_env.hpp"

namespace prot {

class MsalignWriter {
 public:
  static void writeText(std::ofstream &file, MatchEnvPtrVec &envs, 
                        MsHeaderPtr header_ptr, int id);
};

}
#endif