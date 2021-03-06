// mexExample.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "pch.h"
#include "mex.h"
#include "matrix.h"
#include <algorithm>
#include <vector>

int g_seam_number = 1;

class MultiImage
{
public:
	 int mOriginalPos; //will hold original position in vector
	 double mValue; //will hold the value of the pixel
	 MultiImage(int pos, double val)
	 {
		 mOriginalPos = pos;
		 mValue = val;
	 }
	 MultiImage() {}
};

static void calc_accumulative(std::vector<MultiImage> &x, std::vector<double> &y, int rows, int cols)
{
	y.resize(rows*cols);
	for (size_t j = 0; j < cols; j++)
	{
		y[j] = (x[j]).mValue;
	}
	
	for (size_t i = 1; i < rows; i++)
	{
		y[i*cols + 0] = (x[i*cols + 0]).mValue + std::min(y[(i - 1)*cols + 0], y[(i - 1)*cols + 1]);
		for (size_t j = 1; j < cols - 1; j++)
		{
			y[i*cols + j] = (x[i*cols + j]).mValue + std::min({ y[(i - 1)*cols + j - 1], y[(i - 1)*cols + j], y[(i - 1)*cols + j + 1] });
		}
		y[i*cols + cols - 1] = (x[i*cols + cols - 1]).mValue + std::min(y[(i - 1)*cols + cols - 2], y[(i - 1)*cols + cols - 1]);
	}
}

static void calc_seam(std::vector<int> &seam_vector, std::vector<double> const &y, int rows, int cols)
{
	double min = y[(rows - 1)*cols];
	int min_index = 0;

	for (size_t j = 1; j < cols; j++)
	{
		if (y[(rows - 1)*cols + j] < min)
		{
			min = y[(rows - 1)*cols + j];
			min_index = j;
		}
	}

	seam_vector[rows - 1] = min_index;//0

	for (size_t i = rows - 1; i > 0; i--)
	{
		if (min_index == 0)
		{
			if (y[(i - 1)*cols] < y[(i - 1)*cols + 1])
			{
				seam_vector[i - 1] = min_index;//0
			}
			else
			{
				seam_vector[i - 1] = min_index + 1;//+1
				min_index += 1;
			}
		}

		else if (min_index == cols - 1)
		{
			if (y[(i - 1)*cols + cols - 1] < y[(i - 1)*cols + cols - 2])
			{
				seam_vector[i - 1] = min_index;//0
			}
			else
			{
				seam_vector[i - 1] = min_index - 1; //-1
				min_index -= 1;
			}
		}
		else
		{
			if (std::min(y[(i - 1)*cols + min_index - 1], y[(i - 1)*cols + min_index]) > y[(i - 1)*cols + min_index + 1])
			{
				seam_vector[i - 1] = min_index + 1;//+1
				min_index += 1;
			}
			else if (std::min(y[(i - 1)*cols + min_index - 1], y[(i - 1)*cols + min_index + 1]) > y[(i - 1)*cols + min_index])
			{
				seam_vector[i - 1] = min_index;//0
			}
			else
			{
				seam_vector[i - 1] = min_index - 1;//-1
				min_index -= 1;
			}
		}

	}
}

static void reduced_energy_func(double *seam_removal_order, std::vector<int> const &seam_vector, int rows, int cols, int original_cols ,std::vector<MultiImage> &reduced_energy, std::vector<MultiImage> const &output_energy, int number_elements_gray, int number_elements_gray_reduced)
{
	reduced_energy.resize(number_elements_gray_reduced);

	for (int i = 0; i < seam_vector.size(); i++)
	{
		seam_removal_order[output_energy[(i * (cols + 1)) + seam_vector[i]].mOriginalPos] = g_seam_number;
	}

	for (size_t j = cols; j > seam_vector[rows - 1]; j--)
	{
		reduced_energy[--number_elements_gray_reduced] = output_energy[--number_elements_gray];
	}
	number_elements_gray--;

	for (size_t i = rows - 1; i > 0; i--)
	{
		for (size_t j = seam_vector[i] + cols; j > seam_vector[i - 1]; j--)
		{
			reduced_energy[--number_elements_gray_reduced] = output_energy[--number_elements_gray];
		}
		number_elements_gray--;
	}

	for (size_t j = 0; j < seam_vector[0]; j++)
	{
		reduced_energy[--number_elements_gray_reduced] = output_energy[--number_elements_gray];
	}

	g_seam_number++;
}

void mexFunction(int nlhs, mxArray *plhs[], int nrhs, const mxArray *prhs[])
{

	const mwSize *dims = mxGetDimensions(prhs[0]);
	mwSize numel = mxGetNumberOfElements(prhs[0]);
	const double *input_energy = mxGetPr(prhs[0]);
	const int size_to_reduce = int(*mxGetPr(prhs[1]));
	
	mwSize rows = dims[1];
	mwSize cols = dims[0];
	const int original_cols = cols;
	mwSize numel_reduced = (cols - 1) * rows;

	std::vector<MultiImage> temp_energy;
	for (int i = 0; i < numel; i++)
		temp_energy.push_back(MultiImage(i, input_energy[i]));
	std::vector<MultiImage> temp_energy_reduced;

	plhs[0] = mxCreateNumericArray(2, dims, mxDOUBLE_CLASS, mxREAL);
	double* seam_removal_order = mxGetPr(plhs[0]);
	
	std::vector<double> output_accumulative;
	std::vector<int> seam_vector(rows);

	//for (int i = 0; i < 50; i++)
	for (int i = 0; i < size_to_reduce; i++)
	{
		calc_accumulative(temp_energy, output_accumulative, rows, cols);
		calc_seam(seam_vector, output_accumulative, rows, cols);
		reduced_energy_func(seam_removal_order, seam_vector, rows, cols - 1, original_cols, temp_energy_reduced, temp_energy, numel, numel_reduced);

		cols--;
		numel = cols * rows;
		numel_reduced = (cols - 1) * rows;
		temp_energy = temp_energy_reduced;
	}
	g_seam_number = 1;
}