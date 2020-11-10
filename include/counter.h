#ifndef INCLUDED_COUNTER
#define INCLUDED_COUNTER

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <map>
#include <numeric>
#include "util.h"
#include <boost/random.hpp>
#include <random>
using namespace std;

class Counter{
private:
    map<string,int> hist;
    default_random_engine e = default_random_engine(0);
public:
    void loadHist(string);
    map<string,int> get_hist(){
        return hist;
    };
    virtual float query(string);
    virtual float threshold_query(string, float);
    vector<int> random_query();
    int random_threshold(int);
    map<int, float> run_sparse_vector(int);
};

#endif