/*
 * theo_peak.hpp
 *
 *  Created on: Nov 28, 2013
 *      Author: xunlikun
 */

#ifndef THEO_PEAK_HPP_
#define THEO_PEAK_HPP_

#include "base/ion.hpp"
#include "base/bp_spec.hpp"
#include "base/activation.hpp"
#include "spec/peak.hpp"

namespace prot {

class TheoPeak : public Peak {
public:
	TheoPeak(IonPtr ion,double unmode_mass,double shift);
	IonPtr getIon(){return ion_;}
	double getModMass(){return getPosition();}
	double getShift(){return shift_;}
private:
	double unmod_mass_;
	double shift_;

	IonPtr ion_;
};
typedef std::shared_ptr<TheoPeak> TheoPeakPtr;
typedef std::vector<TheoPeakPtr> TheoPeakPtrVec;

inline bool theopeak_up(const TheoPeakPtr p,TheoPeakPtr n){
  return p->getPosition() < n->getPosition();
}

inline bool theopeak_down(const TheoPeakPtr p,TheoPeakPtr n){
  return p->getPosition() > n->getPosition();
}

TheoPeakPtrVec getTheoPeak(BpSpecPtr pep,ActivationPtr type,double n_term_shift,double c_term_shift,int bgn,int end,double min_mass);
} /* namespace prot */

#endif /* THEO_PEAK_HPP_ */