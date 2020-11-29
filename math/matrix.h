#ifndef DRAWER_MATRIX_H
#define DRAWER_MATRIX_H

#include <vector>
#include <functional>

class LUDecomposition;

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
	
	Matrix setOneAtZerosOnDiagonal();
	
	double determinant();
	
	Matrix inverse();
	
	void forEach(const MatrixElem& block);
	
	int rows() const;
	
	int columns() const;
	
	static Matrix solveLinearSystem(Matrix& a, Matrix& b);
	
	static double sumLD(Matrix l, Matrix d, int& i);
	
	static double sumUX(Matrix u, Matrix x, int i);
	
	static Matrix solveLinearSystemCGS(Matrix& a, Matrix& b, int maxIt = 10000, double tol = 1e-10);
	
	double norm();
	
	double dot(Matrix& matrix);
};

class LUDecomposition {

public:
	explicit LUDecomposition(Matrix& matrix);
	
	Matrix l;
	Matrix u;
private:
	static double sumL(Matrix& l, Matrix& u, int& r, int& c);
	
	static double sumU(Matrix& l, Matrix& u, int& r, int& c);
};

#endif