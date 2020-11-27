#include "adaptive_noisy_counter.h"
using namespace std;

float AdaptiveNoisyCounter::bigger_query(string index){
    return Counter::query(index) + this->bigger_laplace_query(gen);
}

float AdaptiveNoisyCounter::threshold_query(string index, float threshold){
    float queried =  bigger_query(index) - threshold - laplace_threshold(gen);
    if(queried <= 0){
        return -1;
    }
    if(queried >= sigma){
        return queried;
    }else{
        return NoisyCounter::threshold_query(index, threshold);
    }
}

void AdaptiveNoisyCounter::sum_budget(int index, float queried){
    if(queried == -1){
        return;
    }else if(queried >= sigma){
        used_budget[index] = bigger_epsilon;
        budget += bigger_epsilon / k;
    }else{
        used_budget[index] = epsilon_query;
        budget += epsilon_query / k;
    }
}