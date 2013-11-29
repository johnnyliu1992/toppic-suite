/*
 * bp_spec.hpp
 *
 *  Created on: Nov 26, 2013
 *      Author: xunlikun
 */

#ifndef PROT_PROTEOFORM_HPP_
#define PROT_PROTEOFORM_HPP_

namespace prot {

class proteoform {
public:
	BpSpec(){};
	BpSpec(ResSeqPtr res_seq);
	ResSeqPtr getResSeq(){return res_seq_;}
	BreakPointPtrVec getBreakPointPtrVec(){return break_point_ptr_vec_;}
	BreakPointPtr getBreakPointPtr(int i){return break_point_ptr_vec_[i];}
	/*implement the function getExt[B\Y\C\Z_DOT]masses and getNTermMasses and getCTermMass*/
	std::vector<double> getBreakPointMasses(IonTypePtr ion_type_ptr);
	/*implement the function getExt[BY\CZ]masses*/
	std::vector<double> getBreakPointMasses(double n_term_shift,double c_term_shift,double min_mass,IonTypePtr ion_type_ptr_n,IonTypePtr ion_type_ptr_c);
	/*implement the function getScaledBMass*/
	std::vector<int> getScaledMass(double scale,IonTypePtr iong_type);
private:
	ResSeqPtr res_seq_ptr_;
  BpSpecPtr bp_spec_ptr_;

	BreakPointPtrVec break_point_ptr_vec_;
	void initBreakPoints();
	/*implement the private function addMass() BpSpec*/
	void addBreakPointMass(double mass,double seq_mass,double min_mass,std::vector<double> mass_vec);
};

typedef std::shared_ptr<BpSpec> BpSpecPtr;
typedef std::vector<BpSpecPtr> BpSpecPtrVec;

/*come from BpSpec Util*/
int getFirstResPos(double n_term_shift,std::vector<double> extbmasses);
int getLastResPos(double c_term_shift,std::vector<double> extbmasses);

BpSpecPtrVec readDb(ResSeqPtrVec rs_list);

} /* namespace prot */

#endif /* BP_SPEC_HPP_ */
