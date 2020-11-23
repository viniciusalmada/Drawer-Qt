#ifndef DRAWERQTPAINTER_POINT_H
#define DRAWERQTPAINTER_POINT_H


#include <cmath>

struct Point {
	double x;
	double y;
	
	Point operator+(const Point& p) const {
		return {x + p.x, y + p.y};
	}
	
	Point operator-(const Point& p) const {
		return {x - p.x, y - p.y};
	}
	
	Point operator*(const double& d) const {
		return {x * d, y * d};
	}
	
	double dot(const Point& p) const {
		return x * p.x + y * p.y;
	}
	
	double norm() const {
		return sqrt(x * x + y * y);
	}
	
	double dist(Point& p) const {
		return sqrt((x - p.x) * (x - p.x) + (y - p.y) * (y - p.y));
	}
};


#endif //DRAWERQTPAINTER_POINT_H
