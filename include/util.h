#ifndef INCLUDED_UTIL
#define INCLUDED_UTIL

#include <functional>
#include <map>
#include <string>
#include <vector>
#include <random>
#include <functional>
using namespace std;

int max_index(map<string,int>);
vector<string> arg_sort(map<string, int>);
pair<float, float> split_epsilon(float, int);
std::function<float(default_random_engine&)> make_onesided_laplace(float, int);
std::function<float(default_random_engine&)> make_laplace(float, int);
std::function<float(default_random_engine&)> make_zero_noise();

#endif
