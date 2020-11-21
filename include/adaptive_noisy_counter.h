#ifndef INCLUDED_ADAPTIVE_NOISY_COUNTER
#define INCLUDED_ADAPTIVE_NOISY_COUNTER

#include <math.h>
#include "noisy_counter.h"
#include "util.h"
using namespace std;

class AdaptiveNoisyCounter : public NoisyCounter{
protected:
    float sigma;
    float bigger_epsilon;
public:
    AdaptiveNoisyCounter(float epsilon_query, float epsilon_threshold, int k, int seed=0): NoisyCounter(epsilon_query, epsilon_threshold, k){
        this->epsilon_query = epsilon_query;
        bigger_epsilon = epsilon_query / (2 * k);
        bigger_laplace_query = make_laplace(epsilon_query / 2, k);
        sigma = 4 * sqrt(2) * k * 2 / epsilon_query;
    };
    std::function<float(default_random_engine&)> bigger_laplace_query;
    float bigger_query(string);
    float threshold_query(string, float) override;
    void sum_budget(float) override;
};

#endif