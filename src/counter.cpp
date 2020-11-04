#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <map>
using namespace std;


void readTest() {
  ifstream ifs("dataset/kosarak/data.txt");
  string tmp;
  map<std::string,int> count;

  while(getline(ifs, tmp)){
    stringstream ss;
    ss << tmp;
    while(!ss.eof()){
      ss >> tmp;
      count[tmp] += 1;
    }
  }

  std::cout << "Kitty on your lap";
  return;
}