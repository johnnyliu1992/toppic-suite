/*
 * ptm_processor.cpp
 *
 *  Created on: Dec 20, 2013
 *      Author: xunlikun
 */

#include "ptm_processor.hpp"
#include "prsm/simple_prsm.hpp"
#include "base/fasta_reader.hpp"
#include "spec/deconv_ms.hpp"
#include "prsm/prsm.hpp"
#include "spec/spectrum_set.hpp"
#include "spec/msalign_reader.hpp"

namespace prot {

PtmProcessor::PtmProcessor(PtmMngPtr mng){
	mng_ = mng;
	init();
}

void PtmProcessor::init(){
	seqs_ = prot::readFastaToProteoform(mng_->search_db_file_name_,mng_->base_data->getAcidPtrVec(),mng_->base_data->getResiduePtrVec());
	std::string sp_file_name = mng_->spectrum_file_name_;
	std::string simplePrsmFileName = mng_->spectrum_file_name_ + "." + mng_->input_file_ext_;
	simplePrsms_  = prot::readSimplePrSM(simplePrsmFileName.c_str());
	prsmFindSeq(simplePrsms_,seqs_);
}

void PtmProcessor::prsmFindSeq(SimplePrSMPtrVec simple_prsms,ProteoformPtrVec seqs){
	for(unsigned int i =0;i<simple_prsms.size();i++){
		simple_prsms[i]->findSeq(seqs);
	}
}

void PtmProcessor::process(){
	PtmSearcherPtr searcher = PtmSearcherPtr(new PtmSearcher(mng_));
	processDatabase(searcher);
}

void PtmProcessor::processDatabase(PtmSearcherPtr searcher){
	std::string sp_file_name = mng_->spectrum_file_name_;
	std::string output_file_name = sp_file_name+"."+mng_->output_file_ext_;

	//reader & writer

	MsAlignReader spReader(sp_file_name.c_str(), mng_->base_data->getActivationPtrVec());

//	SimplePrSMPtrVec2D prsms;
//
//	for(unsigned int i =0;i<mng_->n_unknown_shift_;i++){
//		for(unsigned int j=0;j<4;j++){
//
//		}
//	}

	DeconvMsPtr deconv_sp;
	PrSMPtrVec3D prsms;
	int cnt = 0;
	while((deconv_sp = spReader.getNextMs())!= nullptr){
		cnt++;
		for(int i=0;i<deconv_sp->size();i++){
			SpectrumSetPtr spectrumset = prot::getSpectrumSet(deconv_sp,0,mng_->sp_para_,0,mng_->base_data->getIonTypePtrVec());
			if(spectrumset != nullptr){
				std::string scan = deconv_sp->getHeaderPtr()->getScansString();
				//update message;
				SimplePrSMPtrVec slectedPrsms = prot::findSimplePrsms(simplePrsms_,deconv_sp->getHeaderPtr());
				//todo::searcher.search(spectrumset,slectedPrsms.prsms);
				//write
				for(int j=0;j<mng_->n_unknown_shift_;j++){
					for(int k=0;k<4;k++){


					}
				}
			}
		}
	}
}

} /* namespace prot */