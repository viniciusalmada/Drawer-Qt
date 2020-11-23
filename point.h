#ifndef DRAWERQTPAINTER_POINT_H
#define DRAWERQTPAINTER_POINT_H

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
};


#endif //DRAWERQTPAINTER_POINT_H
