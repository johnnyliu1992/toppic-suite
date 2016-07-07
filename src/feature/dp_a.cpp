#include <limits>
#include <cstddef>
#include <algorithm>

#include "base/logger.hpp"
#include "feature/vertex_base.hpp"
#include "feature/dp_a.hpp"

namespace prot {

DpA::DpA(DeconvDataPtr data_ptr, MatchEnvPtr2D &win_envs_, FeatureMngPtr mng_ptr):
    Dp(data_ptr, win_envs_, mng_ptr) {
      initGraph();
      dp();
      backtrace();
    }

void DpA::initGraph() {
  // use win_num__ + 2 columns 
  vertices_.resize(win_num_ + 2);
  MatchEnvPtrVec envs;
  vertices_[0] = VertexBase::getVertexAList(data_ptr_, -1, envs, envs, mng_ptr_);
  vertices_[1] = VertexBase::getVertexAList(data_ptr_, 0, envs, win_envs_[0], mng_ptr_);
  for (int i = 1; i < win_num_; i++) {
    vertices_[i + 1] = VertexBase::getVertexAList(data_ptr_, i, win_envs_[i - 1], 
                                                 win_envs_[i], mng_ptr_);
  }
  vertices_[win_num_ + 1] = VertexBase::getVertexAList(data_ptr_, win_num_, 
                                                     win_envs_[win_num_ - 1], envs, mng_ptr_);
  int cnt = 0;
  for (int i = 0; i <= win_num_ + 1; i++) {
    cnt += vertices_[i].size();
  }
  LOG_DEBUG("Vertex count " << cnt);
}

void DpA::dp() {
  int cnt = 0;
  for (int i = 1; i < win_num_ + 2; i++) {
    for (size_t j = 0; j < vertices_[i].size(); j++) {
      VertexAPtr cur_ver = vertices_[i][j];
      for (size_t k = 0; k < vertices_[i - 1].size(); k++) {
        VertexAPtr prev_ver = vertices_[i - 1][k];
        if (Vertex::checkConsist(prev_ver, cur_ver, mng_ptr_->max_env_num_per_peak_)) {
          cnt++;
          double new_score = Vertex::getShareScr(prev_ver, cur_ver, mng_ptr_->score_error_tolerance_);
          double cur_score = prev_ver->getScrA() + new_score;
          if (cur_score > cur_ver->getScrA()) {
            cur_ver->setScrA(cur_score);
            cur_ver->setPreA(k);
          }
        }
      }
    }
  }
  LOG_DEBUG("Edge count :" << cnt);
}

// backtracking 
void DpA::backtrace() {
  int best_ver = -1;
  double best_score = - std::numeric_limits<double>::max();
  for (size_t i = 0; i < vertices_[win_num_ + 1].size(); i++) {
    double cur_score = vertices_[win_num_ + 1][i]->getScrA();
    if (cur_score > best_score) {
      best_ver = i;
      best_score = cur_score;
    }
  }
  for (int i = win_num_ + 1; i >= 1; i--) {
    if (vertices_[i][best_ver]->getPreA() >= 0) {
      MatchEnvPtrVec prev_envs = vertices_[i][best_ver]->getPreMatchEnvs();
      addEnv(results_, prev_envs);
    }
    best_ver = vertices_[i][best_ver]->getPreA();
    if (best_ver < 0) {
      break;
    }
  }
  std::sort(results_.begin(), results_.end(), MatchEnv::cmpScoreDec);
}

}
