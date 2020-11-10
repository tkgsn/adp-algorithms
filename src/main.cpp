#include "noisy_counter.h"
using namespace std;

int main() {
	NoisyCounter counter = NoisyCounter(1, 1);
	counter.loadHist("kosarak");
	counter.run_sparse_vector(5);
}