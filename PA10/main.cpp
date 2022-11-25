#include <cstdlib>
#include <ctime>
#include <algorithm>
#include <iostream>
#include <numeric>
#include <vector>
#include "dataset.h"
#include "nn.h"
using namespace std;

void nn_training_testing(dataset&);

int main() {
	srand(100);
	
	// 	Define a dataset object and load the dataset
	dataset data;
	data.load_dataset();
	
	/** Step 1: Train a network with the complete and class-balanced "train" data,
	 *  test it on the "test" data and report the statistic about accuracy.
	**/
	cout << endl << "================ Step 1: Train a network with the complete dataset. ================" << endl;
	data.print_statistic(TRAIN_IMAGE);
	nn_training_testing(data);


	/** Step 2: Remove part of the "train" data for a digit, making the dataset class-imbalanced.
	 *  You then retrain the network, test again and report the accuracy.
	 *  Now the accuracy for that specific digit is expected to be lower than other digits.
	**/
	cout << endl << "================ Step 2: Train a network with the class-imbalanced dataset. ================" << endl;
	// const size_t imbalanced_digit = rand()% 10;
	const size_t imbalanced_digit = 4;	// To keep a fixed digit for cross-platfrom consistency
	const size_t size = 1600;
	data.remove_some_train_data(imbalanced_digit, size);
	data.print_statistic(TRAIN_IMAGE);
	nn_training_testing(data);

	
	/** Step 3: Generate more "train" data for the imbalanced digit by augmenting the original images.
	 *  Augmenting an image can be done by applying some geometrical transform without affecting 
	 * the fidelity of the label, such as image shifting, resizng and cropping.
	**/
	cout << endl << "================ Step 3: Train a network with the augmented dataset. ================" << endl;
	data.append_some_augmented_train_data(imbalanced_digit, size);
	data.print_statistic(TRAIN_IMAGE);
	nn_training_testing(data);

	return 0;
}