#include <iostream>
#include <fstream>
#include <algorithm>
#include <unordered_set>
#include <set>
#include <stack>
#include <cstring>
#include <iomanip>
#include <stdexcept>
#include <random>

#include "dataset.h"
using namespace std;


const char* PATH_TRAIN_LABELS = "train-labels-idx1-ubyte";
const char* PATH_TRAIN_IMAGES = "train-images-idx3-ubyte";
const char* PATH_TEST_LABELS = "t10k-labels-idx1-ubyte";
const char* PATH_TEST_IMAGES = "t10k-images-idx3-ubyte";


dataset::dataset(){;}


dataset::~dataset(){;}


void dataset::load_dataset() {
	ifstream fin;
	uint32_t MN, N, A, B;

	/*---------------------*\
	\*---------------------*/

	fin = ifstream(PATH_TRAIN_LABELS, ios_base::binary);
	fin.read((char*)&MN, sizeof(MN));
	fin.read((char*)&N, sizeof(N));
	N = __builtin_bswap32(N)/3;
	
	if (MN != 0x01080000) {
		cerr << "Invalid database" << endl;
		exit(1);
	}

	train_labels.resize(N);
	fin.read((char*)train_labels.data(), N * sizeof(uint8_t));

	/*---------------------*\
	\*---------------------*/

	fin = ifstream(PATH_TRAIN_IMAGES, ios_base::binary);
	fin.read((char*)&MN, sizeof(MN));
	fin.read((char*)&N, sizeof(N));
	fin.read((char*)&A, sizeof(A));
	fin.read((char*)&B, sizeof(B));

	N = __builtin_bswap32(N)/3;
	A = __builtin_bswap32(A);
	B = __builtin_bswap32(B);

	if (MN != 0x03080000 || A != COLS || B != ROWS) {
		cerr << "invalid database" << endl;
		exit(1);
	}

	train_images.resize(N);
	fin.read((char*)train_images.data(), N * sizeof(array<uint8_t, PIXELS>));

	/*---------------------*\
	\*---------------------*/

	fin = ifstream(PATH_TEST_LABELS, ios_base::binary);
	fin.read((char*)&MN, sizeof(MN));
	fin.read((char*)&N, sizeof(N));
	N = __builtin_bswap32(N)/3;

	if (MN != 0x01080000) {
		cerr << "Invalid database" << endl;
		exit(1);
	}

	test_labels.resize(N);
	fin.read((char*)test_labels.data(), N * sizeof(uint8_t));

	/*---------------------*\
	\*---------------------*/

	fin = ifstream(PATH_TEST_IMAGES, ios_base::binary);
	fin.read((char*)&MN, sizeof(MN));
	fin.read((char*)&N, sizeof(N));
	fin.read((char*)&A, sizeof(A));
	fin.read((char*)&B, sizeof(B));

	N = __builtin_bswap32(N)/3;
	A = __builtin_bswap32(A);
	B = __builtin_bswap32(B);

	if (MN != 0x03080000 || A != COLS || B != ROWS) {
		cerr << "invalid database" << endl;
		exit(1);
	}

	test_images.resize(N);
	fin.read((char*)test_images.data(), N * sizeof(array<uint8_t, PIXELS>));
}


void dataset::remove_some_train_data(const uint8_t digit, const size_t size){
	// START OF YOUR IMPLEMENTATION

	stack<array<uint8_t, PIXELS>>  stack_images;
	stack<uint8_t>  stack_labels;
	unsigned int counter = 0;

	while(counter<size){	
		if(digit == train_labels.back()){
			++counter;
			train_images.pop_back();
			train_labels.pop_back();
		}	
		else{
			stack_images.push(train_images.back());
			stack_labels.push(train_labels.back());
			train_images.pop_back();
			train_labels.pop_back();
		}
	}
	while (!stack_labels.empty())
	{
		train_images.push_back(stack_images.top());
		train_labels.push_back(stack_labels.top());
		stack_images.pop();
		stack_labels.pop();
	}
	train_labels.shrink_to_fit();
	train_images.shrink_to_fit();

	// END OF YOUR IMPLEMENTATION
}


void dataset::append_some_augmented_train_data(const uint8_t digit, const size_t size){
    vector<size_t> indices;
	unsigned int counter = 0;

    for (vector<uint8_t>::iterator it = train_labels.begin(); it != train_labels.end(); it++) {
        if (*it == digit){
			indices.push_back(distance(train_labels.begin(), it));
		}
    }

	vector<size_t>::iterator it2 = indices.begin();
	while(counter<size){	
		++counter;

		train_images.push_back(this->compound_augment_image(train_images[*it2]));
		train_labels.push_back(digit);
		
		it2++;
		if(it2 == indices.end())
			it2 = indices.begin();
	}
}


