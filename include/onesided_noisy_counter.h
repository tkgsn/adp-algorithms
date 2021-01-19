#ifndef INCLUDED_ONESIDED_NOISY_COUNTER
#define INCLUDED_ONESIDED_NOISY_COUNTER

#include "noisy_counter.h"
#include "util.h"
using namespace std;

class OnesidedNoisyCounter : public NoisyCounter{

public:
    OnesidedNoisyCounter(float epsilon, int k, bool rnam, int seed=0): NoisyCounter(epsilon, k, rnam, seed){
        pair<float, float> splitted_epsilon = split_epsilon(epsilon, k);
        this->epsilon_query = splitted_epsilon.second;
        this->epsilon_threshold = splitted_epsilon.first;
        if(rnam){
            this->epsilon_query = epsilon;
        }
        laplace_query = make_onesided_laplace(epsilon_query, k);
    };
    virtual map<int, float> top_k(map<int, float>);
    virtual map<int, float> measure_rnam_w_free_gap(map<int, float>, float);
};

#endif