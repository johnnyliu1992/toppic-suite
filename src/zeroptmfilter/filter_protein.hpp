#ifndef PROT_FILTER_PROTEIN_HPP_
#define PROT_FILTER_PROTEIN_HPP_

#include <memory>
#include <vector>

namespace prot {

class FilterProtein {
 public:
  FilterProtein(int protein_id, int score):
      protein_id_(protein_id),
      score_(score) {
      }
  int getProteinId() {return protein_id_;}
  int getScore() {return score_;}
  std::vector<double> getNTermShifts() {return n_term_shifts_;}
  std::vector<double> getCTermShifts() {return c_term_shifts_;}

  void setNTermShifts(std::vector<double> shifts) {n_term_shifts_ = shifts;}
  void setCTermShifts(std::vector<double> shifts) {c_term_shifts_ = shifts;}

 private:
  int protein_id_;
  int score_;
  std::vector<double> n_term_shifts_;
  std::vector<double> c_term_shifts_;
};

typedef std::shared_ptr<FilterProtein> FilterProteinPtr;
typedef std::vector<FilterProteinPtr> FilterProteinPtrVec;

} /* namespace prot */

#endif 