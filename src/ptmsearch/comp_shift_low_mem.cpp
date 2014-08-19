#include <cmath>

#include "base/logger.hpp"
#include "ptmsearch/comp_shift_low_mem.hpp"

namespace prot {

std::vector<std::vector<int>> CompShiftLowMem::findBestShift(
    const std::vector<int> &a,const std::vector<int> &b){
  return findBestShift(a,b,1,0);
}

std::vector<double> CompShiftLowMem::findBestShift(const std::vector<int> &a,
                                                   const std::vector<int> &errors,
                                                   const std::vector<int> &b,
                                                   int total,int min_gap,
                                                   double scale){
  std::vector<std::vector<int>> list = findBestShift(a,errors,b,total,min_gap);
  std::vector<double> result;
  for(size_t i = 0;i<list.size();i++){
    result.push_back(list[i][0]/scale);
  }
  return result;
}

std::vector<std::vector<int>> CompShiftLowMem::findBestShift(
    const std::vector<int> &a, const std::vector<int> &errors,
    const std::vector<int> &b, int total,int minimum_gap){
  int a_length = a.size();
  int b_length = b.size();
  std::vector<std::vector<int>> ans;
  if(b_length == 0){
    return ans;
  }
  int delta = a[0]-b[b_length-1];
  int minD = delta-1;
  int maxD = a[a_length-1]-b[0]+1+errors[a_length-1];
  if(maxD-minD + 1 >= (int)num_.size()){
    size_t required_len = maxD - minD + 1;
    for (size_t i = num_.size(); i < required_len; i++) {
      num_.push_back(0);
    }
  }
  for(int i=0;i<a_length;i++){
    int a_value = a[i];
    for(int j =0;j <b_length;j++){
      int d = a_value - b[j]-delta;
      int start = 0;
      if(d >errors[i]){
        start = d- errors[i];
      }
      for(int k = start;k<=d+errors[i];k++){
        num_[k]=0;
      }
    }
  }

  int current_minimum =1;

  for(int i=0;i<a_length;i++){
    int a_value = a[i];
    for(int j =0;j <b_length;j++){
      int d = a_value - b[j]-delta;
      int start = 0;
      if(d >errors[i]){
        start = d- errors[i];
      }
      for(int k = start;k<=d+errors[i];k++){
        num_[k]++;
        current_minimum = checkD(ans,k,current_minimum,total,minimum_gap);
      }
    }
  }

  for(size_t i=0;i<ans.size();i++){
    ans[i][0] += minD;
  }
  return ans;
}

inline std::vector<std::vector<int>> CompShiftLowMem::findBestShift(
    const std::vector<int> &a, const std::vector<int> &b,int total,int min_gap) {
  const int a_length = a.size();
  const int b_length = b.size();
  std::vector<std::vector<int>> ans;
  const int delta = a[0]-b[b_length-1];
  const int minD = delta-1;
  const int maxD = a[a_length-1]-b[0]+1;
  if(maxD-minD + 1 >= (int)num_.size()) {
    int required_len = maxD - minD + 1;
    for (int i = num_.size(); i < required_len; i++) {
      num_.push_back(0);
    }
  }

  for(int i=0;i<a_length;i++){
    int a_value = a[i];
    for(int j =0;j <b_length;j++){
      int d = a_value - b[j]-delta;
      num_[d] =0;
      num_[d+1] =0;
      num_[d+2] = 0;
    }
  }

  for(int i=0;i<a_length;i++){
    int a_value = a[i];
    for(int j =0;j <b_length;j++){
      int d = a_value - b[j]-delta;
      num_[d] ++;
      num_[d+1] ++;
      num_[d+2] ++;
    }
  }

  int cur_min =1;

  for(int i=0;i<a_length;i++){
    int a_value = a[i];
    for(int j =0;j <b_length;j++){
      int d = a_value - b[j]-delta;
      cur_min=checkD(ans,d+1,cur_min,total,min_gap);
      cur_min=checkD(ans,d,cur_min,total,min_gap);
      cur_min=checkD(ans,d+2,cur_min,total,min_gap);
    }
  }

  for(size_t i=0;i<ans.size();i++){
    ans[i][0] +=minD;
  }
  return ans;
}

inline int CompShiftLowMem::checkD(std::vector<std::vector<int>> &ans,int d,
                                   int cur_min,int total,int min_gap){
  short new_value = num_[d];
  if(new_value < cur_min){
    return cur_min;
  }
  for(size_t i =0;i<ans.size();i++){
    std::vector<int>  cur = ans[i];
    if(std::abs(cur[0]-d)<= min_gap){
      if(cur[1]<new_value){
        ans.erase(ans.begin()+i);
        i--;
      }
      else {
        return cur_min;
      }
    }
  }

  int insert_pos = ans.size()-1;

  while(insert_pos >= 0 && ans[insert_pos][1] < new_value){
    insert_pos--;
  }
  std::vector<int> insert_temp = {d,new_value};
  ans.insert(ans.begin()+insert_pos+1, insert_temp);
  if((int)ans.size()>total){
    ans.pop_back();
  }

  if ((int)ans.size() == total) {
    return ans[ans.size()-1][1]+1;
  }
  else {
    return 1;
  }
}

} /* namespace prot */
