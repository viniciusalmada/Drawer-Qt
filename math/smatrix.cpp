#include <cmath>
#include <stdexcept>
#include "smatrix.h"

SMatrix::SMatrix(int r, int c, double tol) : mNumRows(r), mNumColumns(c), tolerance(tol) {

}

SMatrix::SMatrix(int rows, int cols, std::vector<double> data, double tol) : mNumRows(rows), mNumColumns(cols),
                                                                             tolerance(tol) {
	for (int i = 0; i < data.size(); i++) {
		if (std::abs(data[i]) <= tol)
			continue;
		int c = i % cols;
		int r = (i - c) / cols;
		mData[{r, c}] = data[i];
	}
}

SMatrix::SMatrix(int dim, double tol) : mNumRows(dim), mNumColumns(dim), tolerance(tol) {

}

SMatrix::~SMatrix() = default;

int SMatrix::numColumns() const {
	return this->mNumColumns;
}

bool SMatrix::get(Position pos, double& value) const {
	if (mData.find(pos) == mData.end()) // not found
		return false;
	value = mData.find(pos)->second;
	return true;
}

void SMatrix::set(Position pos, double value) {
	if (value == 0.0 || std::abs(value) <= this->tolerance) {
		if (mData.find(pos) != mData.end()) { // already exists a key
			mData.erase(pos);
		}
		return;
	}
	
	if (pos.first < mNumRows && pos.second < mNumColumns)
		mData[pos] = value;
	else
		throw std::runtime_error{"Position passed is out of bounds"};
}

void SMatrix::forEach(const LoopMatrixFunction& loop) const {
	int r = 0;
	int c = 0;
	while (r < mNumRows) {
		while (c < mNumColumns) {
			loop({r, c});
			c++;
		}
		c = 0;
		r++;
	}
}

void SMatrix::forEachElement(const LoopMapFunction& loop) const {
	for (const auto& it : mData) {
		loop(it.first, it.second);
	}
}

SMatrix SMatrix::operator*(const SMatrix& other) const {
	SMatrix res{mNumRows, other.mNumColumns};
	res.forEach([&](Position pos) {
		for (int n = 0; n < numColumns(); ++n) {
			double rowVal, colVal;
			if (!this->get({pos.first, n}, rowVal) ||
			    !other.get({n, pos.second}, colVal))
				continue;
			const double prod = rowVal * colVal;
			res.accumulate(pos, prod);
		}
	});
	return res;
}

SMatrix SMatrix::times(const SMatrix& other) const {
	SMatrix res{mNumRows, 1};
	int i = 0;
	this->forEachElement([&](Position pos, double value) {
		double current = res.get({pos.first, 0});
		double newValue = current + value * other.get({pos.second, 0});
		res.set({pos.first, 0}, newValue);
		printf("passing on item %d\n", i);
	});
	return res;
}

void SMatrix::accumulate(Position pos, double value) {
	double current = 0.0;
	get(pos, current);
	double newValue = current + value;
	this->set(pos, newValue);
}

SMatrix SMatrix::operator*(const double& other) const {
	if (std::abs(other) <= tolerance)
		return SMatrix{mNumRows, mNumColumns};
	
	
	SMatrix res{*this};
	res.forEach([&](Position pos) {
		double current = 0.0;
		res.get(pos, current);
		res.set(pos, current * other);
	});
	return res;
}

SMatrix SMatrix::operator+(const SMatrix& other) const {
	SMatrix res{mNumRows, mNumColumns};
	res.forEach([&](Position pos) {
		double a = 0.0;
		double b = 0.0;
		if (this->get(pos, a) || other.get(pos, b)) {
			res.set(pos, a + b);
		}
	});
	return res;
}

SMatrix SMatrix::operator-(const SMatrix& other) const {
	SMatrix res{mNumRows, mNumColumns};
	res.forEach([&](Position pos) {
		double a = 0.0;
		double b = 0.0;
		if (this->get(pos, a) || other.get(pos, b)) {
			res.set(pos, a - b);
		}
	});
	return res;
}

SMatrix SMatrix::operator-() const {
	SMatrix res{*this};
	this->forEachElement([&](Position pos, double value) {
		res.mData[pos] = -value;
	});
	return res;
}

void SMatrix::operator+=(const SMatrix& other) {
	forEach([&](Position pos) {
		double a = 0.0;
		double b = 0.0;
		if (this->get(pos, a) || other.get(pos, b)) {
			set(pos, a + b);
		}
	});
}

void SMatrix::operator-=(const SMatrix& other) {
	forEach([&](Position pos) {
		double a = 0.0;
		double b = 0.0;
		if (this->get(pos, a) || other.get(pos, b)) {
			set(pos, a - b);
		}
	});
}

SMatrix SMatrix::transpose() const {
	SMatrix res{mNumColumns, mNumRows};
	this->forEachElement([&](Position pos, double value) {
		Position inv{pos.second, pos.first};
		res.mData[inv] = value;
	});
	return res;
}

double SMatrix::norm() const {
	double res = 0.0;
	this->forEachElement([&](Position pos, double value) {
		res += value * value;
	});
	return std::sqrt(res);
}

double SMatrix::dot(const SMatrix& other) const {
	double res = 0.0;
	this->forEach([&](Position pos) {
		double a = 0.0;
		double b = 0.0;
		this->get(pos, a);
		other.get(pos, b);
		res += a * b;
	});
	return res;
}

double SMatrix::get(Position pos) const {
	double value = 0.0;
	get(pos, value);
	return value;
}

SMatrix SMatrix::genMatrix(int r, int c) {
	SMatrix res{r, c};
	int i = 0;
	res.forEach([&](Position pos) {
		if (i++ % 100 == 0) {
			double value = rand() % 10;
			res.set(pos, value);
		}
		
	});
	return res;
}
