/*
 * ptm_slow_match.cpp
 *
 *  Created on: Dec 27, 2013
 *      Author: xunlikun
 */

#include <ptmsearch/ptm_slow_match.hpp>

namespace prot {

PtmSlowMatch::PtmSlowMatch(ProteoformPtr seq,SpectrumSetPtr spectrum_set,CompShiftLowMemPtr comp_shift,PtmMngPtr mng){
	mng_=mng;
	deconv_ms_ = spectrum_set->getDeconvMs();
	ms_six_=spectrum_set->getSpSix();
	ms_three = spectrum_set->getSpThree();
	seq_=seq;
	comp(comp_shift);
}

void PtmSlowMatch::comp(CompShiftLowMemPtr comp_shift){
	double scale = mng_->ptm_fast_filter_scale_;
	std::vector<std::vector<int>> sp_masses = prot::getIntMassErrorList(ms_six_,scale,true,false);
	std::vector<double> best_shift= comp_shift->findBestShift(sp_masses[0],sp_masses[1],seq_->getBpSpecPtr()->getScaledMass(scale,IonTypePtr(new IonType("B",true,0))),mng_->n_top_diagonals_,mng_->min_diagonal_gap_,scale);
	DiagonalHeaderPtrVec n_term_shifts =getNTermShiftList(best_shift,ms_six_,seq_,mng_);
	BasicDiagPairDiagPtrVec diagonals =
}

DiagonalHeaderPtrVec PtmSlowMatch::getNTermShiftList(std::vector<double> best_shift,PrmMsPtr ms_six,ProteoformPtr seq,PtmMngPtr mng){
	DiagonalHeaderPtrVec headers = prot::getNTermShiftListCommon(best_shift);
	DiagonalHeaderPtrVec n_term_shifts_comp_left = prot::getNTermShiftListCompLeft(seq,mng);
	DiagonalHeaderPtrVec n_term_shifts_comp_right = prot::getNTermShiftListCompRight(seq,ms_six);
	DiagonalHeaderPtrVec extend_n_term_shifts;
	std::vector<double> ms_masses = prot::getMassList(ms_six);
	std::vector<double> seq_masses = seq->getBpSpecPtr()->getBreakPointMasses(IonTypePtr(new IonType("B",true,0)));
	double shift;

	for(int i=1;i<seq_masses.size();i++){
		shift = - seq_masses[i];
		if(found(shift,headers,mng)){
			extend_n_term_shifts.push_back(DiagonalHeaderPtr(new DiagonalHeader(shift,true,false,true,false)));
		}
	}

	for(int i=1;i<seq_masses.size();i++){
		shift = ms_masses[ms_masses.size()-1] - seq_masses[i];
		if(found(shift,headers,mng)){
			extend_n_term_shifts.push_back(DiagonalHeaderPtr(new DiagonalHeader(shift,true,false,true,false)));
		}
	}

	for(int i=0;i<extend_n_term_shifts.size();i++){
		headers.push_back(extend_n_term_shifts[i]);
	}

	return headers;
}

bool PtmSlowMatch::found(double shift,DiagonalHeaderPtrVec headerlist,PtmMngPtr mng){
	for(int i=0;i<headerlist.size();i++){
		if(std::abs(shift-headerlist[i]->getProtNTermShift())<= mng->extend_diagonal_error_tolerance_){
			return true;
		}
	}
	return false;
}
} /* namespace prot */
