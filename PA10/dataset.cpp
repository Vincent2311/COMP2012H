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

const char *PATH_TRAIN_LABELS = "train-labels-idx1-ubyte";
const char *PATH_TRAIN_IMAGES = "train-images-idx3-ubyte";
const char *PATH_TEST_LABELS = "t10k-labels-idx1-ubyte";
const char *PATH_TEST_IMAGES = "t10k-images-idx3-ubyte";

dataset::dataset() { ; }

dataset::~dataset() { ; }

void dataset::load_dataset()
{
	ifstream fin;
	uint32_t MN, N, A, B;

	/*---------------------*\
	\*---------------------*/

	fin = ifstream(PATH_TRAIN_LABELS, ios_base::binary);
	fin.read((char *)&MN, sizeof(MN));
	fin.read((char *)&N, sizeof(N));
	N = __builtin_bswap32(N) / 3;

	if (MN != 0x01080000)
	{
		cerr << "Invalid database" << endl;
		exit(1);
	}

	train_labels.resize(N);
	fin.read((char *)train_labels.data(), N * sizeof(uint8_t));

	/*---------------------*\
	\*---------------------*/

	fin = ifstream(PATH_TRAIN_IMAGES, ios_base::binary);
	fin.read((char *)&MN, sizeof(MN));
	fin.read((char *)&N, sizeof(N));
	fin.read((char *)&A, sizeof(A));
	fin.read((char *)&B, sizeof(B));

	N = __builtin_bswap32(N) / 3;
	A = __builtin_bswap32(A);
	B = __builtin_bswap32(B);

	if (MN != 0x03080000 || A != COLS || B != ROWS)
	{
		cerr << "invalid database" << endl;
		exit(1);
	}

	train_images.resize(N);
	fin.read((char *)train_images.data(), N * sizeof(array<uint8_t, PIXELS>));

	/*---------------------*\
	\*---------------------*/

	fin = ifstream(PATH_TEST_LABELS, ios_base::binary);
	fin.read((char *)&MN, sizeof(MN));
	fin.read((char *)&N, sizeof(N));
	N = __builtin_bswap32(N) / 3;

	if (MN != 0x01080000)
	{
		cerr << "Invalid database" << endl;
		exit(1);
	}

	test_labels.resize(N);
	fin.read((char *)test_labels.data(), N * sizeof(uint8_t));

	/*---------------------*\
	\*---------------------*/

	fin = ifstream(PATH_TEST_IMAGES, ios_base::binary);
	fin.read((char *)&MN, sizeof(MN));
	fin.read((char *)&N, sizeof(N));
	fin.read((char *)&A, sizeof(A));
	fin.read((char *)&B, sizeof(B));

	N = __builtin_bswap32(N) / 3;
	A = __builtin_bswap32(A);
	B = __builtin_bswap32(B);

	if (MN != 0x03080000 || A != COLS || B != ROWS)
	{
		cerr << "invalid database" << endl;
		exit(1);
	}

	test_images.resize(N);
	fin.read((char *)test_images.data(), N * sizeof(array<uint8_t, PIXELS>));
}

void dataset::remove_some_train_data(const uint8_t digit, const size_t size)
{
	// START OF YOUR IMPLEMENTATION
	size_t index = 0;
	size_t count_index = 0;
	size_t label_count = count(train_labels.begin(), train_labels.end(), digit);
	vector<array<uint8_t, PIXELS>>::iterator img_new_end = remove_if(train_images.begin(), train_images.end(), [&](array<uint8_t, PIXELS>)
																	 {
		if(get_a_label(TRAIN_LABEL,count_index) == digit) {
			++index;
			count_index++;
			return index > label_count - size;
		}
		count_index++;
		return false; });
	train_images.erase(img_new_end, train_images.end());

	index = 0;
	vector<uint8_t>::iterator lable_new_end = remove_if(train_labels.begin(), train_labels.end(), [&](uint8_t i)
														{
		if(i == digit) {
			++index;
		}
		return index > label_count - size && i == digit; });
	train_labels.erase(lable_new_end, train_labels.end());

	// END OF YOUR IMPLEMENTATION
}

