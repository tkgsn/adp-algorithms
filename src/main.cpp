#include "noisy_counter.h"
#include "util.h"
#include "onesided_adaptive_noisy_counter.h"
#include <boost/foreach.hpp>
#include <json.h>
#include <iomanip>
using namespace std;


template<class COUNTER>
void run(string alg, string db, std::vector<float> epsilons){

	BOOST_FOREACH(float epsilon, epsilons){

		tuple<float, float, float, float, float> t;

		float ratio_of_bigger_epsilon = 0.5;
		int k = 15;
		float epsilon_for_svt;
		float epsilon_for_measure = epsilon / 2;
		int n_iter = 10000;

		if (alg == "onesided"){
			epsilon_for_svt = epsilon;
		}else{
			epsilon_for_svt = epsilon / 2;
		}


		COUNTER counter = COUNTER(epsilon_for_svt, k, ratio_of_bigger_epsilon);
		counter.loadHist(db);
		nlohmann::json out_j;

		for(int i=0; i<n_iter; i++){

			nlohmann::json in_j;

			map<int, float> res = counter.run_sparse_vector();
			map<int, float> measured = counter.measure(res, epsilon_for_measure);
			map<int, float> measured_w_free_gap = counter.measure_w_free_gap(res, epsilon_for_measure);
			
			float error = counter.compute_mean_squared_error(measured);
			float error_w_free_gap = counter.compute_mean_squared_error(measured_w_free_gap);
			float precision = counter.evaluate_precision(res);
			float recall = counter.evaluate_recall(res);
			float f_value = counter.evaluate_f_value(res);

			std::get<0>(t) += precision / n_iter;
			std::get<1>(t) += recall / n_iter;
			std::get<2>(t) += f_value / n_iter;
			std::get<3>(t) += error / n_iter;
			std::get<4>(t) += error_w_free_gap / n_iter;

			in_j["error"] = error;
			in_j["error_w_free_gap"] = error_w_free_gap;
			in_j["precision"] = precision;
			in_j["recall"] = recall;
			in_j["f_value"] = f_value;

			out_j[to_string(i)] = in_j;
		}

		std::ofstream o("result/" + alg + "_" + db + "_epsilon" + to_string(epsilon) + ".json");
		o << std::setw(4) << out_j << endl;
		cout << std::get<0>(t) << endl;
		cout << std::get<1>(t) << endl;
		cout << std::get<2>(t) << endl;
		cout << std::get<3>(t) << endl;
		cout << std::get<4>(t) << endl;
	}
}

int main(int argc, char *argv[]) {
	string alg = argv[1];
	string db = argv[2];

	const std::vector<float> epsilons{0.1, 0.3, 0.5, 0.7, 1};

	if (alg == "onesided"){
		run<OnesidedAdaptiveNoisyCounter>(alg, db, epsilons);
	}else if (alg == "adaptive"){
		run<AdaptiveNoisyCounter>(alg, db, epsilons);
	}
}
