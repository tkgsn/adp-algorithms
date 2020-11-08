#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <map>
using namespace std;

class Counter{
private:
    map<string,int> hist;
public:
    void loadHist(string);
    virtual float query(string);
    virtual float threshold_query(string, float);
};