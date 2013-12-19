/*
 * ptm_fast_filter_block.cpp
 *
 *  Created on: Dec 1, 2013
 *      Author: xunlikun
 */

#include <iostream>
#include "ptm_fast_filter_block.hpp"

namespace prot {

PtmFastFilterBlock::PtmFastFilterBlock(ProteoformPtrVec seqs,PtmFastFilterMngPtr mng){
	mng_= mng;
	seqs_=seqs;
	initSeqBlocks();
}
void PtmFastFilterBlock::initSeqBlocks(){
	int start = 0;
	int pos =0;
	int len=0;
//	seq_blocks_
	while(start < seqs_.size()){
		if(pos <seqs_.size() && len+seqs_[pos]->getResSeqPtr()->getLen() < mng_->db_block_size_){
			len = len+seqs_[pos]->getResSeqPtr()->getLen();
			pos++;
		}
		else{
			int end = pos;
			if(end == seqs_.size()){
				end --;
			}
			ProteoformPtrVec seqs_in_block;
			for(int i =start;i<=end;i++){
				seqs_in_block.push_back(seqs_[i]);
			}
			seq_blocks_.push_back(seqs_in_block);
			start = end +1;
			pos = end +1;
			len = 0;
		}
	}
}

SimplePrSMPtrVec PtmFastFilterBlock::getBestMathBatch(SpectrumSetPtr spectrum_set){
	SimplePrSMPtrVec result;
	PrmMsPtr ms = spectrum_set->getSpTwo();
	SimplePrSMPtrVec fast_match_list = filter_->getBestMatch(ms);
	for(unsigned int i=0;i<fast_match_list.size();i++){
		if(i>= mng_->ptm_fast_filter_result_num_){
			break;
		}
		result.push_back(fast_match_list[i]);
	}
	return result;
}

} /* namespace prot */
