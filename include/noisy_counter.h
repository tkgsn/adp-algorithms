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

public:
    NoisyCounter(float epsilon_query, float epsilon_threshold, int k, int seed=0): Counter(k){
        this->epsilon_query = epsilon_query;
        laplace_query = make_laplace(epsilon_query, k);
        laplace_threshold = make_laplace(epsilon_threshold, 1);
    };
    float query(string) override;
    float threshold_query(string, float) override;
};

#endif