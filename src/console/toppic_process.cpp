//Copyright (c) 2014 - 2017, The Trustees of Indiana University.
//
//Licensed under the Apache License, Version 2.0 (the "License");
//you may not use this file except in compliance with the License.
//You may obtain a copy of the License at
//
//    http://www.apache.org/licenses/LICENSE-2.0
//
//Unless required by applicable law or agreed to in writing, software
//distributed under the License is distributed on an "AS IS" BASIS,
//WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
//See the License for the specific language governing permissions and
//limitations under the License.


#include <iostream>
#include <iomanip>
#include <map>
#include <string>
#include <vector>

#include "base/fasta_reader.hpp"
#include "base/fasta_util.hpp"
#include "base/base_data.hpp"

#include "spec/msalign_reader.hpp"
#include "spec/msalign_util.hpp"

#include "prsm/prsm_para.hpp"
#include "prsm/prsm_str_combine.hpp"
#include "prsm/prsm_top_selector.hpp"
#include "prsm/prsm_cutoff_selector.hpp"
#include "prsm/prsm_species.hpp"
#include "prsm/prsm_table_writer.hpp"
#include "prsm/prsm_fdr.hpp"
#include "prsm/prsm_feature_species.hpp"
#include "prsm/prsm_form_filter.hpp"

#include "zeroptmfilter/zero_ptm_filter_mng.hpp"
#include "zeroptmfilter/zero_ptm_filter_processor.hpp"
#include "zeroptmsearch/zero_ptm_search_mng.hpp"
#include "zeroptmsearch/zero_ptm_search_processor.hpp"

#include "oneptmfilter/one_ptm_filter_mng.hpp"
#include "oneptmfilter/one_ptm_filter_processor.hpp"
#include "oneptmsearch/ptm_search_mng.hpp"
#include "oneptmsearch/one_ptm_search_processor.hpp"

#include "diagfilter/diag_filter_mng.hpp"
#include "diagfilter/diag_filter_processor.hpp"

#include "ptmsearch/ptm_search_processor.hpp"

#include "tdgf/tdgf_mng.hpp"
#include "tdgf/evalue_processor.hpp"

#include "local/local_mng.hpp"
#include "local/local_processor.hpp"

#include "prsmview/xml_generator.hpp"
#include "prsmview/transformer.hpp"

#include "console/toppic_argument.hpp"

