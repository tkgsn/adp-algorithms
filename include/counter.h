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
#include <boost/foreach.hpp>
#include <random>
using namespace std;

class Counter{
protected:
    float budget = 0;
    int k;
    int choosed_k;
    float threshold;
private:
    map<string,int> hist;
    vector<string> sorted;
    virtual void sum_budget(float);
    virtual bool judge_budget();
    default_random_engine e = default_random_engine(0);
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
    virtual int random_threshold();
    map<int, float> run_sparse_vector();
    float compute_mean_squared_error(map<int, float>);
    float evaluate_precision(map<int, float>);
    float evaluate_recall(map<int, float>);
    float evaluate_f_value(map<int, float>);
};

#endif