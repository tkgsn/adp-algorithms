#include "noisy_counter.h"
#include "onesided_adaptive_noisy_counter.h"
#include <boost/foreach.hpp>
using namespace std;

int main() {
	tuple<float, float, float> t;
	float epsilon = 0.7;
	float ratio_of_bigger_epsilon = 0.5;

	int k = 15;
	float epsilon_for_svt = epsilon / 2;
	float epsilon_for_measure = epsilon / 2;
	float error = 0;
	float error_w_free_gap = 0;
	pair<float, float> splitted_epsilon = split_epsilon(epsilon_for_svt, k);
	//OnesidedAdaptiveNoisyCounter counter = OnesidedAdaptiveNoisyCounter(epsilon_for_svt, k, ratio_of_bigger_epsilon);
	//AdaptiveNoisyCounter counter = AdaptiveNoisyCounter(splitted_epsilon.second, splitted_epsilon.first, k, ratio_of_bigger_epsilon);
	NoisyCounter counter = NoisyCounter(splitted_epsilon.second, splitted_epsilon.first, k);
	counter.loadHist("kosarak");
	int n_iter = 100;
	for(int i=0; i<n_iter; i++){
		map<int, float> res = counter.run_sparse_vector();
		map<int, float> measured = counter.measure(res, epsilon_for_measure);
		map<int, float> measured_w_free_gap = counter.measure_w_free_gap(res, epsilon_for_measure);
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
		error += counter.compute_mean_squared_error(measured) / n_iter;
		error_w_free_gap += counter.compute_mean_squared_error(measured_w_free_gap) / n_iter;
	}
	cout << std::get<0>(t) << endl;
	cout << std::get<1>(t) << endl;
	cout << std::get<2>(t) << endl;
	cout << error << endl;
	cout << error_w_free_gap << endl;
}