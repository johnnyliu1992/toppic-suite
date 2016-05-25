#include <iomanip>
#include <boost/algorithm/string.hpp>
#include "base/file_util.hpp"
#include "base/xml_dom_util.hpp"
#include "console/argument.hpp"

namespace prot {

Argument::Argument() {
  initArguments();
}

void Argument::initArguments() {
  arguments_["oriDatabaseFileName"]="";
  arguments_["databaseFileName"] = "";
  arguments_["databaseBlockSize"] = "1000000";
  arguments_["spectrumFileName"] = "";
  arguments_["activation"] = "FILE";
  arguments_["searchType"] = "TARGET";
  arguments_["fixedMod"] = "";
  arguments_["ptmNumber"] = "1";
  arguments_["errorTolerance"] = "15";
  arguments_["cutoffType"] = "EVALUE";
  arguments_["cutoffValue"] = "0.01";
  arguments_["allowProtMod"] = "NONE,NME,NME_ACETYLATION";
  arguments_["numOfTopPrsms"] = "1";
  arguments_["maxPtmMass"] = "500";
  arguments_["useGf"] = "false";
  arguments_["executiveDir"] = ".";
  arguments_["logFileName"] = "";
  arguments_["keepTempFiles"] = "false";
  arguments_["fullBinaryPath"] = "false";
  arguments_["local_threshold"] = "0.45";
  arguments_["groupSpectrumNumber"] = "1";
  arguments_["filteringResultNumber"] = "20";
  arguments_["residueModFileName"] = "";
  arguments_["proteo_graph_dis"] = "20";
  arguments_["threadNumber"] = "1";
}

void Argument::outputArguments(std::ostream &output, 
                               std::map<std::string, std::string> arguments) {
  output << "********************** Parameters **********************" << std::endl;
  output << std::setw(44) << std::left << "Protein database file: " << "\t" << arguments["oriDatabaseFileName"] << std::endl;
  output << std::setw(44) << std::left << "Spectrum file: " << "\t" << arguments["spectrumFileName"] << std::endl;
  output << std::setw(44) << std::left << "Number of combined spectra: " << "\t" << arguments["groupSpectrumNumber"] << std::endl;
  output << std::setw(44) << std::left << "Fragmentation method: " << "\t" << arguments["activation"] << std::endl;
  output << std::setw(44) << std::left << "Search type: " << "\t" << arguments["searchType"] << std::endl;
  if (arguments["fixedMod"] == "") {
    output << std::setw(44) << std::left << "Fixed modifications: " << "\t" << "None" << std::endl;
  }
  else {
    output << std::setw(44) << std::left << "Fixed modifications: " << "\t" << arguments["fixedMod"] << std::endl;
  }
  output << std::setw(44) << std::left << "Maximum number of unexpected modifications: " << "\t" << arguments["ptmNumber"] << std::endl;
  output << std::setw(44) << std::left << "Error tolerance: " << "\t" << arguments["errorTolerance"] << " ppm" << std::endl;
  output << std::setw(44) << std::left << "Cutoff type: " << "\t" << arguments["cutoffType"] << std::endl;
  output << std::setw(44) << std::left << "Cutoff value: " << "\t" << arguments["cutoffValue"] << std::endl;
  output << std::setw(44) << std::left << "Allowed N-terminal forms: " << "\t" << arguments["allowProtMod"] << std::endl;
  output << std::setw(44) << std::left << "Maximum mass shift of modifications: " << "\t" << arguments["maxPtmMass"] << " Da" << std::endl;
#if defined MASS_GRAPH
  output << std::setw(44) << std::left << "Thread number: " << "\t" << arguments["threadNumber"] << std::endl;
#endif

  if (arguments["useGf"] == "true") {
    output << std::setw(44) << std::left << "E-value computation: " << "\t" << "Generating function" << std::endl;
  } else {
    output << std::setw(44) << std::left << "E-value computation: " << "\t" << "Lookup table" << std::endl;
  }

  if (arguments["residueModFileName"] != "") {
    output << std::setw(44) << std::left << "Common modification file name: " << "\t" << arguments["residueModFileName"] << std::endl;
    output << std::setw(44) << std::left << "MIScore threshold: " << "\t" << arguments["local_threshold"] << std::endl;
  }
#if defined MASS_GRAPH
  output << std::setw(44) << std::left << "Gap in proteoform graph: " << "\t" << arguments["proteo_graph_dis"] << std::endl;
#endif
  output << std::setw(44) << std::left << "Executable file directory: " << "\t" << arguments["executiveDir"] << std::endl;
  output << std::setw(44) << std::left << "Start time: " << "\t" << arguments["start_time"];
  if (arguments["end_time"] != "") {
    output << std::setw(44) << std::left << "End time: " << "\t" << arguments["end_time"];
    output << std::setw(44) << std::left << "Running time: " << "\t" << arguments["running_time"] << " seconds" << std::endl;
  }
  output << "********************** Parameters **********************" << std::endl;
}

void Argument::showUsage(boost::program_options::options_description &desc) {
  std::cout << "Usage: toppic [options] database-file-name spectrum-file-name" << std::endl; 
  std::cout << desc << std::endl; 
}

void Argument::setArgumentsByConfigFile(const std::string &filename){
  initArguments();
  XmlDOMParser* parser = XmlDOMParserFactory::getXmlDOMParserInstance();
  if(parser){
    XmlDOMDocument* doc = new XmlDOMDocument(parser, filename.c_str());
    if (doc) {
      xercesc::DOMElement* root = doc->getDocumentElement();
      arguments_["executiveDir"] = XmlDomUtil::getChildValue(root,"executiveDir",0);
      arguments_["oriDatabaseFileName"] = XmlDomUtil::getChildValue(root,"database_file_name",0);
      arguments_["spectrumFileName"] = XmlDomUtil::getChildValue(root,"spectrum_file_name",0);
      arguments_["logFileName"] = XmlDomUtil::getChildValue(root,"log_file_name",0);
      arguments_["activation"] = XmlDomUtil::getChildValue(root,"fragmentation_method",0);
      arguments_["fixedMod"] = XmlDomUtil::getChildValue(root,"fixed_mod",0);
      arguments_["searchType"] = XmlDomUtil::getChildValue(root,"search_type",0);
      arguments_["ptmNumber"] = XmlDomUtil::getChildValue(root,"shift_number",0);
      arguments_["errorTolerance"] = XmlDomUtil::getChildValue(root,"error_tolerance",0);
      arguments_["cutoffType"] = XmlDomUtil::getChildValue(root,"cutoff_type",0);
      arguments_["cutoffValue"] = XmlDomUtil::getChildValue(root,"cutoff_value",0);
      arguments_["maxPtmMass"] = XmlDomUtil::getChildValue(root,"max_ptm_mass",0);
      arguments_["useGf"] = XmlDomUtil::getChildValue(root,"use_gf",0);
      arguments_["groupSpectrumNumber"] = XmlDomUtil::getChildValue(root, "groupSpectrumNumber", 0);
      arguments_["residueModFileName"] = XmlDomUtil::getChildValue(root, "residueModFileName", 0);
      arguments_["local_threshold"] = XmlDomUtil::getChildValue(root, "local_threshold", 0);

      xercesc::DOMElement* prot_mod_list = XmlDomUtil::getChildElement(root,"protein_variable_ptm_list",0);
      int allow_prot_node_number = XmlDomUtil::getChildCount(prot_mod_list,"protein_variable_ptm");
      std::string allow_mod="";
      for(int i = 0; i < allow_prot_node_number; i++){
        if(i == 0){
          allow_mod = XmlDomUtil::getChildValue(prot_mod_list, "protein_variable_ptm", i);
        } else{
          allow_mod = allow_mod+","+XmlDomUtil::getChildValue(prot_mod_list, "protein_variable_ptm", i);
        }
      }
      arguments_["allowProtMod"] = allow_mod;
    }
    delete doc;
  }
}

bool Argument::parse(int argc, char* argv[]) {
  std::string database_file_name = "";
  std::string spectrum_file_name = "";
  std::string argument_file_name = "";
  std::string activation = "";
  std::string fixed_mod = "";
  std::string allow_mod = "";
  std::string ptm_num = "";
  std::string error_tole = "";
  std::string max_ptm_mass = "";
  std::string cutoff_type = "";
  std::string cutoff_value = "";
  std::string log_file_name = "";
  std::string use_table = "";
  std::string group_num = "";
  std::string local_threshold = "";
  std::string filtering_result_num = "";
  std::string residue_mod_file_name = "";
  std::string proteo_graph_dis = "";
  std::string thread_number = "";

  /** Define and parse the program options*/
  try {
    namespace po = boost::program_options;
    po::options_description display_desc("Options");

    display_desc.add_options() 
        ("help,h", "Print the help message.") 
        ("activation,a", po::value<std::string>(&activation),
         "<CID|HCD|ETD|UVPD|FILE>. Fragmentation method of tandem mass spectra. When FILE is used, fragmentation methods of spectra are given in the input spectral data file. Default value: FILE.")
        ("fixed-mod,f", po::value<std::string> (&fixed_mod), 
         "<C57|C58|a fixed modification file>. Fixed modifications. Three available options: C57, C58, or the name of a text file containing the information of fixed modifications. When C57 is selected, carbamidometylation on cysteine is the only fixed modification. When C58 is selected, carboxymethylation on cysteine is the only fixed modification.")
        ("n-termimal-form,n", po::value<std::string> (&allow_mod), 
         "<a list of allowed N-terminal forms>. N-terminal forms of proteins. Four N-terminal forms can be selected: NONE, NME, NME_ACETYLATION, and M_ACETYLATION. NONE stands for no modifications; NME for N-terminal methionine excision, NME_ACETYLATION for N-terminal acetylation after the initiator methionine is removed, and M_ACETYLATION for N-terminal methionine acetylation. When multiple forms are allowed, they are separated by commas. Default value: NONE,NME,NME_ACETYLATION.")
        ("decoy,d", "Use a decoy protein database to estimate false discovery rates.")
        ("error-tolerance,e", po::value<std::string> (&error_tole), "<a positive integer>. Error tolerance for precursor and fragment masses in PPM. Default value: 15.")
        ("max-mod,m", po::value<std::string> (&max_ptm_mass), "<a positive number>. Maximum absolute value of mass shifts (in Dalton) of unexpected modifications. Default value: 500.")
        ("num-mod,p", po::value<std::string> (&ptm_num), "<0|1|2>. Maximum number of unexpected modifications in a proteoform spectrum-match. Default value: 1.")
        ("cutoff-type,t", po::value<std::string> (&cutoff_type), "<EVALUE|FDR>. Cutoff type for filtering identified proteoform spectrum-matches. Default value: EVALUE.")
        ("cutoff-value,v", po::value<std::string> (&cutoff_value), "<a positive number>. Cutoff value for filtering identified proteoform spectrum-matches. Default value: 0.01.")
        ("generating-function,g", "Use the generating function approach to compute p-values and E-values.")
        ("num-combined-spectra,r", po::value<std::string> (&group_num), 
         "<a positive integer>. Number of combined spectra. The parameter is set as 2 (or 3) for combining spectral pairs (or triplets) generated by the alternating fragmentation mode. Default value: 1")
        ("mod-file-name,i", po::value<std::string>(&residue_mod_file_name), "<a common modification file>. Specify a text file containing the information of common PTMs for characterization of PTMs in proteoform spectrum-matches.")
        ("miscore-threshold,s", po::value<std::string> (&local_threshold), "<a positive number between 0 and 1>. Threshold modification identification score for filtering results of PTM characterization. Default value: 0.45.")
#if defined MASS_GRAPH
        ("proteo-graph-dis,j", po::value<std::string> (&proteo_graph_dis), "<positive number>. Gap in constructing proteoform graph. Default value: 20.")
        ("thread-number,u", po::value<std::string> (&thread_number), "<positive number>. Number of threads used in the computation. Default value: 1.")
#endif
        ;
    po::options_description desc("Options");

    desc.add_options() 
        ("help,h", "") 
        ("argument-file,c",po::value<std::string>(&argument_file_name),"Argument file name.")
        ("activation,a", po::value<std::string>(&activation), "")
        ("fixed-mod,f", po::value<std::string> (&fixed_mod), "")
        ("n-termimal-form,n", po::value<std::string> (&allow_mod), "")
        ("decoy,d", "")
        ("error-tolerance,e", po::value<std::string> (&error_tole), "")
        ("max-mod,m", po::value<std::string> (&max_ptm_mass), "")
        ("num-mod,p", po::value<std::string> (&ptm_num), "")
        ("cutoff-type,t", po::value<std::string> (&cutoff_type), "")
        ("cutoff-value,v", po::value<std::string> (&cutoff_value), "")
        ("filtering-result-number,o", po::value<std::string>(&filtering_result_num), "Filtering result number. Default value: 20.")
        ("log-file-name,l", po::value<std::string>(&log_file_name), "Log file name with its path.")
        ("keep-temp-files,k", "Keep temporary files.")
        ("generating-function,g", "")
        ("local-threshold,s", po::value<std::string> (&local_threshold), "")
        ("full-binary-path,b", "Full binary path.")
        ("num-combined-spectra,r", po::value<std::string> (&group_num), "")
        ("mod-file-name,i", po::value<std::string>(&residue_mod_file_name), "")
        ("proteo-graph-dis,j", po::value<std::string> (&proteo_graph_dis), "<positive number>. Gap in constructing proteoform graph. Default value: 20.")
        ("thread-number,u", po::value<std::string> (&thread_number), "<positive number>. Number of threads used in the computation. Default value: 1.")
        ("database-file-name", po::value<std::string>(&database_file_name)->required(), "Database file name with its path.")
        ("spectrum-file-name", po::value<std::string>(&spectrum_file_name)->required(), "Spectrum file name with its path.");

    po::positional_options_description positional_options;
    positional_options.add("database-file-name", 1);
    positional_options.add("spectrum-file-name", 1);

    std::string app_name;
    //= boost::filesystem::basename(argv[0]);
    po::variables_map vm;
    try {
      po::store(po::command_line_parser(argc, argv).options(desc).positional(positional_options).run(),vm); 
      if ( vm.count("help") ) {
        showUsage(display_desc);
        return false;
      }
      po::notify(vm); 
      // throws on error, so do after help in case there are any problems
    } 
    catch(boost::program_options::required_option& e) {
      std::cerr << "ERROR: " << e.what() << std::endl << std::endl;
      showUsage(display_desc);
      return false;
    }
    catch(boost::program_options::error& e) {
      std::cerr << "ERROR: " << e.what() << std::endl << std::endl;
      showUsage(display_desc);
      return false;
    }
    std::string argv_0 (argv[0]);
    if (vm.count("full-binary-path")) {
      arguments_["executiveDir"] = argv[0];
    } else {
      arguments_["executiveDir"] = FileUtil::getExecutiveDir(argv_0);
    }
    LOG_DEBUG("Executive Dir " << arguments_["executiveDir"]);
    if (vm.count("argument-file")) {
      setArgumentsByConfigFile(argument_file_name);
    }
    arguments_["oriDatabaseFileName"] = database_file_name;
    arguments_["spectrumFileName"] = spectrum_file_name;
    if (vm.count("activation")) {
      arguments_["activation"] = activation;
    }
    if (vm.count("decoy")) {
      arguments_["searchType"] = "TARGET+DECOY";
    }
    if (arguments_["searchType"] == "TARGET+DECOY") {
      arguments_["databaseFileName"]=arguments_["oriDatabaseFileName"] + "_target_decoy";
    } else {
      arguments_["databaseFileName"]=arguments_["oriDatabaseFileName"] + "_target";
    }
    if (vm.count("fixed-mod")) {
      arguments_["fixedMod"] = fixed_mod;
    }
    if (vm.count("n-termimal-form")) {
      arguments_["allowProtMod"] = allow_mod;
    }    
    if (vm.count("num-mod")) {
      arguments_["ptmNumber"] = ptm_num;
    }
    if (vm.count("error-tolerance")) {
      arguments_["errorTolerance"] = error_tole;
    }
    if (vm.count("max-mod")) {
      arguments_["maxPtmMass"] = max_ptm_mass;
    }
    if (vm.count("cutoff-type")) {
      arguments_["cutoffType"] = cutoff_type;
    }
    if (vm.count("cutoff-value")) {
      arguments_["cutoffValue"] = cutoff_value;
    }
    if (vm.count("log-file-name")) {
      arguments_["logFileName"] = log_file_name;
    }
    if (vm.count("keep-temp-files")) {
      arguments_["keepTempFiles"] = "true";
    }
    if (vm.count("full-binary-path")) {
      arguments_["fullBinaryPath"] = "true";
    }
    if (vm.count("generating-function")) {
      arguments_["useGf"] = "true";
    }
    if (vm.count("miscore-threshold")) {
      arguments_["local_threshold"] = local_threshold;
    }
    if (vm.count("num-combined-spectra")) {
      arguments_["groupSpectrumNumber"] = group_num;
    }
    if (vm.count("filtering-result-number")) {
      arguments_["filteringResultNumber"] = filtering_result_num;
    }
    if (vm.count("mod-file-name")) {
      arguments_["residueModFileName"] = residue_mod_file_name;
    }
    if (vm.count("proteo-graph-dis")) {
      arguments_["proteo_graph_dis"] = proteo_graph_dis;
    }
    if (vm.count("thread-number")) {
      arguments_["threadNumber"] = thread_number;
    }
  }
  catch(std::exception&e ) {
    std::cerr << "Unhandled Exception in parsing command line"<<e.what()<<", application will now exit"<<std::endl;
    return false;
  }

  return validateArguments();
}

bool Argument::validateArguments() {
  if (!boost::filesystem::exists(arguments_["oriDatabaseFileName"])) {
    LOG_ERROR("Database file " << arguments_["databaseFileName"] << " does not exist!");
    return false;
  }

  if (!boost::filesystem::exists(arguments_["spectrumFileName"])) {
    LOG_ERROR("Spectrum file " << arguments_["spectrumFileName"] << " does not exist!");
    return false;
  }
  std::string activation = arguments_["activation"];
  if(activation != "CID" && activation != "HCD" 
     && activation != "ETD" && activation != "FILE" && activation != "UVPD") {
    LOG_ERROR("Activation type " << activation << " error! The value should be CID|HCD|ETD|UVPD|FILE!");
    return false;
  }

  std::string search_type = arguments_["searchType"];
  if(search_type != "TARGET" && search_type != "TARGET+DECOY"){
    LOG_ERROR("Search type " << search_type << " error! The value should be TARGET|TARGET+DECOY!");
    return false;
  }
  std::string ptm_number = arguments_["ptmNumber"];
  if (ptm_number != "0" && ptm_number != "1" && ptm_number != "2") {
    LOG_ERROR("PTM number "<< ptm_number <<" error! The value should be 0|1|2!");
    return false;
  }

  std::string allow_mod = arguments_["allowProtMod"]; 
  std::vector<std::string> strs;
  boost::split(strs, allow_mod, boost::is_any_of(","));
  for (size_t i = 0; i < strs.size(); i++) {
    if (strs[i] != "NONE" && strs[i] != "M_ACETYLATION" && strs[i] != "NME" && strs[i] != "NME_ACETYLATION") {
      LOG_ERROR("N-Terminal Variable PTM can only be NONE, M_ACETYLATION, NME or NME_ACETYLATION.");
      return false;
    }
  }

  std::string cutoff_type = arguments_["cutoffType"];
  if (cutoff_type != "EVALUE" && cutoff_type != "FDR") {
    LOG_ERROR("Cutoff type " << cutoff_type << " error! The value should be EVALUE|FDR");
    return false;
  }

  if (cutoff_type == "FDR" && search_type != "TARGET+DECOY"){
    LOG_ERROR("Cutoff type "<< cutoff_type << " error! FDR cutoff cannot be used when no decoy database is used! Please add argument '-d' in the command.");
    return false;
  }

  std::string use_gf = arguments_["useGf"];
  if(use_gf != "true" && use_gf != "false"){
    LOG_ERROR("Use gf " << use_gf << " error! The value should be true|false!");
    return false;
  }

  if(use_gf == "false" && arguments_["errorTolerance"] !="5" && arguments_["errorTolerance"]!="10" && arguments_["errorTolerance"]!="15"){
    LOG_ERROR("Error tolerance can only be 5, 10 or 15 when the generation function approach for E-value computation is not selected!");
    return false;
  }

  std::string max_ptm_mass = arguments_["maxPtmMass"];
  try {
    double mass = std::stod(max_ptm_mass.c_str());
    if(mass <= 0.0){
      LOG_ERROR("Maximum PTM mass " << max_ptm_mass << " error! The value should be positive.");
      return false;
    }
  }
  catch (int e) {
    LOG_ERROR("Maximum ptm mass " << max_ptm_mass << " should be a number.");
    return false;
  }
  return true;

  std::string cutoff_value = arguments_["cutoffValue"];
  try {
    double th = std::stod(cutoff_value.c_str());
    if(th < 0){
      LOG_ERROR("Cutoff value " << cutoff_value << " error! The value should be positive.");
      return false;
    }
  }
  catch (int e) {
    LOG_ERROR("Cutoff value " << cutoff_value << " should be a number.");
    return false;
  }

  std::string thread_number = arguments_["threadNumber"];
  try {
    int num = std::stoi(thread_number.c_str());
    if(num <= 0){
      LOG_ERROR("Thread number " << thread_number << " error! The value should be positive.");
      return false;
    }
    if(num > 64){
      LOG_ERROR("Thread number " << thread_number << " error! The value is too large.");
      return false;
    }
  }
  catch (int e) {
    LOG_ERROR("Cutoff value " << cutoff_value << " should be a number.");
    return false;
  }

  return true;
}

} /* namespace prot */
