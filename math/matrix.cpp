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

Matrix Matrix::operator*(const double& other) const {
	Matrix copy{*this};
	for (int i = 0; i < copy.mNumRows; ++i) {
		for (int j = 0; j < copy.mNumColumns; ++j) {
			copy[{i, j}] *= other;
		}
	}
	return copy;
}

Matrix Matrix::operator+(Matrix& other) const {
	Matrix copy{*this};
	for (int i = 0; i < copy.mNumRows; ++i) {
		for (int j = 0; j < copy.mNumColumns; ++j) {
			copy[{i, j}] += other[{i, j}];
		}
	}
	return copy;
}

Matrix Matrix::operator-(Matrix& other) const {
	Matrix copy{*this};
	for (int i = 0; i < copy.mNumRows; ++i) {
		for (int j = 0; j < copy.mNumColumns; ++j) {
			copy[{i, j}] -= other[{i, j}];
		}
	}
	return copy;
}

Matrix Matrix::operator-() const {
	Matrix copy{*this};
	for (int i = 0; i < copy.mNumRows; ++i) {
		for (int j = 0; j < copy.mNumColumns; ++j) {
			copy[{i, j}] = -copy[{i, j}];
		}
	}
	return copy;
}

Matrix Matrix::transpose() {
	Matrix t{mNumColumns, mNumRows};
	for (int i = 0; i < mNumRows; ++i) {
		for (int j = 0; j < mNumColumns; ++j) {
			t[{j, i}] = this->operator[]({i, j});
		}
	}
	return t;
}
