/*
 * main.hpp
 *
 *  Created on: Jan 28, 2014
 *      Author: xunlikun
 */

#include <iostream>
#include <map>
#include <string>
#include <vector>

#include "base/fasta_reader.hpp"
#include "prsm/prsm_writer.hpp"
#include "prsm/prsm_combine.hpp"
#include "prsm/prsm_selector.hpp"
#include "zeroptmsearch/zero_ptm_mng.hpp"
#include "zeroptmsearch/zero_ptm_search.hpp"
#include "filterdiagonal/ptm_fast_filter_mng.hpp"
#include "filterdiagonal/ptm_fast_filter_processor.hpp"
#include "ptmsearch/ptm_mng.hpp"
#include "ptmsearch/ptm_processor.hpp"
#include "tdgf/tdgf_mng.hpp"
#include "tdgf/evalue_processor.hpp"
#include "prsm/table_writer.hpp"
#include "prsm/output_selector.hpp"
#include "xpp/xml_generator.hpp"
#include "xpp/transformer.hpp"

using namespace prot;
using namespace std;
void showCommondList(){
	cout <<"************************************************"<<endl;
	cout <<"**                  arguments                 **"<<endl;
	cout <<"************************************************"<<endl;
	cout <<"configuration=conf/configuration.xml"<<endl;
	cout <<"databaseFileName=in/prot.fasta"<<endl;
	cout <<"spectrumFileName=in/spectra.msalign"<<endl;
	cout <<"# Activation can be CID, HCD, ETD or FILE (FILE means that the "
			"activation type is given in the spectral data file)."<<endl;
	cout <<"activation=FILE"<<endl;
	cout <<"# SearchType can be TARGET or TARGET+DECOY. When TARGET+DECOY is "
			"used, MS-Align+ will generate a scramble database from the protein "
			"database, and search spectra against the TARGET+DECOY database."<<endl;
	cout <<"searchType=TARGET"<<endl;
	cout <<"#Cysteine protection group can be C0, C57 or C58. C0: no modification, "
			"C57: Carbamidoemetylation or C58:Carboxymethylation"<<endl;
	cout <<"cysteineProtection=C0"<<endl;
	cout <<"# Maximum number of modifications"<<endl;
	cout <<"shiftNumber=2"<<endl;
	cout <<"# Error tolerance in PPM"<<endl;
	cout <<"errorTolerance=15"<<endl;
	cout <<"#CutoffType can be EVALUE or FDR. 'FDR' can be used only if "
			"searchtype=TARGET+DECOY. "<<endl;
	cout <<"cutoffType=EVALUE"<<endl;
	cout <<"# cutoff value. When cutoffType = EValue, this cutoff value "
			"is for EVALUE. When cutoff = FDR, this cutoff value is for FDR."<<endl;
	cout <<"cutoff=0.01"<<endl;
	cout <<"# Correct +/-1 Da errors introduced in the deconvolution of precursor ions."<<endl;
	cout <<"doOneDaltonCorrection=false"<<endl;
	cout <<"# Correct charget state errors introduced in the deconvolution of precursor ions."<<endl;
	cout <<"doChargeCorrection=false"<<endl;
	cout <<"tableOutputFileName=result_table.txt"<<endl;
	cout <<"detailOutputFileName=result_detail.txt"<<endl;
}
map<string,string> getArugmentMap(){
	map<string,string> arguments;
	arguments["databaseFileName"]="in/prot.fasta";
	arguments["spectrumFileName"]="in/spectra.msalign";
	arguments["activation"]="FILE";
	arguments["searchType"]="TARGET";
	arguments["cysteineProtection"]="C0";
	arguments["shiftNumber"]="2";
	arguments["errorTolerance"]="15";
	arguments["cutoffType"]="EVALUE";
	arguments["cutoff"]="0.01";
	arguments["doOneDaltonCorrection"]="false";
	arguments["doChargeCorrection"]="false";
	arguments["tableOutputFileName"]="result_table.txt";
	arguments["detailOutputFileName"]="result_detail.txt";
	arguments["configuration"]="conf/configuration.xml";
	return arguments;
}
bool findArgument(map<string,string> &arguments,string name){
	if(arguments[name].compare("")==0){
		return true;
	}
	return false;
}
void split(std::string& s, std::string& delim,std::vector< std::string >& ret)
{
	size_t last = 0;
	size_t index=s.find_first_of(delim,last);
	while (index!=std::string::npos){
		ret.push_back(s.substr(last,index-last));
		last=index+1;
		index=s.find_first_of(delim,last);
	}
	if (index-last>0)
	{
		ret.push_back(s.substr(last,index-last));
	}
}

