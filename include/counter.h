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
protected:
    float epsilon_query = std::numeric_limits<float>::max();
    float budget = 0;
    int k;
private:
    map<string,int> hist;
    vector<string> sorted;
    virtual void sum_budget(float);
    bool judge_budget();
    default_random_engine e = default_random_engine(0);
    int choosed_k;
public:
    Counter(int k, int seed=0){
        this->k = k;
    };
    void loadHist(string);
    map<string,int> get_hist(){
        return hist;
    };
    virtual float query(string);
    virtual float threshold_query(string, float);
    vector<int> random_query();
    int random_threshold();
    map<int, float> run_sparse_vector();
    float evaluate_precision(map<int, float>);
    float evaluate_recall(map<int, float>);
    float evaluate_f_value(map<int, float>);
};

#endif