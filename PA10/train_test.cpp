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
	array<size_t,10> correct_count;
	array<size_t,10> label_count;
	vector<float> accuracy;
	float total_accuracy = 0;
	fill(begin(correct_count), end(correct_count), 0);
	fill(begin(label_count), end(label_count), 0);
	vector<float> input;
	vector<float> prediction;
	uint8_t label;
	size_t total_image = data.get_data_size(TEST_LABEL);

	for (size_t i = 0; i < total_image; i++) {
		input = data.get_a_normalized_image(TEST_IMAGE,i);
		prediction = nn(input);
		label = data.get_a_label(TEST_LABEL,i);
		if(max_element(prediction.begin(),prediction.end()) - prediction.begin() == label) {
			correct_count[label]++;
		}
		label_count[label]++;
	}
	
	for(uint8_t i = 0; i < 10; i++) {
		accuracy.push_back(1.0 * correct_count[i] / label_count[i]);
		total_accuracy += accuracy[i] * label_count[i] / total_image;
	}

	printf("Overall accuracy: %.2f%%\n",total_accuracy*100);
	for(int i = 0; i < 10;i++) {
		printf("Accuracy for digit %d: %.2f%%\n",i,accuracy[i]*100);
	}
	fflush(stdout);
	// END OF YOUR IMPLEMENTATION
}
