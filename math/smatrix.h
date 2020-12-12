#ifndef DRAWER_SMATRIX_H
#define DRAWER_SMATRIX_H

#include <functional>
#include <map>
#include <vector>

typedef std::pair<int, int> Position;

typedef std::function<void(Position)> LoopMatrixFunction;
typedef std::function<void(Position, double)> LoopMapFunction;

class SMatrix {
	std::map<Position, double> mData;
	int mNumRows;
	int mNumColumns;
	double tolerance;

	inline void forEach(const LoopMatrixFunction& loop) const;

	inline void forEachElement(const LoopMapFunction& loop) const;

	bool get(Position pos, double& value) const;

public:

	SMatrix(int r, int c, double tol = 1.0e-10);

	SMatrix(int rows, int cols, std::vector<double> data, double tol = 1.0e-10);

	explicit SMatrix(int dim, double tol = 1.0e-10);

	virtual ~SMatrix();

	double get(Position pos) const;

	void set(Position pos, double value);

	void accumulate(Position pos, double value);

	int numColumns() const;

	SMatrix operator*(const SMatrix& other) const;

	SMatrix times(const SMatrix& other) const;

	SMatrix operator*(const double& other) const;

	SMatrix operator+(const SMatrix& other) const;

	SMatrix operator-(const SMatrix& other) const;

	void operator-=(const SMatrix& other);

	SMatrix operator-() const;

	void operator+=(const SMatrix& other);



	SMatrix transpose() const;

	double norm() const;

	double dot(const SMatrix& other) const;

	static SMatrix genMatrix(int r, int c);

	static SMatrix solveLinearSystemCG(SMatrix& a, SMatrix& b, int maxIt = 10000, double tol = 1e-2);
};

#endif