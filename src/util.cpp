#include "util.h"
#include <map>
#include <string>
#include <boost/foreach.hpp>
#include <algorithm>
#include <numeric>
#include <vector>
#include <iostream>
#include <cmath>

using namespace std;

int max_index(map<string,int> dict){
    pair<string, int> me;

    int max_value = 0;

    BOOST_FOREACH (me, dict) {
        int value = stoi(me.first);
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

pair<float, float> split_epsilon(float epsilon, int k){
    float theta = 1. / (1 + std::pow(k, 2./3.));
    return make_pair(epsilon * theta, epsilon * (1 - theta));
}