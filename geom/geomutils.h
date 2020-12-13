#ifndef GEOM_UTILS_H
#define GEOM_UTILS_H

#include <QPointF>

#include <cmath>
#include <vector>
#include <array>

namespace GeomUtils {
	enum class Orientation {
		LEFT, RIGHT, COLLINEAR
	};
	
	class Circle {
	
	public:
		Circle(QPointF c, double r);
		
		bool contains(const QPointF& pt) const;
		
		double radius{};
		QPointF center;
	};
	
	double ceil2(double num, double interval);
	
	Orientation orientation(QPointF a, QPointF b, QPointF c);
	
	bool checkLinesCrossing(QPointF a, QPointF b, QPointF c, QPointF d);
	
	QPointF linesCrossingPoint(const QPointF& p1, const QPointF& p2, const QPointF& q1, const QPointF& q2);
	
	QPointF midPoint(QPointF p0, QPointF p1);
	
	QPointF perpendToLine(QPointF a, QPointF b);
	
	bool isConvexPolygon(std::vector<QPointF> pts);
	
	Circle circle3Points(QPointF p0, QPointF p1, QPointF p2);
	
	bool edgeContains(const QPointF& pt0, const QPointF& pt1, const QPointF& pt);
	
	std::array<QPointF, 3> triangleBox(const std::vector<QPointF>& pts);
	
	bool polygonContains(QPointF p0, QPointF p1, QPointF p2, QPointF pt);
	
	double pseudoAngle(double x1, double y1, double x2, double y2);
}

#endif