// Copyright (c) 2014 - 2017, The Trustees of Indiana University.
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

#include <algorithm>

#include "env_rescore.hpp"

namespace prot {
namespace EnvRescore {

std::vector<double> diff(MatchEnvPtr env, MatchEnvPtr2D &match_envs) {
  std::vector<double> result(2);
  double temp;
  int res = 0, res2 = 0;
  std::vector<double> sum;
  for (size_t i = 0; i < match_envs.size(); i++) {
    for (size_t j = 0; j < match_envs[i].size(); j++) {
      if (match_envs[i][j] != nullptr) {
        sum.push_back(match_envs[i][j]->getRealEnvPtr()->compIntensitySum()); 
        temp = std::abs(env->getRealEnvPtr()->getMonoMass()
                        - match_envs[i][j]->getRealEnvPtr()->getMonoMass());
        if (std::abs(temp - 18) < 0.01) {
          res++;
        } else if (std::abs(temp - 17) < 0.01) {
          res++;
        }
        if (std::abs(env->getRealEnvPtr()->getMonoMass()
                     - match_envs[i][j]->getRealEnvPtr()->getMonoMass()) < 0.01) {
          res2++;
        }
      }
    }
  }
  if (res >= 3) {
    result[0] = 3;
  } else {
    result[0] = res;
  }
  if (res2 >= 3) {
    result[1] = 3;
  } else {
    result[1] = res2;
  }
  return result;
}

double diffMZ(MatchEnvPtr env) {
  double result = 0;
  int n = env->getTheoEnvPtr()->getPeakNum();
  for (int i = 0; i < n; i++) {
    if (env->getRealEnvPtr()->getMz(i) == -1) {
      result += 0;
    } else if (std::abs(env->getRealEnvPtr()->getMz(i)
                        - env->getTheoEnvPtr()->getMz(i)) <= 0.02) {
      result += std::pow((env->getRealEnvPtr()->getMz(i)
                          - env->getTheoEnvPtr()->getMz(i)), 2);
    } else {
      result += std::pow(0.02, 2);
    }
  }

  result = result / (env->getRealEnvPtr()->getPeakNum() * std::pow(0.02, 2));
  result = std::sqrt(result);
  return result;
}

double intenDis(MatchEnvPtr env) {
  double result = 0;
  int n = env->getTheoEnvPtr()->getPeakNum();
  std::vector<double> theo = env->getTheoEnvPtr()->getIntensities();
  std::vector<double> real = env->getRealEnvPtr()->getIntensities();
  std::vector<double> intensities;
  for (double d : theo) intensities.push_back(d);
  double max = *std::max_element(intensities.begin(), intensities.end()) / 2;
  for (int i = 0; i < n; i++) {
    if (real[i] > theo[i]) {
      if (real[i] - theo[i] > max) {
        result += std::pow(max, 2);
      } else {
        result += std::pow(real[i] - theo[i], 2);
      }
    } else {
      if (theo[i] - real[i] > max) {
        result += 2 * std::pow(max, 2);
      } else {
        result += 2 * std::pow(theo[i] - real[i], 2);
      }
    }
  }
  result = result / (n * std::pow(max, 2));

  result = std::sqrt(result);

  return result;
}

int getMissingPeak(MatchEnvPtr env) {
  return env->getTheoEnvPtr()->getPeakNum() - env->getRealEnvPtr()->getPeakNum();
}

double rankScore(MatchEnvPtr env, MatchEnvPtr2D &match_envs,
                  const std::vector<double> & para) {
  double raw = 0;
  double result = 0;
  std::vector<double> diffRes = diff(env, match_envs);
  double mz = diffMZ(env);
  double inten = intenDis(env);
  int missing = getMissingPeak(env);
  raw = mz * para[0] + inten * para[1] + diffRes[1] * para[2]
      + diffRes[0] * para[3] + missing * para[4] + para[5];
  result = para[6] + para[7] * raw + para[8] * std::pow(raw, 2);
  return result;
}

void rescore(MatchEnvPtr2D &match_envs, const std::vector<std::vector<double> > para) {
  for (size_t i = 0; i < match_envs.size(); i++) {
    for (size_t j = 0; j < match_envs[i].size(); j++) {
      if (match_envs[i][j] != nullptr) {
        if (match_envs[i][j]->getTheoEnvPtr()->getPeakNum() == 2) {
          match_envs[i][j]->setScore(rankScore(match_envs[i][j],
                                               match_envs, para[0])); 
        } else if (match_envs[i][j]->getTheoEnvPtr()->getPeakNum() == 3) {
          match_envs[i][j]->setScore(rankScore(match_envs[i][j],
                                               match_envs, para[1])); 
        } else if (match_envs[i][j]->getTheoEnvPtr()->getPeakNum() == 4) {
          match_envs[i][j]->setScore(rankScore(match_envs[i][j],
                                               match_envs, para[2])); 
        } else {
          match_envs[i][j]->setScore(rankScore(match_envs[i][j],
                                               match_envs, para[3])); 
        }
      } 
    }
  }
}
}  // namespace EnvRescore
}  // namespace prot
