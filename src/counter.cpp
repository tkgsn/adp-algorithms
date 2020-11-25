#include "counter.h"
#include <limits>
using namespace std;

void Counter::loadHist(string dataname){
  ifstream ifs("dataset/" + dataname + "/data.txt");
  string tmp;

  while(getline(ifs, tmp)){
    stringstream ss;
    ss << tmp;
    while(!ss.eof()){
      ss >> tmp;
      hist[tmp] += 1;
    }
  }
  sorted = arg_sort(hist);
}

float Counter::query(string index){
  return hist[index];
}

float Counter::threshold_query(string index, float threshold){
  float res = this->query(index);
  if(res <= threshold){
    return -1;
  }else{
    return res - threshold;
  }
}

vector<int> Counter::random_query(){
  int max = max_index(hist);
  vector<int> indices(max, 0);
  iota(indices.begin(), indices.end(), 1);
  shuffle( indices.begin(), indices.end(), e );
  return indices;
}

int Counter::random_threshold(){
  uniform_int_distribution<> uni_dis(2 * k, 8 * k);
  int sampled = uni_dis(e);
  choosed_k = sampled;
  return hist[sorted[sampled]];
}

map<int, float> Counter::run_sparse_vector(){
    vector<int> queries = Counter::random_query();
    threshold = this->random_threshold();
    this->budget = 0;
    int index;
    map<int, float> res;

    BOOST_FOREACH (index, queries) {
      float queried = this->threshold_query(to_string(index), threshold);
      this->sum_budget(queried);
      if(this->judge_budget()) break;
      res[index] = queried;
    }

    return res;
}

void Counter::sum_budget(float queried){
}

bool Counter::judge_budget(){
  return false;
}

float Counter::compute_mean_squared_error(map<int, float> measured){
    pair<int, float> temp;
    int counter = 0;
    float res = 0;

    BOOST_FOREACH (temp, measured) {
      counter += 1;
      float accurate = Counter::query(to_string(temp.first));
      float mse = std::pow(temp.second - accurate, 2);
      res += mse;
    }

    return res / counter;
}

float Counter::evaluate_precision(map<int, float> res){
  pair<int, float> temp;
  int counter = 0;
  int right_counter = 0;

  BOOST_FOREACH (temp, res){
    if (temp.second == -1 || temp.second == 0) {
      continue;
    }
    counter += 1;
    if( find(sorted.begin(), sorted.begin() + choosed_k, to_string(temp.first)) != sorted.begin() + choosed_k ){
    //if( find(sorted.begin(), sorted.begin() + k, to_string(temp.first)) != sorted.begin() + k ){
      right_counter += 1;
    }
  }
  return float(right_counter) / counter ;
}

float Counter::evaluate_recall(map<int, float> res){
  pair<int, float> temp;
  int right_counter = 0;

  BOOST_FOREACH (temp, res){
    if (temp.second == -1 || temp.second == 0) {
      continue;
    }
    if( find(sorted.begin(), sorted.begin() + choosed_k, to_string(temp.first)) != sorted.begin() + choosed_k ){
    //if( find(sorted.begin(), sorted.begin() + k, to_string(temp.first)) != sorted.begin() + k ){
      right_counter += 1;
    }
  }
  return float(right_counter) / choosed_k;
}

float Counter::evaluate_f_value(map<int, float> res){
  float precision = Counter::evaluate_precision(res);
  float recall = Counter::evaluate_recall(res);
  return 1./( (1./2) * (1./precision + 1./recall) );
}