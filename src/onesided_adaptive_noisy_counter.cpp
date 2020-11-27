#include "onesided_adaptive_noisy_counter.h"
using namespace std;

int OnesidedAdaptiveNoisyCounter::random_threshold(){
    return Counter::random_threshold() + 200;
}

map<int, float> OnesidedAdaptiveNoisyCounter::measure(map<int, float> res, float epsilon=0){
  pair<int, float> temp;
  map<int, float> measured_dict;

  BOOST_FOREACH (temp, res){
    if (temp.second == -1 || temp.second == 0) {
      continue;
    }
    float epsilon = used_budget[temp.first] / k;
    measured_dict[temp.first] = temp.second - ( 1 / epsilon ) + threshold;
  }

  return measured_dict;
}