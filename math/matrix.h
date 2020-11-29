#ifndef DRAWER_MATRIX_H
#define DRAWER_MATRIX_H

#include <vector>
#include <functional>

class Matrix {
	std::vector<double> mData;
	int mNumRows;
	int mNumColumns;

public:
	
	typedef std::function<void(int&, int&, double)> MatrixElem;
	
	Matrix(int r, int c);
	
	Matrix(int r, int c, std::vector<double> data);
	
	explicit Matrix(int dim);
	
	~Matrix();
	
	double& operator[](std::pair<int, int> i);
	
	Matrix operator*(Matrix& other);
	
	Matrix operator*(const double& other) const;
	
	Matrix operator+(Matrix other) const;
	
	Matrix operator-(Matrix other) const;
	
	Matrix operator-() const;
	
	void operator+=(Matrix other);
	
	void operator-=(Matrix other);
	
	Matrix transpose();
	
	double norm();
	
	double dot(Matrix& matrix);
	
	Matrix setOneAtZerosOnDiagonal();
	
	void forEach(const MatrixElem& block);
	
	int rows() const;
	
	int columns() const;
	
	static Matrix solveLinearSystemCGS(Matrix& a, Matrix& b, int maxIt = 10000, double tol = 1e-10);
};

#endif