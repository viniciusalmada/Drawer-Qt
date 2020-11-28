#include "matrix.h"

Matrix::Matrix(int r, int c) : mNumRows(r), mNumColumns(c) {
	mData = std::vector<double>(r * c, 0.0);
}

Matrix::Matrix(int dim) : mNumRows(dim), mNumColumns(dim) {
	mData = std::vector<double>(dim * dim, 0.0);
}

Matrix::~Matrix() {
	mData.clear();
}

double& Matrix::operator[](std::pair<int, int> ij) {
	const int index = ij.first * mNumColumns + ij.second;
	return mData.at(index);
}

Matrix Matrix::operator*(Matrix& other) const {
	Matrix res{mNumRows, other.mNumColumns};
	for (int i = 0; i < res.mNumRows; ++i) {
		for (int j = 0; j < res.mNumColumns; ++j) {
			for (int k = 0; k < this->mNumColumns; ++k) {
				const double rowVal = mData[j * res.mNumColumns + k];
				const double colVal = other[{k, j}];
				const double prod = rowVal * colVal;
				res[{i, j}] += prod;
			}
		}
	}
	return res;
}
