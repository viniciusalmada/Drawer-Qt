#ifndef DRAWERQTPAINTER_POINT_H
#define DRAWERQTPAINTER_POINT_H

#include <cmath>
#include <vector>
#include <QPointF>

namespace PointUtils {
	
	static double dist(QPointF p1, QPointF p2) {
		return sqrt((p1.x() - p2.x()) * (p1.x() - p2.x()) +
		            (p1.y() - p2.y()) * (p1.y() - p2.y()));
	}
	
	static QPointF closestPoint(QPointF pt, std::vector<QPointF> pts) {
		QPointF pf = QPointF{pts[0]};
		double d = dist(pt, pf);
		for (QPointF& p : pts) {
			if (dist(pt, QPointF{p}) < d) {
				d = dist(pt, QPointF{p});
				pf = p;
			}
		}
		return pf.toPoint();
	}
	
	static double norm(const QPointF& pt) {
		return sqrt(pt.x() * pt.x() + pt.y() * pt.y());
	}
	
	static QPointF normalize(const QPointF& pt) {
		double n = norm(pt);
		return {pt.x() / n, pt.y() / n};
	}
	
	static double crossProd(const QPointF& pt1, const QPointF& pt2) {
		QPointF pt1Inv{-pt1.y(), pt1.x()};
		return QPointF::dotProduct(pt1Inv, pt2);
	}
};


#endif //DRAWERQTPAINTER_POINT_H
