// mexExample.cpp : This file contains implementation of 1 iteration of seam removel and 
//image update. looping takes place in MATLAB, as opossed to mexWithLoop

#include "pch.h"
#include "mex.h"
#include "matrix.h"
#include <algorithm>
#include <string>
#include <vector>


static void calc_accumulative(const size_t len, const double *x, std::vector<double> &y, int rows, int cols)
{
	y[0] = x[0];
	y[cols - 1] = x[cols - 1];
	for (size_t j = 1; j < cols - 1; j++)
	{
		y[j] = x[j] + abs(x[j - 1] - x[j + 1]);
	}

	for (size_t i = 1; i < rows; i++)
	{
		y[i*cols + 0] = x[i*cols + 0] + std::min(y[(i - 1)*cols + 0], y[(i - 1)*cols + 1] + abs(x[i*cols + 1] - x[(i - 1)*cols ]));
		for (size_t j = 1; j < cols - 1; j++)
		{
			y[i*cols + j] = x[i*cols + j] + std::min({ y[(i - 1)*cols + j - 1] + abs(x[i*cols + j - 1] - x[i*cols + j + 1]) + abs(x[i*cols + j - 1] - x[(i - 1)*cols + j]), y[(i - 1)*cols + j] + abs(x[i*cols + j - 1] - x[i*cols + j + 1]), y[(i - 1)*cols + j + 1] + abs(x[i*cols + j - 1] - x[i*cols + j + 1]) + abs(x[i*cols + j + 1] - x[(i - 1)*cols + j]) });
		}
		y[i*cols + cols - 1] = x[i*cols + cols - 1] + std::min(y[(i - 1)*cols + cols - 1], y[(i - 1)*cols + cols - 2] + abs(x[i*cols + cols - 2] - x[(i - 1)*cols + cols - 1]));
	}
}



static double calc_seam(double *seam_vector, std::vector<double> &y, const double *x, int rows, int cols)
{
	double seam_sum = 0;
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

	seam_sum = min;
	seam_vector[rows - 1] = min_index + 1;//0

	for (size_t i = rows - 1; i > 0; i--)
	{
		if (min_index == 0)
		{
			if (y[(i - 1)*cols]  < y[(i - 1)*cols + 1] + abs(x[i*cols + 1] - x[(i - 1)*cols]))
			{
				seam_vector[i - 1] = min_index + 1;//0
			}
			else
			{
				seam_vector[i - 1] = min_index + 2;//+1
				min_index += 1;
			}
		}

		else if (min_index == cols - 1)
		{
			if (y[(i - 1)*cols + cols - 1]  < y[(i - 1)*cols + cols - 2] + abs(x[i*cols + cols - 2] - x[(i - 1)*cols + cols - 1]))
			{
				seam_vector[i - 1] = min_index + 1;//0
			}
			else
			{
				seam_vector[i - 1] = min_index; //-1
				min_index -= 1;
			}
		}
		else
		{
			if (std::min(y[(i - 1)*cols + min_index - 1] + abs(x[i*cols + min_index - 1] - x[i*cols + min_index + 1]) + abs(x[i*cols + min_index - 1] - x[(i - 1)*cols + min_index]), y[(i - 1)*cols + min_index] + abs(x[i*cols + min_index - 1] - x[i*cols + min_index + 1])) > y[(i - 1)*cols + min_index + 1] + abs(x[i*cols + min_index - 1] - x[i*cols + min_index + 1]) + abs(x[i*cols + min_index + 1] - x[(i - 1)*cols + min_index]))
			{
				seam_vector[i - 1] = min_index + 2;//+1
				min_index += 1;
			}
			else if (std::min(y[(i - 1)*cols + min_index - 1] + abs(x[i*cols + min_index - 1] - x[i*cols + min_index + 1]) + abs(x[i*cols + min_index - 1] - x[(i - 1)*cols + min_index]), y[(i - 1)*cols + min_index + 1] + abs(x[i*cols + min_index - 1] - x[i*cols + min_index + 1]) + abs(x[i*cols + min_index + 1] - x[(i - 1)*cols + min_index])) > y[(i - 1)*cols + min_index] + abs(x[i*cols + min_index - 1] - x[i*cols + min_index + 1]))
			{
				seam_vector[i - 1] = min_index + 1;//0
			}
			else
			{
				seam_vector[i - 1] = min_index;//-1
				min_index -= 1;
			}
		}
	}
	return seam_sum;
}
static void reduced_img_func(double *seam_vector, int rows, int cols, double* reduced, double* reduced_energy, const double* input_img, const double* energy, int number_elements, int numel_reduced)
{
	int number_elements_gray = number_elements / 3;
	int number_elements_gray_reduced = numel_reduced / 3;

	for (size_t j = cols; j > seam_vector[rows - 1] - 1; j--)
	{
		reduced[numel_reduced - 1] = input_img[number_elements - 1];
		reduced_energy[number_elements_gray_reduced - 1] = energy[number_elements_gray - 1];
		reduced[numel_reduced - 1 - cols * rows] = input_img[number_elements - 1 - (cols + 1)*rows];
		reduced[numel_reduced - 1 - 2 * cols*rows] = input_img[number_elements - 1 - 2 * (cols + 1)*rows];
		number_elements -= 1;
		numel_reduced -= 1;
		number_elements_gray -= 1;
		number_elements_gray_reduced -= 1;
	}
	number_elements -= 1;
	number_elements_gray -= 1;

	for (size_t i = rows - 1; i > 0; i--)
	{
		for (size_t j = seam_vector[i] + cols; j > seam_vector[i - 1]; j--)
		{
			reduced[numel_reduced - 1] = input_img[number_elements - 1];
			reduced_energy[number_elements_gray_reduced - 1] = energy[number_elements_gray - 1];
			reduced[numel_reduced - 1 - cols * rows] = input_img[number_elements - 1 - (cols + 1)*rows];
			reduced[numel_reduced - 1 - 2 * cols*rows] = input_img[number_elements - 1 - 2 * (cols + 1)*rows];
			number_elements -= 1;
			numel_reduced -= 1;
			number_elements_gray -= 1;
			number_elements_gray_reduced -= 1;
		}
		number_elements -= 1;
		number_elements_gray -= 1;
	}

	for (size_t j = 0; j < seam_vector[0] - 1; j++)
	{
		reduced[numel_reduced - 1] = input_img[number_elements - 1];
		reduced_energy[number_elements_gray_reduced - 1] = energy[number_elements_gray - 1];
		reduced[numel_reduced - 1 - cols * rows] = input_img[number_elements - 1 - (cols + 1)*rows];
		reduced[numel_reduced - 1 - 2 * cols*rows] = input_img[number_elements - 1 - 2 * (cols + 1)*rows];
		number_elements -= 1;
		numel_reduced -= 1;
		number_elements_gray -= 1;
		number_elements_gray_reduced -= 1;
	}
}


