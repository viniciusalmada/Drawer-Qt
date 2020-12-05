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

public:
	
	SMatrix(int r, int c, double tol = 1.0e-10);
	
	SMatrix(int rows, int cols, std::vector<double> data, double tol = 1.0e-10);
	
	explicit SMatrix(int dim, double tol = 1.0e-10);
	
	virtual ~SMatrix();
	
	bool get(Position pos, double& value) const;
	
	void set(Position pos, double value);
	
	void accumulate(Position pos, double value);
	
	int numColumns() const;
	
	SMatrix operator*(SMatrix& other) const;
	
	SMatrix operator*(double& other) const;
	
	SMatrix operator+(SMatrix& other) const;
	
	SMatrix operator-(SMatrix& other) const;
	
	SMatrix operator-() const;
	
	void operator+=(SMatrix& other);
	
	void operator-=(SMatrix& other);
	
	SMatrix transpose() const;
	
	double norm() const;
	
	double dot(SMatrix& other) const;
};

#endif