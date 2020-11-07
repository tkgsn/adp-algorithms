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
  if(this->query(index) <= threshold){
    return 0;
  }else{
    return this->query(index) - threshold;
  }
}