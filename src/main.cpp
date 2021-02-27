#include "noisy_counter.h"
#include "util.h"
#include "onesided_adaptive_noisy_counter.h"
#include "onesided_noisy_counter.h"
#include <boost/foreach.hpp>
#include <iomanip>
#include <json.h>
#include <iomanip>
using namespace std;

template<class COUNTER>
void run_rnam(string mec, string alg, string db, float epsilon, int k=100){

	tuple<float, float, float, float> t;

	float epsilon_for_alg = epsilon;

	if (alg == "onesided"){
		epsilon_for_alg = epsilon;
	}else{
		epsilon_for_alg = epsilon / 2;
	}

	int n_iter = 10000;
	COUNTER counter = COUNTER(epsilon_for_alg, k, true);
	counter.set_k(k);
	counter.loadHist(db);
	nlohmann::json out_j;

	for(int i=0; i<n_iter; i++){
		nlohmann::json in_j;
		map<int, float> res = counter.report_noisy_k_max();
		map<int, float> measured = counter.measure_rnam_w_free_gap(res, epsilon);
		vector<string> sorted_res = arg_sort_result(res);

		float error = counter.compute_mean_squared_error(measured);
		float precision = counter.evaluate_precision(res);
		float recall = counter.evaluate_recall(res);
		float f_value = counter.evaluate_f_value(res);
		cout << epsilon << "\t" << i << ":\t" << error << "\t" << precision << "\t" << recall << "\t" << f_value << endl;

		std::get<0>(t) += precision / n_iter;
		std::get<1>(t) += recall / n_iter;
		std::get<2>(t) += f_value / n_iter;
		std::get<3>(t) += error / n_iter;

		in_j["error"] = error;
		in_j["precision"] = precision;
		in_j["recall"] = recall;
		in_j["f_value"] = f_value;

		out_j[to_string(i)] = in_j;
	}

	std::ofstream o("result/" + mec + "_" + alg + "_" + db + "_epsilon" + to_string(epsilon) + "_k" + to_string(k) + ".json");
	o << std::setw(4) << out_j << endl;
}

template<class COUNTER>
void run_svt(string mec, string alg, string db, float epsilon, int k=15){
	tuple<float, float, float, float, float, float> t;

	float ratio_of_bigger_epsilon = 0.5;
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

	cout << "epsilon" << "\t" << "i" << ":\t" << "error" << "\t" << "accuracy" << "\t" << "precision" << "\t" << "recall" << "\t" << "f_value" << endl;

	for(int i=0; i<n_iter; i++){

		nlohmann::json in_j;

		map<int, float> res = counter.run_sparse_vector();
		vector<string> sorted_res = arg_sort_result(res);
		map<int, float> measured = counter.measure(res, epsilon_for_measure);
		map<int, float> measured_w_free_gap = counter.measure_w_free_gap(res, epsilon_for_measure);
		
		float error = counter.compute_mean_squared_error(measured);
		float error_w_free_gap = counter.compute_mean_squared_error(measured_w_free_gap);
		float precision = counter.evaluate_precision(res);
		float recall = counter.evaluate_recall(res);
		float f_value = counter.evaluate_f_value(res);
		float accuracy = counter.evaluate_accuracy(sorted_res);

		cout << epsilon << "\t" << i << ":\t" << error << "\t" << accuracy << "\t" << precision << "\t" << recall << "\t" << f_value << endl;

		std::get<0>(t) += precision / n_iter;
		std::get<1>(t) += recall / n_iter;
		std::get<2>(t) += f_value / n_iter;
		std::get<3>(t) += error / n_iter;
		std::get<4>(t) += error_w_free_gap / n_iter;
		std::get<5>(t) += accuracy / n_iter;

		in_j["error"] = error_w_free_gap;
		in_j["error_"] = error;
		in_j["precision"] = precision;
		in_j["recall"] = recall;
		in_j["f_value"] = f_value;
		in_j["accuracy"] = accuracy;

		out_j[to_string(i)] = in_j;
	}

	std::ofstream o("result/" + mec + "_" + alg + "_" + db + "_epsilon" + to_string(epsilon) + "_k" + to_string(k) + ".json");
	o << std::setw(4) << out_j << endl;
}

int main(int argc, char *argv[]) {
	string alg = argv[1];
	string db = argv[2];
	string mec = argv[3];

	const std::vector<float> epsilons{0.1, 0.3, 0.5, 0.7, 1};
	int k = 100;
	BOOST_FOREACH(float epsilon, epsilons){
		if (mec == "svt"){
			if (alg == "onesided"){
				run_svt<OnesidedAdaptiveNoisyCounter>(mec, alg, db, epsilon, k);
			}else if (alg == "adaptive"){
				run_svt<AdaptiveNoisyCounter>(mec, alg, db, epsilon, k);
			}
		}else if (mec == "rnam"){
			if (alg == "onesided"){
				run_rnam<OnesidedNoisyCounter>(mec, alg, db, epsilon, k);
			}else if (alg == "adaptive"){
				run_rnam<NoisyCounter>(mec, alg, db, epsilon, k);
			}
		}
	}

	const std::vector<int> ks{10, 50, 100, 200, 500};
	float epsilon = 0.5;
	BOOST_FOREACH(int k, ks){
		if (mec == "svt"){
			if (alg == "onesided"){
				run_svt<OnesidedAdaptiveNoisyCounter>(mec, alg, db, epsilon, k);
			}else if (alg == "adaptive"){
				run_svt<AdaptiveNoisyCounter>(mec, alg, db, epsilon, k);
			}
		}else if (mec == "rnam"){
			if (alg == "onesided"){
				run_rnam<OnesidedNoisyCounter>(mec, alg, db, epsilon, k);
			}else if (alg == "adaptive"){
				run_rnam<NoisyCounter>(mec, alg, db, epsilon, k);
			}
		}
	}
}