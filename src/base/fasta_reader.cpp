#include <base/logger.hpp>

#include "base/fasta_reader.hpp"

namespace prot {

/** process fasta string and remove unknown letters */
std::string FastaSeq::rmChar(const std::string &ori_seq) {
  std::string seq = "";
  for (unsigned int i = 0; i < ori_seq.length(); i++) {
    char c = ori_seq.at(i);
    if (c < 'A' || c > 'Z') {
      continue;
    }
    char r = c;
    if (c == 'B') {
      r = 'D';
    } else if (c == 'Z') {
      r = 'E';
    } else if (c == 'X') {
      r = 'A';
    }
    seq = seq + r;
  }
  if (ori_seq != seq) { 
    LOG_INFO( "Reading sequence. Unknown letter occurred. ");
  }
  return seq;
}

FastaReader::FastaReader(const std::string &file_name) {
  input_.open(file_name.c_str(), std::ios::in);
  if (!input_.is_open()) {
    LOG_ERROR( "fasta file  " << file_name << " does not exist.");
    throw "fasta file does not exist.";
  }
  std::getline(input_, ori_name_);
}

FastaSeqPtr FastaReader::getNextSeq() {
  if (!input_.is_open()) {
    return FastaSeqPtr(nullptr);
  }

  /* get the letters of sequence */
  std::string ori_seq;
  std::string prot_name = ori_name_.substr(1, ori_name_.size() -1 );
  std::string line;
  while (std::getline(input_, line)) {
    if (line.length() >= 1 && line.substr(0, 1) == ">") {
      ori_name_ = line;
      return FastaSeqPtr(new FastaSeq(prot_name, ori_seq));
    }
    line = trim(line);
    ori_seq = ori_seq + line;
  }
  input_.close();
  return FastaSeqPtr(new FastaSeq(prot_name, ori_seq));
}

/**
 * Read FASTA file and return next protein as an ResSeq.
 * residue_list determine fixed PTMs
 **/
ProteoformPtr FastaReader::getNextProteoformPtr(const ResiduePtrVec &residue_list) {
  FastaSeqPtr seq_ptr = getNextSeq();
  if (seq_ptr.get() == nullptr) {
    return ProteoformPtr(nullptr);
  }
  LOG_TRACE("name " << seq_ptr->getName() << " seq " << seq_ptr->getSeq());
  AcidPtrVec acid_seq = AcidFactory::convertSeqToAcidSeq(seq_ptr->getSeq()); 
  ResiduePtrVec residue_ptrs = convertAcidToResidueSeq(residue_list, acid_seq);
  DbResSeqPtr db_residue_seq_ptr(
      new DbResidueSeq(residue_ptrs, id_, seq_ptr->getName())); 
  id_++;
  return getDbProteoformPtr(db_residue_seq_ptr);
}

ProteoformPtrVec readFastaToProteoform(const std::string &file_name, 
                                       const ResiduePtrVec &residue_list) {
  LOG_DEBUG( "start open file " << file_name);
  FastaReader reader(file_name);
  LOG_DEBUG( "open file done " << file_name);
  ProteoformPtrVec list;
  ProteoformPtr ptr = reader.getNextProteoformPtr(residue_list);
  while (ptr.get() != nullptr) {
    list.push_back(ptr);
    ptr = reader.getNextProteoformPtr(residue_list);
  }
  return list;
}

}