void MsAlignPipeline(map<string,string> arguments){

//    std::string config_file_name = "conf/configuration.xml";
//    std::string search_db_file_name = "in/prot.fasta";
//    std::string spectrum_file_name_ = "in/spectra_one.msalign";
//    std::string spectrum_file_name_ = "in/spectra.msalign";

    prot::ZeroPtmMngPtr mng_ptr = prot::ZeroPtmMngPtr(new prot::ZeroPtmMng (arguments));

//    mng_ptr->search_db_file_name_ = search_db_file_name;
//    mng_ptr->spectrum_file_name_ = spectrum_file_name_;
    mng_ptr->output_file_ext_ = "ZERO_PTM_SEARCH";
    prot::zeroPtmSearchProcess(mng_ptr);

    PtmFastFilterMngPtr fast_filter_mng = PtmFastFilterMngPtr(new PtmFastFilterMng(arguments));
//    fast_filter_mng->search_db_file_name_ = search_db_file_name;
//    fast_filter_mng->spectrum_file_name_ = spectrum_file_name_;
    PtmFastFilterProcessorPtr processorb = PtmFastFilterProcessorPtr(new PtmFastFilterProcessor(fast_filter_mng));
    processorb->process();

    PtmMngPtr ptm_search_mng = PtmMngPtr(new PtmMng(arguments));
//    ptm_search_mng->search_db_file_name_ = search_db_file_name;
//    ptm_search_mng->spectrum_file_name_ = spectrum_file_name_;
    ptm_search_mng->input_file_ext_ ="_COMBINED";
    ptm_search_mng->output_file_ext_="PTM_SEARCH_RESULT";
    PtmProcessorPtr processor = PtmProcessorPtr(new PtmProcessor(ptm_search_mng));
    processor->process();

    std::vector<std::string> input_exts ;
    input_exts.push_back("PTM_SEARCH_RESULT");
    input_exts.push_back("ZERO_PTM_SEARCH");
    PrSMCombinePtr combine = PrSMCombinePtr(new PrSMCombine(arguments,
                                                            input_exts,
                                                            "RAW_SEARCH_RESULT"));
    combine->process();


    prot::TdgfMngPtr tdgf_mng = prot::TdgfMngPtr(new prot::TdgfMng (arguments));
//    tdgf_mng->search_db_file_name_ = search_db_file_name;
//    tdgf_mng->spectrum_file_name_ = spectrum_file_name_;
    tdgf_mng->input_file_ext_ = "RAW_SEARCH_RESULT";
    tdgf_mng->output_file_ext_ = "EVALUED_RESULT";
    EValueProcessor evalue_processor(tdgf_mng);
    evalue_processor.init();
    /* compute E-value for a set of prsms each run */
    evalue_processor.process(false);

    PrSMSelector selector = PrSMSelector(arguments,
                                         "EVALUED_RESULT",
                                         "TOP_RESULT",
                                         1,
                                         "e");
    selector.process();

    OutputSelector output_selector = OutputSelector(arguments,
                                                    "TOP_RESULT",
                                                    "OUTPUT_RESULT");
    output_selector.process();

    TableWriterPtr table_out = TableWriterPtr(new TableWriter(arguments,"OUTPUT_RESULT","OUTPUT_TABLE"));
    table_out->write();




//    XmlGenerator xml_gene = XmlGenerator(spectrum_file_name_,search_db_file_name,"TOP_RESULT");
//    xml_gene.process();

//    TransformerPtr trans = TransformerPtr(new Transformer());
//    trans->trans();
//
}

int main(int argc, char* argv[]){

	map<string,string> arguments = getArugmentMap();
	string spliter = "=";
    for(int i=1;i<argc;i++){

        string commond = argv[i];
        if(i==1 && commond.compare("help")==0){
        	showCommondList();
        }
        else{
        	vector<string> commond_arr;
        	split(commond,spliter,commond_arr);
        	std::cout<<commond_arr[0]<<":"<<commond_arr[1]<< std::endl;
        	if(commond_arr.size() == 1){
        		cout<< "argument error! argument:"<<commond_arr[0]<<" have no value!"<<endl;
        		cout<< "see all argument ,use 'help'"<<endl;
        		return 1;
        	}
        	if(findArgument(arguments,commond_arr[0])){
        		cout<< "argument error! argument name:"<<commond_arr[0]<<" is wrong!"<<endl;
        		cout<< "see all argument ,use 'help'"<<endl;
        		return 1;
        	}
        	if(commond_arr.size() >2){
        		cout<< "argument error! argument:"<<commond_arr[0]<<" have wrong format!"<<endl;
        		cout<< "see all argument ,use 'help'"<<endl;
        		return 1;
        	}
        	arguments[commond_arr[0]]=commond_arr[1];
        }
    }

    MsAlignPipeline(arguments);

    return 1;
}



