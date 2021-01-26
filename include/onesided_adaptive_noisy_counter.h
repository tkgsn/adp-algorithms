#ifndef ONESIDED_INCLUDED_ADAPTIVE_NOISY_COUNTER
#define ONESIDED_INCLUDED_ADAPTIVE_NOISY_COUNTER

#include <math.h>
#include "adaptive_noisy_counter.h"
#include "util.h"
using namespace std;

class OnesidedAdaptiveNoisyCounter : public AdaptiveNoisyCounter{
public:
    OnesidedAdaptiveNoisyCounter(float epsilon_query, int k, float ratio_of_bigger_epsilon=0.5, int seed=0): AdaptiveNoisyCounter(epsilon_query, k){
        this->bigger_epsilon = epsilon_query * ratio_of_bigger_epsilon;
        this->epsilon_query = epsilon_query;
        this->bigger_laplace_query = make_onesided_laplace(this->bigger_epsilon, k);
        this->laplace_query = make_onesided_laplace(this->epsilon_query, k);
        this->laplace_threshold = make_zero_noise();
    };
    int random_threshold() override;
    map<int, float> measure(map<int, float>, float) override;
    map<int, float> measure_w_free_gap(map<int, float>, float) override;
};

#endif