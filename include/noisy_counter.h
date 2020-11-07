#include "counter.h"
#include <boost/math/distributions/laplace.hpp>
#include <boost/random.hpp>

class NoisyCounter : public Counter{
private:
    boost::random::laplace_distribution<> laplace_query;
    boost::random::laplace_distribution<> laplace_threshold;
    boost::random::mt19937 gen;

public:
    NoisyCounter(float epsilon_query, float epsilon_threshold, int seed=0){
        gen = boost::random::mt19937(seed);
        laplace_query = boost::random::laplace_distribution<>(0, 2/epsilon_query);
        laplace_threshold = boost::random::laplace_distribution<>(0, 2/epsilon_threshold);
    };
    float query(string) override;
    float threshold_query(string, float) override;
};