array<uint8_t, PIXELS> dataset::compound_augment_image(array<uint8_t, PIXELS>& img_ar){
	img_dt img = convert_image_dt(img_ar);
	size_t rand_num = rand()% 8;
	this->shift_image(img, shift_type(rand_num), rand()% 4);

	rand_num = rand()% 9 + ROWS - 4;
	this->resize_image(img, rand_num, rand_num);
	if(rand_num>ROWS){
		this->crop_image(img, (rand_num-ROWS)/2, (rand_num-COLS)/2, ROWS, COLS);
	}
	else{
		this->padding_image(img, (ROWS-rand_num)/2, ROWS-(ROWS-rand_num)/2-rand_num, (COLS-rand_num)/2, COLS-(COLS-rand_num)/2-rand_num);
	}

	array<uint8_t, PIXELS> img_ar_out = convert_image_dt(img);

	return img_ar_out;
}


void dataset::shift_image(img_dt& img, const enum shift_type dir, const size_t p, const uint8_t value){
	// START OF YOUR IMPLEMENTATION

	img_dt img0(img);
	const size_t rows= img0.size();
	const size_t cols= img0[0].size();

	for (size_t y=0; y<rows; ++y)
		for (size_t x=0; x<cols; ++x){
			switch(dir){
				case LEFT:
					img[y][x] = x<cols-p ? img0[y][x+p] : value;	
					break;
				case RIGHT:
					img[y][x] = x>=p ? img0[y][x-p] : value;	
					break;
				case UP:
					img[y][x] = y<rows-p ? img0[y+p][x] : value;
					break;
				case DOWN:
					img[y][x] = y>=p ? img0[y-p][x] : value;
					break;
				case UPLEFT:
					img[y][x] = (x<cols-p && y<rows-p) ? img0[y+p][x+p] : value;	
					break;
				case UPRIGHT:
					img[y][x] = (x>=p && y<rows-p ) ? img0[y+p][x-p] : value;
					break;
				case DOWNLEFT:{
					img[y][x] = (x<cols-p && y>=p) ? img0[y-p][x+p] : value;
				}	break;
				case DOWNRIGHT:
					img[y][x] = (x>=p && y>=p) ? img0[y-p][x-p] : value;
					break;
				default:
					throw invalid_argument( "Invalid shift_type!");
		}
	}

	// END OF YOUR IMPLEMENTATION
}


void dataset::resize_image(img_dt& img, const size_t new_rows, const size_t new_cols){
	// START OF YOUR IMPLEMENTATION

	img_dt img0(img);

	const float ratio_row = float(img0.size())/new_rows;
	const float ratio_col = float(img0[0].size())/new_cols;
	size_t iy;	// interpolated_y
	size_t ix;	// interpolated_x

	// Resize the vector to `new_rows` elements of type std::vector<int>, each having size `new_cols`.
    img.resize(new_rows, vector<uint8_t>(new_cols));
	for(size_t i=0; i<img.size();i++)
    	(img.at(i)).resize(new_cols);

	for (size_t y=0; y<new_rows; ++y) {
    	for (size_t x=0; x<new_cols; ++x) {
			iy = floor(float(ratio_row*(y+0.5)));
			ix = floor(float(ratio_col*(x+0.5)));
			img[y][x] = img0[iy][ix];
    	}
  	}
	// END OF YOUR IMPLEMENTATION
}


void dataset::crop_image(img_dt& img, const size_t y0, const size_t x0, const size_t new_rows, const size_t new_cols){
	// START OF YOUR IMPLEMENTATION

	img_dt img0(img);
	
	// Resize the vector to `y1`-'y0' elements of type std::vector<int>, each having size `x1`-'x0'.
    img.resize(new_rows, vector<uint8_t>(new_cols));
	for(size_t i=0; i<img.size();i++)
    	(img.at(i)).resize(new_cols);

	for (size_t y=0; y<new_rows; ++y) {
    	for (size_t x=0; x<new_cols; ++x) {
			img[y][x] = img0[y+y0][x+x0];
    	}
  	}

	// END OF YOUR IMPLEMENTATION
}


void dataset::padding_image(img_dt& img, const size_t top, const size_t down, const size_t left, const size_t right, const uint8_t value){
	// START OF YOUR IMPLEMENTATION

	img_dt img0(img);
	const size_t new_rows=img0.size()+top+down;
	const size_t new_cols=img0[0].size()+left+right;

    img.resize(new_rows, vector<uint8_t>(new_cols));
	for(size_t i=0; i<img.size();i++)
    	(img.at(i)).resize(new_cols);

	for (size_t y=0; y<new_rows; ++y) {
    	for (size_t x=0; x<new_cols; ++x) {
			if(y<top || x<left || y>=(new_rows-down) || x>=(new_cols-right))
				img[y][x] = value;
			else
				img[y][x] = img0[y-top][x-left];
    	}
  	}

	// END OF YOUR IMPLEMENTATION
}


