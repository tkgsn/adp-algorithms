#include "noisy_counter.h"
using namespace std;

float NoisyCounter::query(string index){
    return Counter::query(index) + laplace_query(gen);
}

float NoisyCounter::threshold_query(string index, float threshold){
    return Counter::threshold_query(index, threshold + laplace_threshold(gen));
}