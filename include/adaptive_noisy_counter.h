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
    AdaptiveNoisyCounter(float epsilon, int k, float ratio_of_bigger_epsilon=0.5, int seed=0): NoisyCounter(epsilon, k, false){
        bigger_epsilon = epsilon_query * ratio_of_bigger_epsilon;
        bigger_laplace_query = make_laplace(bigger_epsilon, k);
        sigma = 4 * sqrt(2) * k * 2 / epsilon_query;
    };
    std::function<float(default_random_engine&)> bigger_laplace_query;
    float bigger_query(string);
    float threshold_query(string, float) override;
    void sum_budget(int, float) override;
};

#endif