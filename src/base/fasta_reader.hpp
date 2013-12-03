#ifndef PROT_FASTA_READER_HPP_
#define PROT_FASTA_READER_HPP_

#include <iostream>
#include <fstream>
#include <string>
#include <vector>

#include "base/residue_seq.hpp"
#include "base/proteoform.hpp"

namespace prot {

class FastaReader {
 public:
  /**
   * Constructs an instance with a File.
   **/
  FastaReader(const char* file_name);

  /**
   * Read FASTA file and return next protein
   * name and sequence. result[0] is
   * protein name and result[1] is sequence.
   **/                  
  std::vector<std::string> getNextSeq();

  ProteoformPtr getNextProteoformPtr(AcidPtrVec acid_list, 
                                  ResiduePtrVec residue_list);
 private:
  std::ifstream input_;
  std::string ori_name_;
};

std::vector<std::string> fastaPreprocess(std::string name, std::string seq);

ProteoformPtrVec readFastaToProteoform(std::string file_name, 
                                       AcidPtrVec &acid_list, ResiduePtrVec &residue_list);
}

#endif
