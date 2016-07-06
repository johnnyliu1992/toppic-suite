#include "base/logger.hpp"
#include "spec/peak_list.hpp"
#include "feature/env_detect.hpp" 

namespace prot {

MatchEnvPtr2D EnvDetect::getCandidate(DeconvDataPtr data_ptr, FeatureMngPtr mng_ptr) {
  PeakPtrVec peak_list = data_ptr->getPeakList();
  int peak_num = peak_list.size();
  int max_charge = data_ptr->getMaxCharge();
  LOG_DEBUG(peak_num << " " << max_charge);
  MatchEnvPtr2D match_envs;
  for (int idx = 0; idx < peak_num; idx++) {
    MatchEnvPtrVec envs;
    for (int charge = 1; charge <= max_charge; charge++) {
      MatchEnvPtr env =  detectEnv(peak_list, idx, charge, data_ptr->getMaxMass(), mng_ptr);
      envs.push_back(env);
    }
    match_envs.push_back(envs);
  }
  return match_envs;
}

// detect a MatchEnv 
MatchEnvPtr EnvDetect::detectEnv(PeakPtrVec &peak_list, int base_peak,
                                 int charge, double max_mass, FeatureMngPtr mng_ptr) {

  double base_mz = peak_list[base_peak]->getPosition();
  // check if the mass is greater than the precursor mass 
  double base_mass = base_mz * charge - charge * MassConstant::getProtonMass();
  if (base_mass >= max_mass || base_mass < mng_ptr->min_mass_) {
    return nullptr;
  }

  // get a reference distribution based on the base mass 
  EnvelopePtr ref_env = mng_ptr->env_base_ptr_->getEnvByBaseMass(base_mass);
  if (ref_env == nullptr) {
    LOG_WARN("reference envelope is null");
    return nullptr;
  }

  // convert the reference distribution to a theoretical distribution
  // based on the base mz and charge state
  EnvelopePtr theo_env = ref_env->distrToTheoBase(base_mz, charge);
  // scale theoretical distribution 
  double ratio = calcInteRatio(theo_env, peak_list, mng_ptr->mz_tolerance_);
  theo_env->changeIntensity(ratio);

  int mass_group = mng_ptr->getMassGroup(base_mass);

  // get the highest 85%--95% peaks 
  double percentage = mng_ptr->getPercentBound(mass_group);
  theo_env = theo_env->getSubEnv(percentage, mng_ptr->min_inte_,
                                 mng_ptr->max_back_peak_num_, mng_ptr->max_forw_peak_num_);

  // get real envelope 
  if (peak_list[base_peak]->getIntensity() < mng_ptr->min_refer_inte_) {
    return nullptr;
  }
  RealEnvPtr real_env(new RealEnv(peak_list, theo_env, mng_ptr->mz_tolerance_,
                                  mng_ptr->min_inte_));
  MatchEnvPtr match_env(new MatchEnv(mass_group, theo_env, real_env));
  return match_env;
}

// compute the intensity ratio based on the top three peaks 
double EnvDetect::calcInteRatio(EnvelopePtr theo_env, PeakPtrVec &peak_list, 
                                double tolerance) {
  std::vector<double> theo_intes = theo_env->getIntensities();
  double theo_sum = 0;
  double obs_sum = 0;
  int refer_idx = theo_env->getReferIdx();
  double mz = theo_env->getMz(refer_idx);
  int peak_idx = getNearPeakIdx(peak_list, mz, tolerance);
  if (peak_idx >= 0) {
    theo_sum += theo_intes[refer_idx];
    obs_sum += peak_list[peak_idx]->getIntensity();
  }
  if (refer_idx - 1 >= 0) {
    theo_sum += theo_intes[refer_idx - 1];
    mz = theo_env->getMz(refer_idx-1);
    peak_idx = getNearPeakIdx(peak_list, mz, tolerance);
    if (peak_idx >= 0) {
      obs_sum += peak_list[peak_idx]->getIntensity();
    }
  }
  if (refer_idx + 1 < theo_env->getPeakNum()) {
    theo_sum += theo_intes[refer_idx + 1];
    mz = theo_env->getMz(refer_idx + 1);
    peak_idx = getNearPeakIdx(peak_list, mz, tolerance);
    if (peak_idx >= 0) {
      obs_sum += peak_list[peak_idx]->getIntensity();
    }
  }
  if (theo_sum == 0) {
    return 1.0;
  } else {
    return obs_sum / theo_sum;
  }
}

}