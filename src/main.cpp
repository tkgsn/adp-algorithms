#include "noisy_counter.h"
#include "onesided_adaptive_noisy_counter.h"
#include <boost/foreach.hpp>
using namespace std;

int main() {
	tuple<float, float, float> t;
	float epsilon = 0.7;
	int k = 15;
	pair<float, float> epsilons = split_epsilon(epsilon, k);
	//OnesidedAdaptiveNoisyCounter counter = OnesidedAdaptiveNoisyCounter(epsilon, k);
	AdaptiveNoisyCounter counter = AdaptiveNoisyCounter(epsilons.second, epsilons.first, k);
	counter.loadHist("kosarak");
	int n_iter = 100;
	for(int i=0; i<n_iter; i++){
		map<int, float> res = counter.run_sparse_vector();
		// pair<int, float> temp;
	    // BOOST_FOREACH(temp, res){
		//   if(temp.second != -1){
		// 	  cout << temp.first << endl;
		// 	  cout << temp.second << endl;
		//   }
    	// }
		std::get<0>(t) += counter.evaluate_precision(res) / n_iter;
		std::get<1>(t) += counter.evaluate_recall(res) / n_iter;
		std::get<2>(t) += counter.evaluate_f_value(res) / n_iter;
	}
	cout << std::get<0>(t) << endl;
	cout << std::get<1>(t) << endl;
	cout << std::get<2>(t) << endl;
}