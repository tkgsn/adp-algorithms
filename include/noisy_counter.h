#ifndef INCLUDED_NOISY_COUNTER
#define INCLUDED_NOISY_COUNTER

#include "counter.h"
#include "util.h"
using namespace std;

class NoisyCounter : public Counter{
protected:
    std::function<float(default_random_engine&)> laplace_query;
    std::function<float(default_random_engine&)> laplace_threshold;
    default_random_engine gen = std::default_random_engine(0);
    map<int, float> used_budget;
    float epsilon_threshold;
    float epsilon_query;
    float epsilon;

public:
    NoisyCounter(float epsilon, int k, bool rnam, int seed=0): Counter(k){
        pair<float, float> splitted_epsilon = split_epsilon(epsilon, k);
        this->epsilon_query = splitted_epsilon.second;
        this->epsilon_threshold = splitted_epsilon.first;
        if(rnam){
            this->epsilon_query = epsilon;
        }
        laplace_query = make_laplace(epsilon_query, k);
        laplace_threshold = make_laplace(epsilon_threshold, 1);
    };
    virtual map<int, float> measure(map<int, float>, float);
    virtual map<int, float> measure_w_free_gap(map<int, float>, float);
    map<int, float> measure_rnam_w_free_gap(map<int, float>, float);
    float query(string) override;
    float threshold_query(string, float) override;
    void sum_budget(int, float) override;
    bool judge_budget() override;
};

#endif