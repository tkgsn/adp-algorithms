#ifndef INCLUDED_UTIL
#define INCLUDED_UTIL

#include <map>
#include <string>
#include <vector>
using namespace std;

int max_index(map<string,int>);
vector<string> arg_sort(map<string, int>);
pair<float, float> split_epsilon(float, int);

#endif