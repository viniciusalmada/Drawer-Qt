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
		QPointF center;
		double radius{};
	
	public:
		Circle(QPointF c, double r);
		
		bool contains(const QPointF& pt) const;
	};
	
	static double ceil2(double num, double interval);
	
	static Orientation orientation(QPointF a, QPointF b, QPointF c);
	
	static bool checkLinesCrossing(QPointF a, QPointF b, QPointF c, QPointF d);
	
	static QPointF linesCrossingPoint(const QPointF& p1, const QPointF& p2, const QPointF& q1, const QPointF& q2);
	
	static QPointF midPoint(QPointF p0, QPointF p1);
	
	static QPointF perpendToLine(QPointF a, QPointF b);
	
	static bool isConvexPolygon(std::vector<QPointF> pts);
	
	static Circle circle3Points(QPointF p0, QPointF p1, QPointF p2);
	
	static bool edgeContains(const QPointF& pt0, const QPointF& pt1, const QPointF& pt);
	
	static std::array<QPointF, 3> triangleBox(const std::vector<QPointF>& pts);
	
	static bool polygonContains(const std::vector<QPointF>& pts, QPointF pt);
}

#endif