uint8_t	dataset::get_a_label(const data_type mode, const size_t index) const{
	switch(mode){
		case TRAIN_LABEL: 
			return train_labels[index];		break;
		case TEST_LABEL: 
			return test_labels[index];		break;
		default:
			throw invalid_argument( "Invalid mode!");
	}
}


img_dt dataset::convert_image_dt(const array<uint8_t, PIXELS>& img) const {
	// START OF YOUR IMPLEMENTATION

	img_dt out(ROWS, vector<uint8_t> (COLS, 0));
	for (size_t y=0; y<ROWS; ++y)
    	for (size_t x=0; x<COLS; ++x)
			out[y][x] = img[y*COLS+x];
	
	return out;

	// END OF YOUR IMPLEMENTATION
}


array<uint8_t, PIXELS>  dataset::convert_image_dt(const img_dt& img) const {
	// START OF YOUR IMPLEMENTATION

	array<uint8_t, PIXELS> out;
	for (size_t y=0; y<ROWS; ++y)
    	for (size_t x=0; x<COLS; ++x)
			out[y*COLS+x] = img[y][x];
	
	return out;

	// END OF YOUR IMPLEMENTATION
}


array<uint8_t, PIXELS>	dataset::get_an_image(const enum data_type mode, const size_t index) const{
	switch(mode){
		case TRAIN_IMAGE:
			return train_images[index];		break;
		case TEST_IMAGE: 
			return test_images[index];		break;
		default:
			throw invalid_argument( "Invalid mode!");
	}
}


vector<float>	dataset::get_a_normalized_image(const enum data_type mode, const size_t index) const{
	const array<uint8_t, PIXELS>& img = this->get_an_image(mode, index);
	vector<float> img_normalized(PIXELS);

	transform(img.begin(), img.end(), img_normalized.begin(), normalize(255.0f));
	return img_normalized;
}


size_t	dataset::get_data_size(const enum data_type mode) const{
	switch(mode){
		case TRAIN_LABEL: 
			return train_labels.size();		break;
		case TEST_LABEL:
			return test_labels.size();		break;
		case TRAIN_IMAGE: 
			return train_images.size();		break;
		case TEST_IMAGE: 
			return test_images.size();		break;
		default:
			throw invalid_argument( "Invalid mode!");
	}
}


void dataset::print_image(const array<uint8_t, PIXELS>& img_ar) const {
	// START OF YOUR IMPLEMENTATION

	for (size_t y=0; y<ROWS; ++y) {
    	for (size_t x=0; x<COLS; ++x) {
			cout << setw(4) << unsigned(img_ar[y*COLS+x]);
    }
    	cout << endl;
  	}
	cout << endl;

	// END OF YOUR IMPLEMENTATION
}


void dataset::print_image(const img_dt& img) const {
	// START OF YOUR IMPLEMENTATION

	img_dt::const_iterator it1;	// Iterator for the 2-D vector
	vector<uint8_t>::const_iterator it2;	// Iterator for each vector inside the 2-D vector

	// Traversing a 2-D vector using iterators
	for(it1 = img.begin();it1 != img.end();it1++){
		for(it2 = it1->begin();it2 != it1->end();it2++){
			cout << setw(4) << unsigned(*it2);
		}
		cout << endl;
	}
	cout << endl;

	// END OF YOUR IMPLEMENTATION
}


void dataset::print_statistic(const enum data_type mode) const{
	// START OF YOUR IMPLEMENTATION

	switch(mode){
		case TRAIN_IMAGE:{
			unordered_multiset<uint8_t> train_labels_multiset (train_labels.begin(), train_labels.end());
			set<uint8_t> train_labels_set (train_labels.begin(), train_labels.end());
			for (const uint8_t& l: train_labels_set) {
				std::cout << "Number of train images for digit " << unsigned(l) << ": " << train_labels_multiset.count(l) << endl;
			}
		}
			break;
		case TEST_IMAGE:{
			unordered_multiset<uint8_t> test_labels_multiset (test_labels.begin(), test_labels.end());
			set<uint8_t> test_labels_set (test_labels.begin(), test_labels.end());
			for (const uint8_t& l: test_labels_set) {
				std::cout << "Number of test images for digit " << unsigned(l) << ": " << test_labels_multiset.count(l) << endl;
			}
		} 
			break;
		default:
			throw invalid_argument( "Invalid mode!");
	}

	// END OF YOUR IMPLEMENTATION
}