void dataset::append_some_augmented_train_data(const uint8_t digit, const size_t size)
{
	vector<size_t> indices;
	unsigned int counter = 0;

	for (vector<uint8_t>::iterator it = train_labels.begin(); it != train_labels.end(); it++)
	{
		if (*it == digit)
		{
			indices.push_back(distance(train_labels.begin(), it));
		}
	}

	vector<size_t>::iterator it2 = indices.begin();
	while (counter < size)
	{
		++counter;

		train_images.push_back(this->compound_augment_image(train_images[*it2]));
		train_labels.push_back(digit);

		it2++;
		if (it2 == indices.end())
			it2 = indices.begin();
	}
}

array<uint8_t, PIXELS> dataset::compound_augment_image(array<uint8_t, PIXELS> &img_ar)
{
	img_dt img = convert_image_dt(img_ar);
	size_t rand_num = rand() % 8;
	this->shift_image(img, shift_type(rand_num), rand() % 4);

	rand_num = rand() % 9 + ROWS - 4;
	this->resize_image(img, rand_num, rand_num);
	if (rand_num > ROWS)
	{
		this->crop_image(img, (rand_num - ROWS) / 2, (rand_num - COLS) / 2, ROWS, COLS);
	}
	else
	{
		this->padding_image(img, (ROWS - rand_num) / 2, ROWS - (ROWS - rand_num) / 2 - rand_num, (COLS - rand_num) / 2, COLS - (COLS - rand_num) / 2 - rand_num);
	}

	array<uint8_t, PIXELS> img_ar_out = convert_image_dt(img);

	return img_ar_out;
}

void shift_left(img_dt &img, const size_t p, const uint8_t value)
{
	if (p >= img[0].size())
	{
		for (size_t i = 0; i < img.size(); i++)
			fill(img[i].begin(), img[i].end(), value);
	}
	else
	{
		for (size_t i = 0; i < img.size(); i++)
		{
			rotate(img[i].begin(), img[i].begin() + p, img[i].end());
			for (size_t j = img[0].size() - p; j < img[0].size(); j++)
			{
				img[i][j] = value;
			}
		}
	}
}

void shift_right(img_dt &img, const size_t p, const uint8_t value)
{
	if (p >= img[0].size())
	{
		for (size_t i = 0; i < img.size(); i++)
			fill(img[i].begin(), img[i].end(), value);
		return;
	}
	else
	{
		for (size_t i = 0; i < img.size(); i++)
		{
			rotate(img[i].begin(), img[i].begin() + (img[0].size() - p), img[i].end());
			for (size_t j = 0; j < p; j++)
			{
				img[i][j] = value;
			}
		}
	}
}

void shift_up(img_dt &img, const size_t p, const uint8_t value)
{
	if (p >= img.size())
	{
		for (size_t i = 0; i < img.size(); i++)
			fill(img[i].begin(), img[i].end(), value);
		return;
	}
	else
	{
		size_t rows = img.size();
		deque<uint8_t> temp;
		for (size_t i = 0; i < img[0].size(); i++)
		{
			temp.clear();
			size_t j;
			for (j = p; j < img.size(); j++)
			{
				temp.push_back(img[j][i]);
			}
			for (j = 0; j < temp.size(); j++)
			{
				img[j][i] = temp[j];
			}
			for (; j < img.size(); j++)
			{
				img[j][i] = value;
			}
		}
	}
}

void shift_down(img_dt &img, const size_t p, const uint8_t value)
{
	if (p >= img.size())
	{
		for (size_t i = 0; i < img.size(); i++)
			fill(img[i].begin(), img[i].end(), value);
		return;
	}
	else
	{
		size_t rows = img.size();
		deque<uint8_t> temp;
		for (size_t i = 0; i < img[0].size(); i++)
		{
			temp.clear();
			size_t j;
			for (j = 0; j < img.size() - p; j++)
			{
				temp.push_back(img[j][i]);
			}
			for (j = p; j < temp.size() + p; j++)
			{
				img[j][i] = temp[j - p];
			}
			for (; j < p; j++)
			{
				img[j][i] = value;
			}
		}
	}
}

