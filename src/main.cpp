#include "noisy_counter.h"
#include "adaptive_noisy_counter.h"
using namespace std;

int main() {
	tuple<float, float, float> t;
	AdaptiveNoisyCounter counter = AdaptiveNoisyCounter(1, 1, 5);
	counter.loadHist("kosarak");
	int n_iter = 100;
	for(int i=0; i<n_iter; i++){
		map<int, float> res = counter.run_sparse_vector();
		std::get<0>(t) += counter.evaluate_precision(res) / n_iter;
		std::get<1>(t) += counter.evaluate_recall(res) / n_iter;
		std::get<2>(t) += counter.evaluate_f_value(res) / n_iter;
	}
	cout << std::get<0>(t) << endl;
	cout << std::get<1>(t) << endl;
	cout << std::get<2>(t) << endl;
}