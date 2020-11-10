#include "counter.h"
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
    return 0;
  }else{
    return res - threshold;
  }
}

vector<int> Counter::random_query(){
  int max = max_index(hist);
  vector<int> indices(max, 0);
  iota(indices.begin(), indices.end(), 1);
  shuffle( indices.begin(), indices.end(), e );
  cout << indices[0] << endl;
  return indices;
}