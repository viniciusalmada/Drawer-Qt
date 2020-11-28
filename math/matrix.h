#ifndef DRAWER_MATRIX_H
#define DRAWER_MATRIX_H

#include <vector>

class Matrix {
	std::vector<double> mData;
	int mNumRows;
	int mNumColumns;

public:
	Matrix(int r, int c);
	
	explicit Matrix(int dim);
	
	~Matrix();
	
	double& operator[](std::pair<int, int> i);
	
	Matrix operator*(Matrix& other) const;
	
	Matrix operator*(const double& other) const;
	
	Matrix operator+(const Matrix& other) const;
	
	Matrix operator-(const Matrix& other) const;
	
	Matrix operator-() const;
	
	Matrix transpose() const;
	
	static Matrix solveLinearSystem(const Matrix& a, const Matrix& b);
	
};

#endif