namespace prot {

int TopPICProgress(std::map<std::string, std::string> arguments) {
  try {
    std::cout << "TopPIC 1.1.4 (" << __DATE__ << ")" << std::endl;
    std::string exe_dir = arguments["executiveDir"];
    time_t start = time(0);
    char buf[50];
    arguments["start_time"] = std::string(ctime_r(&start, buf));
    Argument::outputArguments(std::cout, arguments);

    BaseData::init(exe_dir);

    LOG_DEBUG("Init base data completed");

    std::string db_file_name = arguments["databaseFileName"];
    std::string sp_file_name = arguments["spectrumFileName"];
    std::string ori_db_file_name = arguments["oriDatabaseFileName"];

    int n_top = std::stoi(arguments["numOfTopPrsms"]);
    int ptm_num = std::stoi(arguments["ptmNumber"]);
    double max_ptm_mass = std::stod(arguments["maxPtmMass"]);
    int filter_result_num = std::stoi(arguments["filteringResultNumber"]);
    int thread_num = std::stoi(arguments["threadNumber"]);

    bool use_gf = false;
    if (arguments["useGf"] == "true") {
      use_gf = true;
    }
    bool localization = false;
    if (arguments["residueModFileName"] != "") {
      localization = true;
    }

    PrsmParaPtr prsm_para_ptr = std::make_shared<PrsmPara>(arguments);
    LOG_DEBUG("prsm para inited");

    bool decoy = false;
    if (arguments["searchType"] == "TARGET+DECOY") {
      decoy = true;
    }
    LOG_DEBUG("block size " << arguments["databaseBlockSize"]);
    int db_block_size = std::stoi(arguments["databaseBlockSize"]);

    FastaUtil::dbPreprocess(ori_db_file_name, db_file_name, decoy, db_block_size);
    MsAlignUtil::geneSpIndex(sp_file_name, prsm_para_ptr->getSpParaPtr());

    std::vector<std::string> input_exts;

    std::cout << "Zero PTM filtering - started." << std::endl;
    ZeroPtmFilterMngPtr zero_filter_mng_ptr
        = std::make_shared<ZeroPtmFilterMng>(prsm_para_ptr, thread_num, "ZERO_FILTER");
    ZeroPtmFilterProcessorPtr zero_filter_processor
        = std::make_shared<ZeroPtmFilterProcessor>(zero_filter_mng_ptr);
    zero_filter_processor->process();
    std::cout << "Zero PTM filtering - finished." << std::endl;

    std::cout << "Zero PTM search - started." << std::endl;
    ZeroPtmSearchMngPtr zero_search_mng_ptr
        = std::make_shared<ZeroPtmSearchMng>(prsm_para_ptr, "ZERO_FILTER", "ZERO_PTM");
    ZeroPtmSearchProcessorPtr zero_search_processor
        = std::make_shared<ZeroPtmSearchProcessor>(zero_search_mng_ptr);
    zero_search_processor->process();
    zero_search_processor = nullptr;
    std::cout << "Zero PTM search - finished." << std::endl;

    input_exts.push_back("ZERO_PTM_COMPLETE");
    input_exts.push_back("ZERO_PTM_PREFIX");
    input_exts.push_back("ZERO_PTM_SUFFIX");
    input_exts.push_back("ZERO_PTM_INTERNAL");

    if (ptm_num >= 1) {
      std::cout << "One PTM filtering - started." << std::endl;
      OnePtmFilterMngPtr one_ptm_filter_mng_ptr
          = std::make_shared<OnePtmFilterMng>(prsm_para_ptr, "ONE_PTM_FILTER");
      OnePtmFilterProcessorPtr one_filter_processor
          = std::make_shared<OnePtmFilterProcessor>(one_ptm_filter_mng_ptr);
      one_filter_processor->process();
      std::cout << "One PTM filtering - finished." << std::endl;

      std::cout << "One PTM search - started." << std::endl;
      int shift_num = 1;
      PtmSearchMngPtr one_search_mng_ptr
          = std::make_shared<PtmSearchMng>(prsm_para_ptr, n_top, max_ptm_mass,
                                           shift_num, thread_num, "ONE_PTM_FILTER", "ONE_PTM");
      OnePtmSearchProcessorPtr one_search_processor
          = std::make_shared<OnePtmSearchProcessor>(one_search_mng_ptr);
      one_search_processor->process();
      one_search_processor = nullptr;
      std::cout << "One PTM search - finished." << std::endl;

      input_exts.push_back("ONE_PTM_COMPLETE");
      input_exts.push_back("ONE_PTM_PREFIX");
      input_exts.push_back("ONE_PTM_SUFFIX");
      input_exts.push_back("ONE_PTM_INTERNAL");
    }

    if (ptm_num >= 2) {
      std::cout << "Diagonal PTM filtering - started." << std::endl;
      DiagFilterMngPtr diag_filter_mng_ptr
          = std::make_shared<DiagFilterMng>(prsm_para_ptr, filter_result_num,
                                            thread_num, "DIAG_FILTER");
      DiagFilterProcessorPtr diag_filter_processor
          = std::make_shared<DiagFilterProcessor>(diag_filter_mng_ptr);
      diag_filter_processor->process();
      std::cout << "Diagonal filtering - finished." << std::endl;

      std::cout << "Two PTM search - started." << std::endl;
      PtmSearchMngPtr two_search_mng_ptr
          = std::make_shared<PtmSearchMng>(prsm_para_ptr, n_top, max_ptm_mass, ptm_num,
                                           thread_num, "DIAG_FILTER", "PTM");
      PtmSearchProcessorPtr processor = std::make_shared<PtmSearchProcessor>(two_search_mng_ptr);
      processor->process();
      std::cout << "Two PTM search - finished." << std::endl;
      input_exts.push_back("PTM");
    }

    std::cout << "Combining PRSMs - started." << std::endl;
    int prsm_top_num = (ptm_num + 1) * 4;
    PrsmStrCombinePtr combine_ptr
        = std::make_shared<PrsmStrCombine>(sp_file_name, input_exts, "RAW_RESULT", prsm_top_num);
    combine_ptr->process();
    combine_ptr = nullptr;
    std::cout << "Combining PRSMs - finished." << std::endl;

    std::cout << "E-value computation - started." << std::endl;
    bool variable_ptm = false;
    TdgfMngPtr tdgf_mng_ptr
        = std::make_shared<TdgfMng>(prsm_para_ptr, ptm_num, max_ptm_mass,
                                    use_gf, variable_ptm, thread_num, "RAW_RESULT", "EVALUE");
    EValueProcessorPtr processor = std::make_shared<EValueProcessor>(tdgf_mng_ptr);
    processor->init();
    // compute E-value for a set of prsms each run
    processor->process(false);
    processor = nullptr;
    std::cout << "E-value computation - finished." << std::endl;

    std::cout << "Finding protein species - started." << std::endl;
    if (arguments["featureFileName"] != "") {
      // TopFD msalign file with feature ID
      double prec_error_tole = 1.2;
      ModPtrVec fix_mod_list = prsm_para_ptr->getFixModPtrVec();
      PrsmFeatureSpeciesPtr prsm_forms
          = std::make_shared<PrsmFeatureSpecies>(db_file_name,
                                                 sp_file_name,
                                                 arguments["featureFileName"],
                                                 "EVALUE",
                                                 "FORMS",
                                                 fix_mod_list,
                                                 prec_error_tole,
                                                 prsm_para_ptr);
      prsm_forms->process();
      prsm_forms = nullptr;
    } else {
      double ppo;
      std::istringstream(arguments["errorTolerance"]) >> ppo;
      ppo = ppo / 1000000.0;
      PrsmSpeciesPtr prsm_species
          = std::make_shared<PrsmSpecies>(db_file_name, sp_file_name,
                                          "EVALUE", prsm_para_ptr->getFixModPtrVec(),
                                          "FORMS", ppo);
      prsm_species->process();
      prsm_species = nullptr;
    }
    std::cout << "Finding protein species - finished." << std::endl;

    if (arguments["searchType"] == "TARGET") {
      std::cout << "Top PRSM selecting - started" << std::endl;
      PrsmTopSelectorPtr selector
          = std::make_shared<PrsmTopSelector>(db_file_name, sp_file_name, "FORMS", "TOP", n_top);
      selector->process();
      selector = nullptr;
      std::cout << "Top PRSM selecting - finished." << std::endl;
    } else {
      std::cout << "Top PRSM selecting - started " << std::endl;
      PrsmTopSelectorPtr selector
          = std::make_shared<PrsmTopSelector>(db_file_name, sp_file_name,
                                              "FORMS", "TOP_PRE", n_top);
      selector->process();
      selector = nullptr;
      std::cout << "Top PRSM selecting - finished." << std::endl;

      std::cout << "FDR computation - started. " << std::endl;
      PrsmFdrPtr fdr = std::make_shared<PrsmFdr>(db_file_name, sp_file_name, "TOP_PRE", "TOP");
      fdr->process();
      fdr = nullptr;
      std::cout << "FDR computation - finished." << std::endl;
    }

    std::cout << "PRSM selecting by cutoff - started." << std::endl;
    std::string cutoff_type = arguments["cutoffSpectralType"];
    double cutoff_value;
    std::istringstream(arguments["cutoffSpectralValue"]) >> cutoff_value;
    PrsmCutoffSelectorPtr cutoff_selector
        = std::make_shared<PrsmCutoffSelector>(db_file_name, sp_file_name, "TOP",
                                               "CUTOFF_RESULT_SPEC", cutoff_type, cutoff_value);
    cutoff_selector->process();
    cutoff_selector = nullptr;
    std::cout << "PRSM selecting by cutoff - finished." << std::endl;

    std::string suffix = "CUTOFF_RESULT_SPEC";

    if (localization) {
      std::cout << "PTM characterization - started." << std::endl;
      LocalMngPtr local_mng
          = std::make_shared<LocalMng>(prsm_para_ptr,
                                       arguments["local_threshold"],
                                       arguments["residueModFileName"],
                                       max_ptm_mass,
                                       "CUTOFF_RESULT", "LOCAL_RESULT");
      LocalProcessorPtr local_ptr = std::make_shared<LocalProcessor>(local_mng);
      local_ptr->process();
      local_ptr = nullptr;
      std::cout << "PTM characterization - finished." << std::endl;
      suffix = "LOCAL_RESULT";
    }

    time_t end = time(0);
    arguments["end_time"] = std::string(ctime_r(&end, buf));
    arguments["running_time"] = std::to_string(static_cast<int>(difftime(end, start)));

    std::cout << "Outputting the PRSM result table - started." << std::endl;
    PrsmTableWriterPtr table_out
        = std::make_shared<PrsmTableWriter>(prsm_para_ptr, arguments, suffix, "OUTPUT_TABLE");
    table_out->write();
    table_out = nullptr;
    std::cout << "Outputting the PRSM result table - finished." << std::endl;

    std::cout << "Generating the PRSM xml files - started." << std::endl;
    XmlGeneratorPtr xml_gene = std::make_shared<XmlGenerator>(prsm_para_ptr, exe_dir, suffix, "prsm_cutoff");
    xml_gene->process();
    xml_gene = nullptr;
    std::cout << "Generating the PRSM xml files - finished." << std::endl;

    std::cout << "Converting the PRSM xml files to html files - started." << std::endl;
    translate(arguments, "prsm_cutoff");
    std::cout << "Converting the PRSM xml files to html files - finished." << std::endl;

    std::cout << "Proteoform selecting by cutoff - started." << std::endl;
    cutoff_type = (arguments["cutoffProteoformType"] == "FDR") ? "FORMFDR": "EVALUE";
    std::istringstream(arguments["cutoffProteoformValue"]) >> cutoff_value;
    cutoff_selector = std::make_shared<PrsmCutoffSelector>(db_file_name, sp_file_name, "TOP",
                                                           "CUTOFF_RESULT_FORM", cutoff_type,
                                                           cutoff_value);
    cutoff_selector->process();
    cutoff_selector = nullptr;
    std::cout << "Proteoform selecting by cutoff - finished." << std::endl;

    std::cout << "Proteoform filtering - started." << std::endl;
    PrsmFormFilterPtr form_filter
        = std::make_shared<PrsmFormFilter>(db_file_name, sp_file_name, "CUTOFF_RESULT_FORM",
                                           "FORM_FILTER_RESULT", "FORM_RESULT");
    form_filter->process();
    form_filter = nullptr;
    std::cout << "Proteoform filtering - finished." << std::endl;

    std::cout << "Outputting the proteoform result table - started." << std::endl;
    PrsmTableWriterPtr form_out
        = std::make_shared<PrsmTableWriter>(prsm_para_ptr, arguments,
                                            "FORM_RESULT", "FORM_OUTPUT_TABLE");
    form_out->write();
    form_out = nullptr;
    std::cout << "Outputting the proteoform result table - finished." << std::endl;

    std::cout << "Generating the proteoform xml files - started." << std::endl;
    xml_gene = std::make_shared<XmlGenerator>(prsm_para_ptr, exe_dir, "CUTOFF_RESULT_FORM",
                                              "proteoform_cutoff");
    xml_gene->process();
    xml_gene = nullptr;
    std::cout << "Generating the proteoform xml files - finished." << std::endl;

    std::cout << "Converting the proteoform xml files to html files - started." << std::endl;
    translate(arguments, "proteoform_cutoff");
    std::cout << "Converting the proteoform xml files to html files - finished." << std::endl;

    if (arguments["keepTempFiles"] != "true") {
      std::cout << "Deleting temporary files - started." << std::endl;
      FileUtil::delDir(FileUtil::basename(sp_file_name) + "_proteoform_cutoff_xml");
      FileUtil::delDir(FileUtil::basename(sp_file_name) + "_prsm_cutoff_xml");
      FileUtil::cleanDir(ori_db_file_name, sp_file_name);
      std::cout << "Deleting temporary files - finished." << std::endl;
    }
  } catch (const char* e) {
    std::cout << "[Exception]" << std::endl;
    std::cout << e << std::endl;
  }
  std::cout << "TopPIC finished." << std::endl;
  return EXIT_SUCCESS;
}

}  // namespace prot
