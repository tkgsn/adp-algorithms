#include "counter.h"
using namespace std;

map<std::string,int> loadToHist() {
  ifstream ifs("dataset/kosarak/data.txt");
  string tmp;
  map<string,int> count;

  while(getline(ifs, tmp)){
    stringstream ss;
    ss << tmp;
    while(!ss.eof()){
      ss >> tmp;
      count[tmp] += 1;
    }
  }

  cout << "Kitty on your lap" << endl;
  
  return count;
}