void dataset::shift_image(img_dt &img, const enum shift_type dir, const size_t p, const uint8_t value)
{
	// START OF YOUR IMPLEMENTATION
	switch (dir)
	{
	case LEFT:
		shift_left(img, p, value);
		break;

	case RIGHT:
		shift_right(img, p, value);
		break;

	case UP:
		shift_up(img, p, value);
		break;

	case DOWN:
		shift_down(img, p, value);
		break;
	case UPLEFT:
		shift_up(img, p, value);
		shift_left(img, p, value);
		break;
	case UPRIGHT:
		shift_up(img, p, value);
		shift_right(img, p, value);
		break;
	case DOWNLEFT:
		shift_down(img, p, value);
		shift_left(img, p, value);
		break;
	case DOWNRIGHT:
		shift_down(img, p, value);
		shift_right(img, p, value);
		break;

	default:
		break;
	}
	// END OF YOUR IMPLEMENTATION
}

void dataset::resize_image(img_dt &img, const size_t new_rows, const size_t new_cols)
{
	//  START OF YOUR IMPLEMENTATION
	vector<vector<pair<double, double>>> old_distance;
	vector<vector<pair<double, double>>> new_distance;
	img_dt new_image;
	deque<uint8_t> new_row;
	vector<pair<double, double>> row;
	uint8_t assigned_value;
	double ratio_x = 1.0*img[0].size() /new_cols;
	double ratio_y = 1.0*img.size()/ new_rows;
	for (size_t i = 0; i < img.size(); i++)
	{
		row.clear();
		for (size_t j = 0; j < img[0].size(); j++)
		{
			row.push_back(make_pair((j + 0.5), (i + 0.5)));
		}
		old_distance.push_back(row);
	}

	for (size_t i = 0; i < new_rows; i++)
	{
		row.clear();
		for (size_t j = 0; j < new_cols; j++)
		{
			row.push_back(make_pair((j + 0.5) * ratio_x, (i + 0.5) * ratio_y));
		}
		new_distance.push_back(row);
	}

	double dist;
	for (size_t i = 0; i < new_rows; i++)
	{
		new_row.clear();
		for (size_t j = 0; j < new_cols; j++)
		{
			double x0 = new_distance[i][j].first;
			double y0 = new_distance[i][j].second;
			double min_dist = INFINITY;
			for (size_t cur_row = 0; cur_row < img.size(); cur_row++)
			{
				for (size_t cur_col = 0; cur_col < img[0].size(); cur_col++)
				{
					double x = old_distance[cur_row][cur_col].first;
					double y = old_distance[cur_row][cur_col].second;
					dist = sqrt((x0 - x) * (x0 - x) + (y0 - y) * (y0 - y));
					if (dist <= min_dist + 0.0000001f)
					{
						min_dist = dist;
						assigned_value = img[cur_row][cur_col];
					}
				}
			}
			new_row.push_back(assigned_value);
		}
		new_image.push_back(new_row);
	}
	img = new_image;

    // END OF YOUR IMPLEMENTATION
}

void dataset::crop_image(img_dt &img, const size_t y0, const size_t x0, const size_t new_rows, const size_t new_cols)
{
	// START OF YOUR IMPLEMENTATION
	img_dt new_img;
	deque<uint8_t> deq;
	for (size_t i = 0; i < new_rows; i++)
	{
		deq.clear();
		for (size_t j = 0; j < new_cols; j++)
		{
			deq.push_back(img[y0 + i][x0 + j]);
		}
		new_img.push_back(deq);
	}
	img = new_img;
	// END OF YOUR IMPLEMENTATION
}

void dataset::padding_image(img_dt &img, const size_t top, const size_t down, const size_t left, const size_t right, const uint8_t value)
{
	// START OF YOUR IMPLEMENTATION
	size_t new_width = img[0].size() + left + right;
	size_t old_height = img.size();
	for (size_t i = 0; i < top; i++)
	{
		img.push_front(deque<uint8_t>(new_width, value));
	}
	for (size_t i = 0; i < old_height; i++)
	{
		img[top + i].insert(img[top + i].begin(), left, value);
		img[top + i].insert(img[top + i].end(), right, value);
	}
	for (size_t i = 0; i < down; i++)
	{
		img.push_back(deque<uint8_t>(new_width, value));
	}
	// END OF YOUR IMPLEMENTATION
}

