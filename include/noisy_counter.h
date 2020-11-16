#ifndef INCLUDED_NOISY_COUNTER
#define INCLUDED_NOISY_COUNTER

#include "counter.h"
#include "util.h"
using namespace std;

class NoisyCounter : public Counter{
protected:
    boost::random::laplace_distribution<> laplace_query;
    boost::random::laplace_distribution<> laplace_threshold;
    default_random_engine gen = std::default_random_engine(0);

public:
    NoisyCounter(float epsilon_query, float epsilon_threshold, int k, int seed=0): Counter(k){
        this->epsilon_query = epsilon_query;
        laplace_query = boost::random::laplace_distribution<>(0, 2*k/epsilon_query);
        laplace_threshold = boost::random::laplace_distribution<>(0, 2/epsilon_threshold);
    };
    float query(string) override;
    float threshold_query(string, float) override;
};

#endif