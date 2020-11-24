#ifndef DRAWERQTPAINTER_POINT_H
#define DRAWERQTPAINTER_POINT_H

#include <cmath>
#include <vector>
#include <QPointF>

struct Point {
	double x = 0.0;
	double y = 0.0;
	
	Point operator+(const Point& p) const {
		return {x + p.x, y + p.y};
	}
	
	Point operator-(const Point& p) const {
		return {x - p.x, y - p.y};
	}
	
	Point operator*(const double& d) const {
		return {x * d, y * d};
	}
	
	Point operator*(const Point& pt) const {
		return {x * pt.x, y * pt.y};
	}
	
	Point operator/(const Point& p) const {
		return {x / p.x, y / p.y};
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
	
	Point trim() const {
		return {static_cast<double>((int) x), static_cast<double>((int) y)};
	}
	
	Point closestPoint(std::vector<Point> pts) const {
		double d = dist(pts[0]);
		Point p = pts[0];
		for (Point pt : pts) {
			if (dist(pt) < d) {
				d = dist(pt);
				p = pt;
			}
		}
		return p;
	}
	
	QPointF toQF() const {
		return QPointF{x, y};
	}
};


#endif //DRAWERQTPAINTER_POINT_H
