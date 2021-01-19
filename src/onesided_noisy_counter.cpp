#include "onesided_noisy_counter.h"
using namespace std;


map<int, float> OnesidedNoisyCounter::top_k(map<int, float> result){
  sort(queries.begin(), queries.end(), [&result](int i1, int i2){return result[i1] > result[i2];});
  map<int, float> res_topk;
  for(int i=0; i<k; i++){
    int index = queries[i];
    res_topk[index] = result[index];
  }
  return res_topk;
}

map<int, float> OnesidedNoisyCounter::measure_rnam_w_free_gap(map<int, float> res, float epsilon=0){
  pair<int, float> temp;
  map<int, float> measured_dict;

  BOOST_FOREACH (temp, res){
    float epsilon = this->epsilon_query;
    measured_dict[temp.first] = temp.second - ( 1 / epsilon );
  }

  return measured_dict;
}