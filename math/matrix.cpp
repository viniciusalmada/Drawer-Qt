#include <cmath>
#include <stdexcept>
#include "matrix.h"

Matrix::Matrix(int r, int c) : mNumRows(r), mNumColumns(c) {
	mData = std::vector<double>(r * c, 0.0);
}

Matrix::Matrix(int dim) : mNumRows(dim), mNumColumns(dim) {
	mData = std::vector<double>(dim * dim, 0.0);
}

Matrix::Matrix(int r, int c, std::vector<double> data) :
		mNumRows(r), mNumColumns(c), mData(data) {
	
}

Matrix::~Matrix() {
	mData.clear();
}

double& Matrix::operator[](std::pair<int, int> ij) {
	const int index = ij.first * mNumColumns + ij.second;
	return mData.at(index);
}

Matrix Matrix::operator*(Matrix& other) {
	Matrix res{mNumRows, other.mNumColumns};
	for (int r = 0; r < res.mNumRows; ++r) {
		for (int c = 0; c < res.mNumColumns; ++c) {
			for (int n = 0; n < this->mNumColumns; ++n) {
				const double rowVal = this->operator[]({r, n});
				const double colVal = other[{n, c}];
				const double prod = rowVal * colVal;
				res[{r, c}] += prod;
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

Matrix Matrix::operator+(Matrix other) const {
	Matrix copy{*this};
	for (int i = 0; i < copy.mNumRows; ++i) {
		for (int j = 0; j < copy.mNumColumns; ++j) {
			copy[{i, j}] += other[{i, j}];
		}
	}
	return copy;
}

void Matrix::operator+=(Matrix other) {
	for (int i = 0; i < mNumRows; ++i) {
		for (int j = 0; j < mNumColumns; ++j) {
			this->operator[]({i, j}) += other[{i, j}];
		}
	}
}

void Matrix::operator-=(Matrix other) {
	for (int i = 0; i < mNumRows; ++i) {
		for (int j = 0; j < mNumColumns; ++j) {
			this->operator[]({i, j}) -= other[{i, j}];
		}
	}
}

Matrix Matrix::operator-(Matrix other) const {
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

double Matrix::determinant() {
	Matrix u = LUDecomposition(*this).u;
	double res = 1.0;
	u.forEach([&](int& i, int& j, double val) {
		if (i == j)
			res *= val;
	});
	return res;
}

Matrix Matrix::inverse() {
	Matrix invMat = Matrix(rows(), columns());
	for (int i = 0; i < rows(); i++) {
		Matrix b = Matrix(rows(), 1);
		b[{i, 0}] = 1.0;
		Matrix x = solveLinearSystem(*this, b);
		invMat.forEach([&](int& m, int& n, double val) {
			if (n == i)
				invMat[{m, n}] = x[{m, 0}];
		});
	}
	return invMat;
}

Matrix Matrix::solveLinearSystemCGS(Matrix& a, Matrix& b, const int maxIt, const double tol) {
	double residue;
	double rho1;
	double rho2;
	double alpha;
	double beta;
	
	Matrix x = b;
	
	double normB = b.norm();
	Matrix r = b - a * x;
	Matrix rTilde = r;
	
	if (normB == 0.0)
		normB = 1.0;
	
	residue = r.norm() / normB;
	if (residue <= tol) {
		throw std::runtime_error{"Initial residue is less than tolerance"};
	}
	
	Matrix u{r};
	Matrix p{u};
	Matrix q{u};
	Matrix pHat{p};
	Matrix vHat{p};
	Matrix uHat{p};
	Matrix qHat{p};
	for (int i = 1; i <= maxIt; ++i) {
		rho1 = rTilde.dot(r);
		if (rho1 == 0.0) {
			throw std::runtime_error{"Variable rho1 is equals to zero"};
		}
		if (i == 1) {
			u = r;
			p = u;
		} else {
			beta = rho1 / rho2;
			u = r + q * beta;
			p = u + (q + p * beta) * beta;
		}
		pHat = p;
		vHat = a * pHat;
		
		alpha = rho1 / rTilde.dot(vHat);
		
		q = u - vHat * alpha;
		
		uHat = u + q;
		
		x += uHat * alpha;
		
		qHat = a * uHat;
		
		r -= qHat * alpha;
		
		rho2 = rho1;
		
		residue = r.norm() / normB;
		if (residue < tol) {
			return x;
		}
	}
	
	throw std::runtime_error{"Could not converge to solution"};
}

double Matrix::norm() {
	double res = 0.0;
	forEach([&](int& r, int& c, double val) {
		res += val * val;
	});
	return sqrt(res);
}

double Matrix::dot(Matrix& matrix) {
	double res = 0.0;
	forEach([&](int& r, int& c, double val) {
		res += val * matrix[{r, c}];
	});
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
