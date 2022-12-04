#include <algorithm>
#include <numeric>
#include "dataset.h"
#include "nn.h"
using namespace std;

void training(NN&, dataset&, const size_t&);
void testing(NN&, dataset&);

void nn_training_testing(dataset& data){		
	NN nn {
		new LayerLinear(PIXELS, 32),
		new LayerSigmoid,
		new LayerLinear(32   , 16),
		new LayerSigmoid,
		new LayerLinear(16   , 10),
		new LayerSigmoid,
	};

	for (size_t epoch = 0; epoch < 10; epoch++)
	{	
		training(nn, data, epoch);
		testing(nn, data);
	}
	// nn.save("nn1.bin");	
}


void training(NN& nn, dataset& data, const size_t& epoch){
	vector<size_t> Sequence(data.get_data_size(TRAIN_LABEL));
	iota(Sequence.begin(), Sequence.end(), 0);
	random_shuffle(Sequence.begin(), Sequence.end());

	printf("================ Epoch %lu ================\n", epoch+1);

	for (size_t i = 0; i < Sequence.size(); i++) {
		vector<float> input = data.get_a_normalized_image(TRAIN_IMAGE, Sequence[i]);
		vector<float> one_hot_label(10);
		one_hot_label[data.get_a_label(TRAIN_LABEL, Sequence[i])] = 1;
		
		nn.backprop(input, one_hot_label);

		if ((i + 1) % 10 == 0)
			nn.apply();

		if ((i + 1) % 500 == 0) {
			printf("Epoch %lu: %lu / %lu\r", epoch+1, i+1, Sequence.size());
			fflush(stdout);
		}
	}
}


void testing(NN& nn, dataset& data){
	// START OF YOUR IMPLEMENTATION

	uint8_t prediction;
	uint8_t label;
	array<size_t, 11> correct_count = {};	// The array will be initialized to 0 if we provide the empty initializer list.
	array<size_t, 11> total_count = {};

	for (size_t i = 0; i < data.get_data_size(TEST_LABEL); i++) {
		vector<float> input = data.get_a_normalized_image(TEST_IMAGE, i);
		vector<float> output = nn(input);

		prediction = max_element(output.begin(), output.end()) - output.begin();
		label = data.get_a_label(TEST_LABEL, i);

		correct_count[prediction] +=  prediction == label;
		++total_count[label];

		correct_count.back() +=  prediction == label;
		++total_count.back();
	}

	float acc = 100.0f * correct_count.back() / total_count.back();
	printf("Overall accuracy: %.2f%%\n", acc);

	for (size_t i = 0; i < 10; ++i){
		acc = 100.0f * correct_count[i] / total_count[i];
		printf("Accuracy for digit %lu: %.2f%%\n", i, acc);
	}

	// END OF YOUR IMPLEMENTATION
}