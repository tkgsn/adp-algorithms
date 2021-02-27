#include "util.h"
#include <map>
#include <string>
#include <boost/foreach.hpp>
#include <algorithm>
#include <numeric>
#include <vector>
#include <iostream>
#include <cmath>
#include <functional>
#include <boost/random.hpp>
#include <random>

using namespace std;

int max_index(map<string,int> dict){
    pair<string, int> me;

    int max_value = 0;

    BOOST_FOREACH (me, dict) {
        int value = std::stoi(me.first);
        if(value > max_value){
            max_value = value;
        }
    }

    return max_value;
}

vector<string> arg_sort(map<string, int> dict){
    pair<string, int> me;
    vector<string> indices;

    BOOST_FOREACH (me, dict) {
        indices.push_back(me.first);
    }

    sort(indices.begin(), indices.end(), [&dict](string i1, string i2){return dict[i1] > dict[i2];});
    return indices;
}

vector<string> arg_sort_result(map<int, float> dict){
    pair<int, float> me;
    vector<string> indices;

    BOOST_FOREACH (me, dict) {
        indices.push_back(to_string(me.first));
    }

    sort(indices.begin(), indices.end(), [&dict](string i1, string i2){return dict[stoi(i1)] > dict[stoi(i2)];});
    return indices;
}

pair<float, float> split_epsilon(float epsilon, int k){
    float theta = 1. / (1 + std::pow(k, 2./3.));
    return make_pair(epsilon * theta, epsilon * (1 - theta));
}

std::function<float(default_random_engine&)> make_zero_noise(){
    std::function<float(default_random_engine&)> f = [](default_random_engine& gen){
        return 0;
        };
    return f;
}

std::function<float(default_random_engine&)> make_laplace(float epsilon, int k){
    boost::random::laplace_distribution<> laplace = boost::random::laplace_distribution<>(0, k * 2 / epsilon);
    std::function<float(default_random_engine&)> f = [=](default_random_engine& gen){
        return laplace(gen);
        };
    return f;
}


std::function<float(default_random_engine&)> make_onesided_laplace(float epsilon, int k){
    boost::random::exponential_distribution<> laplace = boost::random::exponential_distribution<>(epsilon/k);
    std::function<float(default_random_engine&)> f = [=](default_random_engine& gen){
        return laplace(gen);
        };
    return f;
}

// std::function<float(default_random_engine&)> make_onesided_laplace(float epsilon, int k){
//     boost::random::laplace_distribution<> laplace = boost::random::laplace_distribution<>(0, k / epsilon);
//     std::function<float(default_random_engine&)> f = [=](default_random_engine& gen){
//         return std::abs(laplace(gen));
//         };
//     return f;
// }
