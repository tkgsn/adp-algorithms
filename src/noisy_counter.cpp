#include "noisy_counter.h"
using namespace std;

float NoisyCounter::query(string index){
    return Counter::query(index) + this->laplace_query(gen);
}

float NoisyCounter::threshold_query(string index, float threshold){
    return Counter::threshold_query(index, threshold + laplace_threshold(gen));
}

map<int, float> NoisyCounter::measure(map<int, float> res, float epsilon){
  pair<int, float> temp;
  this->laplace_query = make_laplace(epsilon, k);
  map<int, float> measured_dict;

  BOOST_FOREACH (temp, res){
    if (temp.second == -1 || temp.second == 0) {
      continue;
    }
    float measured = this->query(to_string(temp.first));
    measured_dict[temp.first] = measured;
  }

  this->laplace_query = make_laplace(this->epsilon_query, k);

  return measured_dict;
}

map<int, float> NoisyCounter::measure_rnam_w_free_gap(map<int, float> res, float epsilon){
  pair<int, float> temp;
  map<int, float> measured_dict;

  BOOST_FOREACH (temp, res){
    float measured = this->query(to_string(temp.first));
    measured_dict[temp.first] = measured;
  }

  float alpha = 0;
  float p = 0;
  vector<float> ps;
  ps.push_back(0);

  for(int i=1; i<=k; i++){
    alpha += measured_dict[queries[i-1]];
    p += (k-i)*res[queries[i-1]];
    ps.push_back(ps[i-1] + res[queries[i-1]]);
  }

  float lambda = 1;
  map<int, float> beta;
  for(int i=1; i<=k; i++){
    beta[queries[i-1]] = (alpha + lambda * k * measured_dict[queries[i-1]] + p - k * ps[i-1])/((1+lambda)*k);
  }
  return beta;
}

map<int, float> NoisyCounter::measure_w_free_gap(map<int, float> res, float epsilon){
  pair<int, float> temp;

  float var_alpha = 8 * k * k / (epsilon * epsilon);

  this->laplace_query = make_laplace(epsilon, k);

  map<int, float> measured_dict;

  BOOST_FOREACH (temp, res){
    if (temp.second == -1 || temp.second == 0) {
      continue;
    }
    float measured = this->query(to_string(temp.first));
    float var_gamma = 8 * std::pow(1 + std::pow(k, 2./3.), 3) / (used_budget[temp.first] * used_budget[temp.first]);
    float estimated = ( (measured / var_alpha) + (temp.second + threshold) / var_gamma ) / ( 1 / var_alpha + 1 / var_gamma );
    measured_dict[temp.first] = estimated;
  }

  this->laplace_query = make_laplace(this->epsilon_query, k);

  return measured_dict;
}

void NoisyCounter::sum_budget(int index, float queried){
  if(queried != -1){
    used_budget[index] = epsilon_query; 
    budget += epsilon_query / k;
  }
}

bool NoisyCounter::judge_budget(){
  if(budget > epsilon_query){
    return true;
  }else{
    return false;
  }
}