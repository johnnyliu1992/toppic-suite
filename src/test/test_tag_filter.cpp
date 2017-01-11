// Copyright (c) 2014 - 2016, The Trustees of Indiana University.
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without modification,
// are permitted provided that the following conditions are met:
//
// 1. Redistributions of source code must retain the above copyright notice, this
// list of conditions and the following disclaimer.
//
// 2. Redistributions in binary form must reproduce the above copyright notice,
// this list of conditions and the following disclaimer in the documentation and/or
// other materials provided with the distribution.
//
// Indiana University provides no reassurances that the source code provided does
// not infringe the patent or any other intellectual property rights of any other
// entity. Indiana University disclaims any liability to any recipient for claims
// brought by any other entity based on infringement of intellectual property
// rights or otherwise.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
// ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
// WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
// DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR
// ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
// (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
// LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
// ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
// (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
// SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.


#include <iostream>
#include <iomanip>

#include "base/fasta_reader.hpp"
#include "base/fasta_util.hpp"
#include "base/base_data.hpp"
#include "base/web_logger.hpp"

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

#include "tagfilter/tag_filter_mng.hpp"
#include "tagfilter/tag_filter_processor.hpp"


#include "local/local_mng.hpp"
#include "local/local_processor.hpp"

#include "prsmview/xml_generator.hpp"
#include "prsmview/transformer.hpp"

#include "console/argument.hpp"

namespace prot {

int two_base_opt(int argc, char* argv[]) {
  try {
    Argument argu_processor;
    bool success = argu_processor.parse(argc, argv);
    if (!success) {
      return 1;
    }
    std::map<std::string, std::string> arguments = argu_processor.getArguments();
    std::cout << "TopPIC 1.0.0 (" << __DATE__ << ")" << std::endl;

    std::string exe_dir = arguments["executiveDir"];
    time_t start = time(0);
    arguments["start_time"] = std::string(ctime(&start));
    Argument::outputArguments(std::cout, arguments);

    BaseData::init(exe_dir);

    LOG_DEBUG("Init base data completed");

    std::string db_file_name = arguments["databaseFileName"];
    std::string sp_file_name = arguments["spectrumFileName"];
    std::string ori_db_file_name = arguments["oriDatabaseFileName"];
    std::string log_file_name = arguments["logFileName"];

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
    // initialize log file 
    WebLog::init(log_file_name, use_gf, localization, ptm_num);
    LOG_DEBUG("web log inited");

    PrsmParaPtr prsm_para_ptr = PrsmParaPtr(new PrsmPara(arguments));
    LOG_DEBUG("prsm para inited");

    bool decoy = false;
    if (arguments["searchType"] == "TARGET+DECOY") {
      decoy = true;
    }
    LOG_DEBUG("block size " << arguments["databaseBlockSize"]);
    int db_block_size = std::stoi(arguments["databaseBlockSize"]);

    FastaUtil::dbPreprocess (ori_db_file_name, db_file_name, decoy, db_block_size);
    MsAlignUtil::geneSpIndex(sp_file_name);

    std::vector<std::string> input_exts;

    std::cout << "Tag filtering - started." << std::endl;

#ifdef MS_TAG
    TagFilterMngPtr tag_filter_mng_ptr =
        std::make_shared<TagFilterMng>(prsm_para_ptr, "TAG_FILTER",
                                       arguments["residueModFileName"]);
#endif
#ifdef MS_PATHFINDER
    TagFilterMngPtr tag_filter_mng_ptr =
        std::make_shared<TagFilterMng>(prsm_para_ptr, "MS_PATHFINDER_FILTER",
                                       arguments["residueModFileName"]);
#endif

    TagFilterProcessorPtr tag_filter_processor = std::make_shared<TagFilterProcessor>(tag_filter_mng_ptr);
    tag_filter_processor->process();

    std::cout << "Tag filtering - finished." << std::endl;

  } catch (const char* e) {
    std::cout << "[Exception]" << std::endl;
    std::cout << e << std::endl;
  }
  std::cout << "TopPC finished." << std::endl;
  return 0;
}

}

int main(int argc, char* argv[]) {
  // prot::log_level = 2;
  std::cout << std::setprecision(10);
  return prot::two_base_opt(argc, argv);
}
