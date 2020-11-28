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

Matrix Matrix::setOneAtZerosOnDiagonal() {
	Matrix copy{*this};
	for (int i = 0; i < mNumRows; ++i) {
		for (int j = 0; j < mNumColumns; ++j) {
			if (i == j && mData[i * mNumColumns + j] == 0.0)
				copy[{i, j}] = 1.0;
		}
	}
	return copy;
}

int Matrix::rows() const {
	return mNumRows;
}

int Matrix::columns() const {
	return mNumColumns;
}

void Matrix::forEach(const Matrix::MatrixElem& block) {
	int r = 0;
	int c = 0;
	while (r < mNumRows) {
		while (c < mNumColumns) {
			block(r, c, this->operator[]({r, c}));
			c++;
		}
		c = 0;
		r++;
	}
	
}

Matrix Matrix::solveLinearSystem(Matrix& a, Matrix& b) {
	LUDecomposition lu(a);
	Matrix d(a.rows(), 1);
	d.forEach([&](int& r, int&, double) {
		double valD = b[{r, 0}] - sumLD(lu.l, d, r);
		d[{r, 0}] = valD;
	});
	
	Matrix x{a.rows(), 1};
	x.forEach([&](int& r, int&, double) {
		int i = x.rows() - r - 1;
		double valX = d[{i, 0}] / lu.l[{i, i}] - sumUX(lu.u, x, i);
		x[{i, 0}] = valX;
	});
	
	return x;
}

double Matrix::sumLD(Matrix l, Matrix d, int& i) {
	if (i < 1) return 0.0;
	
	double res = 0.0;
	for (int n = 0; n < i; ++n) {
		res += l[{i, n}] * d[{n, 0}];
	}
	return res;
}

double Matrix::sumUX(Matrix u, Matrix x, int i) {
	int j = x.rows();
	
	if (i >= j - 1) return 0.0;
	double res = 0.0;
	for (int n = i + 1; n < j; n++)
		res += u[{i, n}] * x[{n, 0}] / u[{i, i}];
	
	return res;
}

LUDecomposition::LUDecomposition(Matrix& matrix) :
		l(Matrix(matrix.rows(), matrix.columns())),
		u(Matrix(matrix.rows(), matrix.columns())) {
	
	l.setOneAtZerosOnDiagonal();
	
	matrix.forEach([&](int& i, int& j, double value) {
		if (i <= j) {
			const double valU = value - sumU(l, u, i, j);
			u[{i, j}] = valU;
		} else {
			const double valL = value / u[{j, j}] - sumL(l, u, i, j);
			l[{i, j}] = valL;
		}
	});
}

double LUDecomposition::sumL(Matrix& l, Matrix& u, int& r, int& c) {
	if (c < 1) return 0.0;
	
	double res = 0.0;
	
	for (int n = 0; n < c; n++)
		res += l[{r, n}] * u[{n, c}] / u[{c, c}];
	
	return res;
}

double LUDecomposition::sumU(Matrix& l, Matrix& u, int& r, int& c) {
	if (r < 1) return 0.0;
	
	double res = 0.0;
	
	for (int n = 0; n < c; n++)
		res += l[{r, n}] * u[{n, c}];
	
	return res;
}
