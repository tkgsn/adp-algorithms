#include "counter.h"
using namespace std;

int main() {
	map<string,int> count = loadToHist();
	cout << count["1"] << endl;
	return 0;
}