void mexFunction(int nlhs, mxArray *plhs[], int nrhs, const mxArray *prhs[])
{

	const mwSize ndims = mxGetNumberOfDimensions(prhs[0]);
	const mwSize *dims = mxGetDimensions(prhs[0]);
	const mwSize numel = mxGetNumberOfElements(prhs[0]);
	const double *input = mxGetPr(prhs[0]);

	const mwSize *dims_img = mxGetDimensions(prhs[1]);
	const mwSize numel_img = mxGetNumberOfElements(prhs[1]);
	const double *input_img = mxGetPr(prhs[1]);

	const mwSize rows = dims[1];
	const mwSize rows_reduced = dims_img[0] - 1;

	const mwSize cols_reduced = dims_img[1];
	const mwSize depth_reduced = dims_img[2];
	const mwSize size = 1;

	plhs[0] = mxCreateNumericArray(1, &size, mxDOUBLE_CLASS, mxREAL);
	plhs[1] = mxCreateNumericArray(1, &rows, mxDOUBLE_CLASS, mxREAL);


	mwSize reduced_img[3];
	mwSize red_energy[2];

	reduced_img[0] = rows_reduced;
	reduced_img[1] = cols_reduced;
	reduced_img[2] = depth_reduced;

	red_energy[0] = rows_reduced;
	red_energy[1] = cols_reduced;

	plhs[2] = mxCreateNumericArray(3, reduced_img, mxDOUBLE_CLASS, mxREAL);
	plhs[3] = mxCreateNumericArray(2, red_energy, mxDOUBLE_CLASS, mxREAL);

	double* seam_sum = mxGetPr(plhs[0]);
	double* seam_vector = mxGetPr(plhs[1]);
	double* reduced = mxGetPr(plhs[2]); //reduced img matrix
	double* reduced_energy = mxGetPr(plhs[3]); //reduced energy matrix
	std::vector<double> accumulative_mat(dims[0] * dims[1]);

	const mwSize numel_img_reduced = mxGetNumberOfElements(plhs[2]);

	calc_accumulative(numel, input, accumulative_mat, dims[1], dims[0]);
	*seam_sum = calc_seam(seam_vector, accumulative_mat, input, dims[1], dims[0]);
	reduced_img_func(seam_vector, cols_reduced, rows_reduced, reduced, reduced_energy, input_img, input, numel_img, numel_img_reduced);
}