#include "util.h"
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