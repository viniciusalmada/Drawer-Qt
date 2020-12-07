#include "geomutils.h"
#include "point.h"

GeomUtils::Circle::Circle(QPointF c, double r) : center(c), radius(r) {
}

double GeomUtils::ceil2(double num, double interval) {
	return num - fmod(num, interval);
}

GeomUtils::Circle GeomUtils::circle3Points(QPointF p0, QPointF p1, QPointF p2) {
	QPointF mid01 = midPoint(p0, p1);
	QPointF perpend01 = perpendToLine(p0, p1);
	QPointF mid12 = midPoint(p1, p2);
	QPointF perpend12 = perpendToLine(p1, p2);
	QPointF mid20 = midPoint(p2, p0);
	QPointF perpend20 = perpendToLine(p2, p0);
	
	QPointF cross0 = linesCrossingPoint(mid01, perpend01, mid12, perpend12);
	QPointF cross1 = linesCrossingPoint(mid12, perpend12, mid20, perpend20);
	QPointF cross2 = linesCrossingPoint(mid20, perpend20, mid01, perpend01);
	
	QPointF center = (cross0 + cross1 + cross2) / 3.0;
	double radius = PointUtils::dist(center, p0);
	
	return {center, radius};
}

QPointF GeomUtils::linesCrossingPoint(const QPointF& p1, const QPointF& p2, const QPointF& q1, const QPointF& q2) {
	double a1 = p1.y() - p2.y();
	double b1 = p2.x() - p1.x();
	double c1 = p1.x() * p2.y() - p1.y() * p2.x();
	
	double a2 = q1.y() - q2.y();
	double b2 = q2.x() - q1.x();
	double c2 = q1.x() * q2.y() - q1.y() * q2.x();
	
	double xp = (c2 * b1 - c1 * b2) / (a1 * b2 - a2 * b1);
	double yp = (a2 * c1 - a1 * c2) / (a1 * b2 - a2 * b1);
	
	return {xp, yp};
}

QPointF GeomUtils::midPoint(QPointF p0, QPointF p1) {
	return (p0 + p1) / 2.0;
}

QPointF GeomUtils::perpendToLine(QPointF a, QPointF b) {
	QPointF ba = a - b;
	QPointF baPerpend{ba.y(), -ba.x()};
	QPointF mid = midPoint(a, b);
	return mid + baPerpend;
}

bool GeomUtils::isConvexPolygon(std::vector<QPointF> pts) {
	if (pts.size() < 3) return false;
	
	for (int i = 0; i < pts.size(); ++i) {
		QPointF p0;
		QPointF p1;
		QPointF p2;
		if (i == 0) {
			p0 = pts.back();
			p1 = pts[i];
			p2 = pts[i + 1];
		} else if (i == pts.size() - 1) {
			p0 = pts[i - 1];
			p1 = pts[i];
			p2 = pts.front();
		} else {
			p0 = pts[i - 1];
			p1 = pts[i];
			p2 = pts[i + 1];
		}
		if (orientation(p0, p1, p2) != Orientation::LEFT)
			return false;
		
	}
	return true;
}

GeomUtils::Orientation GeomUtils::orientation(QPointF a, QPointF b, QPointF c) {
	double det = (a.x() * b.y() + b.x() * c.y() + c.x() * a.y()) -
	             (a.x() * c.y() + b.x() * a.y() + c.x() * b.y());
	if (det > 0.0) return Orientation::LEFT;
	if (det < 0.0) return Orientation::RIGHT;
	return Orientation::COLLINEAR;
}

bool GeomUtils::edgeContains(const QPointF& pt0, const QPointF& pt1, const QPointF& pt) {
	return orientation(pt0, pt, pt1) == Orientation::COLLINEAR;
}

std::array<QPointF, 3> GeomUtils::triangleBox(const std::vector<QPointF>& pts) {
	double maxX = pts[0].x();
	double minX = pts[0].x();
	double maxY = pts[0].y();
	double minY = pts[0].y();
	
	for (const auto& pt : pts) {
		maxX = pt.x() > maxX ? pt.x() : maxX;
		minX = pt.x() < minX ? pt.x() : minX;
		maxY = pt.x() > maxY ? pt.y() : maxY;
		minY = pt.x() < minY ? pt.y() : minY;
	}
	
	double m;
	if (maxX - minX > maxY - minY)
		m = (maxY - minX) / 2.0;
	else
		m = (maxY - minY) / 2.0;
	
	double centerX = minX + m;
	double centerY = minY + m;
	
	QPointF center{centerX, centerY};
	
	QPointF p0 = QPointF{-3.0 * m, -3.0 * m} + center;
	QPointF p1 = QPointF{3.0 * m, 0.0} + center;
	QPointF p2 = QPointF{0.0, 3.0 * m} + center;
	
	return {p0, p1, p2};
}

bool GeomUtils::polygonContains(const std::vector<QPointF>& pts, QPointF pt) {
	QPointF ptInf(std::abs(pt.x()) * 100, pt.y());
	int counter = 0;
	for (int i = 0; i < pts.size(); ++i) {
		QPointF p0;
		QPointF p1;
		if (i == 0) {
			p0 = pts.back();
			p1 = pts[i];
		} else if (i == pts.size() - 1) {
			p0 = pts[i];
			p1 = pts.front();
		} else {
			p0 = pts[i];
			p1 = pts[i + 1];
		}
		bool check = checkLinesCrossing(p0, p1, pt, ptInf);
		if (check) counter++;
	}
	
	return counter % 2 != 0;
}

bool GeomUtils::checkLinesCrossing(QPointF a, QPointF b, QPointF c, QPointF d) {
	Orientation abc = orientation(a, b, c);
	Orientation abd = orientation(a, b, d);
	Orientation cda = orientation(c, d, a);
	Orientation cdb = orientation(c, d, b);
	
	return abc != abd && cda != cdb;
}

bool GeomUtils::Circle::contains(const QPointF& pt) const {
	return PointUtils::dist(center, pt) <= radius;
}