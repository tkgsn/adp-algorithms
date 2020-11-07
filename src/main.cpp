#include "noisy_counter.h"
using namespace std;

int main() {
	Counter counter = Counter();
	NoisyCounter noisy_counter = NoisyCounter(1, 1);
	counter.loadHist("kosarak");
	noisy_counter.loadHist("kosarak");
	string index = "2";
	cout << counter.query(index) << endl;
	cout << noisy_counter.query(index) << endl;
	cout << counter.threshold_query(index, 100) << endl;
	cout << noisy_counter.threshold_query(index, 100) << endl;
	return 0;
}