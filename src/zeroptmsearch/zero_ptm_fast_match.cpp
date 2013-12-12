#include "zeroptmsearch/zero_ptm_fast_match.hpp"

#include <log4cxx/logger.h>


namespace prot {

static log4cxx::LoggerPtr logger(log4cxx::Logger::getLogger("ComputeMatch"));

ZeroPtmFastMatch computeCompMatch(
    ExtendMsPtr ms_ptr, ProteoformPtr form_ptr) {
  //to do: double maxError = ms_ptr->getHeaderPtr()->getHeader().getErrorTolerance();
  MsHeaderPtr header_ptr = ms_ptr->getHeaderPtr();
  double max_error = 0.01;
  double prec_mass = header_ptr->getPrecMonoMassMinusWater();
  double prot_mass = form_ptr->getResSeqPtr()->getResMassSum();
  double error = abs(prec_mass - prot_mass);
  LOG4CXX_TRACE(logger, "Protein mass " << prot_mass 
                << " precursor mass " << prec_mass 
                << " proteoform name " << form_ptr->getName()
                << " error " << error << " error tolerance " << max_error);
  double score = 0;
  if (error <= max_error) {
    ActivationPtr activation = header_ptr->getActivationPtr();
    IonTypePtr n_ion_type = activation->getNIonType();
    std::vector<double> masses = form_ptr->getBpSpecPtr()->getBreakPointMasses(n_ion_type);
    score = compDiagScr(ms_ptr, masses, 0);

    IonTypePtr c_ion_type = activation->getCIonType();
    masses = form_ptr->getBpSpecPtr()->getBreakPointMasses(c_ion_type);
    score += compDiagScr(ms_ptr, masses, 0);
  }
  return ZeroPtmFastMatch(form_ptr, score);
}


double compDiagScr(ExtendMsPtr ms_ptr,
                  std::vector<double> &masses, double center) {
  unsigned int i = 0;
  unsigned int j = 0;
  double s = 0;
  while (i < ms_ptr->size() && j < masses.size()) {
    ExtendPeakPtr peak_ptr = ms_ptr->getPeakPtr(i);
    double distance = peak_ptr->getMonoMass() - masses[j];
    if (abs(center - distance) <= peak_ptr->getOrigTolerance()) {
      s += peak_ptr->getScore();
      i++;
      j++;
    }
    /*
     * we use 1 here since the difference between consecutive mass_b is
     * at least 50. and some time masses[i+1] may have small error tolerance
     * than masses[i]
     */
    if (distance > center + 1) {
      j++;
    } else {
      i++;
    }
  }
  return s;
}

}
