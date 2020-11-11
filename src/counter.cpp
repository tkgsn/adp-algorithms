#include "counter.h"
#include <boost/foreach.hpp>
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

int Counter::random_threshold(int k){
  vector<string> sorted = arg_sort(hist);
  uniform_int_distribution<> uni_dis(2 * k, 8 * k);
  int sampled = uni_dis(e);
  choosed_k = sampled;
  return hist[sorted[sampled]];
}

map<int, float> Counter::run_sparse_vector(int k){
    vector<int> queries = Counter::random_query();
    float threshold = Counter::random_threshold(k);
    int index;
    int counter = 0;
    map<int, float> res;

    BOOST_FOREACH (index, queries) {
        float queried = this->threshold_query(to_string(index), threshold);
        if(queried != -1){
          counter += 1;
        }
        res[index] = queried;
        if (counter == k) break;
    }

    return res;
}

float Counter::evaluate_precision(map<int, float> res){
  vector<string> sorted = arg_sort(hist);

  pair<int, float> temp;
  int counter = 0;
  int right_counter = 0;

  BOOST_FOREACH (temp, res){
    if (temp.second == -1 || temp.second == 0) {
      continue;
    }
    counter += 1;
    if( find(sorted.begin(), sorted.begin() + choosed_k, to_string(temp.first)) != sorted.begin() + choosed_k ){
      right_counter += 1;
    }
  }
  return float(right_counter) / counter ;
}

float Counter::evaluate_recall(map<int, float> res){
  vector<string> sorted = arg_sort(hist);

  pair<int, float> temp;
  int right_counter = 0;

  BOOST_FOREACH (temp, res){
    if (temp.second == -1 || temp.second == 0) {
      continue;
    }
    if( find(sorted.begin(), sorted.begin() + choosed_k, to_string(temp.first)) != sorted.begin() + choosed_k ){
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