uint8_t dataset::get_a_label(const data_type mode, const size_t index) const
{
	switch (mode)
	{
	case TRAIN_LABEL:
		return train_labels[index];
		break;
	case TEST_LABEL:
		return test_labels[index];
		break;
	default:
		throw invalid_argument("Invalid mode!");
	}
}

img_dt dataset::convert_image_dt(const array<uint8_t, PIXELS> &img) const
{
	// START OF YOUR IMPLEMENTATION
	img_dt output;
	deque<uint8_t> deq;
	for (size_t i = 0; i < ROWS; i++)
	{
		deq.clear();
		for (size_t j = 0; j < COLS; j++)
		{
			deq.push_back(img[i * ROWS + j]);
		}
		output.push_back(deq);
	}
	return output;
	// END OF YOUR IMPLEMENTATION
}

array<uint8_t, PIXELS> dataset::convert_image_dt(const img_dt &img) const
{
	// START OF YOUR IMPLEMENTATION
	array<uint8_t, PIXELS> output;
	for (size_t i = 0; i < img.size(); i++)
	{
		for (size_t j = 0; j < img[0].size(); j++)
		{
			output[i * ROWS + j] = img[i][j];
		}
	}
	return output;
	// END OF YOUR IMPLEMENTATION
}

array<uint8_t, PIXELS> dataset::get_an_image(const enum data_type mode, const size_t index) const
{
	switch (mode)
	{
	case TRAIN_IMAGE:
		return train_images[index];
		break;
	case TEST_IMAGE:
		return test_images[index];
		break;
	default:
		throw invalid_argument("Invalid mode!");
	}
}

vector<float> dataset::get_a_normalized_image(const enum data_type mode, const size_t index) const
{
	const array<uint8_t, PIXELS> &img = this->get_an_image(mode, index);
	vector<float> img_normalized(PIXELS);

	transform(img.begin(), img.end(), img_normalized.begin(), normalize(255.0f));
	return img_normalized;
}

size_t dataset::get_data_size(const enum data_type mode) const
{
	switch (mode)
	{
	case TRAIN_LABEL:
		return train_labels.size();
		break;
	case TEST_LABEL:
		return test_labels.size();
		break;
	case TRAIN_IMAGE:
		return train_images.size();
		break;
	case TEST_IMAGE:
		return test_images.size();
		break;
	default:
		throw invalid_argument("Invalid mode!");
	}
}

void dataset::print_image(const array<uint8_t, PIXELS> &img_ar) const
{
	// START OF YOUR IMPLEMENTATION
	for (size_t i = 0; i < ROWS; i++)
	{
		size_t j = 0;
		for (; j < COLS - 1; j++)
		{
			printf("%4u", img_ar[i * ROWS + j]);
		}
		printf("%4u\n", img_ar[i * ROWS + j]);
	}
	fflush(stdout);
	// END OF YOUR IMPLEMENTATION
}

void dataset::print_image(const img_dt &img) const
{
	// START OF YOUR IMPLEMENTATION
	for (size_t i = 0; i < img.size(); i++)
	{
		size_t j = 0;
		for (; j < img[0].size() - 1; j++)
		{
			printf("%4u", img[i][j]);
		}
		printf("%4u\n", img[i][j]);
	}
	fflush(stdout);
	// END OF YOUR IMPLEMENTATION
}

void dataset::print_statistic(const enum data_type mode) const
{
	// START OF YOUR IMPLEMENTATION
	switch (mode)
	{
	case TRAIN_IMAGE:
		for (int i = 0; i < 10; i++)
		{
			cout << "Number of train images for digit " << i << ": " << count(train_labels.begin(), train_labels.end(), i) << endl;
		}
		break;

	case TEST_IMAGE:
		for (int i = 0; i < 10; i++)
		{
			cout << "Number of test images for digit " << i << ": " << count(test_labels.begin(), test_labels.end(), i) << endl;
		}
		break;
	}
	// END OF YOUR IMPLEMENTATION
}
