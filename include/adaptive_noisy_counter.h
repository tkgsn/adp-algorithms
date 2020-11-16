#ifndef INCLUDED_ADAPTIVE_NOISY_COUNTER
#define INCLUDED_ADAPTIVE_NOISY_COUNTER

#include <math.h>
#include "noisy_counter.h"
#include "util.h"
using namespace std;

class AdaptiveNoisyCounter : public NoisyCounter{
private:
    boost::random::laplace_distribution<> bigger_laplace_query;
    float sigma;
    float bigger_epsilon;
public:
    AdaptiveNoisyCounter(float epsilon_query, float epsilon_threshold, int k, int seed=0): NoisyCounter(epsilon_query, epsilon_threshold, k){
        this->epsilon_query = epsilon_query;
        bigger_epsilon = epsilon_query / (2 * k);
        bigger_laplace_query = boost::random::laplace_distribution<>(0, 2/bigger_epsilon);
        sigma = 4 * sqrt(2) * k * 2 / epsilon_query;
    };
    float bigger_query(string);
    float threshold_query(string, float) override;
    void sum_budget(float) override;
};

#endif