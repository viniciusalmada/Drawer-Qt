#include <set>
#include <unordered_set>
#include "region.h"
#include "cubicbezier.h"
#include "circlearc.h"
#include "geomutils.h"
#include <cfloat>


Region::Region(std::vector<std::pair<Curve*, bool>>& curves) : mCurves(curves) {

}

QPainterPath Region::pathToFill() const {
	QPainterPath path;
	path.moveTo(mCurves[0].first->getPtStart());
	for (auto pair : mCurves) {
		Curve* c = pair.first;
		if (pair.second) {
			if (c->type() == CurveType::LINE) {
				path.lineTo(c->getPtStart());
				path.lineTo(c->getPtEnd());
			} else if (c->type() == CurveType::CUBIC_BEZIER) {
				path.lineTo(c->getPtStart());
				auto pts = c->getPoints();
				QPointF& c1 = pts[1];
				QPointF& c2 = pts[2];
				QPointF& end = pts[3];
				path.cubicTo(c1, c2, end);
			} else if (c->type() == CurveType::ARC) {
				path.lineTo(c->getPtStart());
				QVector<QLineF> lines = c->getPointsToDraw();
				for (const QLineF& l : lines) {
					path.lineTo(l.p1());
					path.lineTo(l.p2());
				}
			}
		} else {
			if (c->type() == CurveType::LINE) {
				path.lineTo(c->getPtEnd());
				path.lineTo(c->getPtStart());
			} else if (c->type() == CurveType::CUBIC_BEZIER) {
				path.lineTo(c->getPtEnd());
				auto pts = c->getPoints();
				QPointF& c1 = pts[2];
				QPointF& c2 = pts[1];
				QPointF& end = pts[0];
				path.cubicTo(c1, c2, end);
			} else if (c->type() == CurveType::ARC) {
				path.lineTo(c->getPtEnd());
				QVector<QLineF> lines = c->getPointsToDraw();
				std::reverse(lines.begin(), lines.end());
				for (const QLineF& l : lines) {
					path.lineTo(l.p2());
					path.lineTo(l.p1());
				}
			}
		}
	}
	return path;
}

DelaunayModel Region::genDelaunayMesh(int subGrid, int subBoundary) {
	std::vector<QPointF> points{};
	for (const auto pair : mCurves) {
		Curve* c = pair.first;
		QPointF first = c->getPoint(0.0);
		points.emplace_back(first.x(), first.y());
		double t = 1.0 / (double) subBoundary;
		while (t < 1.0) {
			QPointF pt = c->getPoint(t);
			points.emplace_back(pt.x(), pt.y());
			t += 1.0 / (double) subBoundary;
		}
		QPointF last = c->getPoint(1.0);
		points.emplace_back(last.x(), last.y());
	}
	
	
	RectUtils::RectF box = boundingBox();
	double hx = box.getWidth() / (double) (subGrid - 1.0);
	double hy = box.getHeight() / (double) (subGrid - 1.0);
	
	std::vector<double> lineX;
	std::vector<double> lineY;
	for (int i = 0; i < subGrid; ++i) {
		double valX = box.getLeft() + i * hx;
		lineX.push_back(valX);
		double valY = box.getBot() + i * hy;
		lineY.push_back(valY);
	}
	
	for (int i = 0; i < subGrid; ++i) {
		for (int j = 0; j < subGrid; ++j) {
			QPointF pt{lineX[i], lineY[j]};
			if (contains(pt))
				points.push_back(pt);
		}
	}
	
	std::vector<QPointF> pointsV{};
	for (auto pt : points) {
		bool contained = false;
		for (auto ptAdded : pointsV) {
			if (PointUtils::dist(pt, ptAdded) < 0.0000001) {
				contained = true;
				break;
			}
		}
		if (!contained)
			pointsV.push_back(pt);
	}
	
	auto model = DelaunayModel{pointsV};
	
	auto polygons = model.finish();
	
	delete mMesh;
	mMesh = new QVector<QPolygonF>();
	
	for (QPolygonF pol : polygons) {
		QPointF mid01 = GeomUtils::midPoint(pol[0], pol[1]);
		QPointF mid12 = GeomUtils::midPoint(pol[1], pol[2]);
		QPointF mid20 = GeomUtils::midPoint(pol[2], pol[0]);
		int count = 0;
		if (pathToFill().contains(mid01)) count++;
		if (pathToFill().contains(mid12)) count++;
		if (pathToFill().contains(mid20)) count++;
		
		if (count >= 2)
			mMesh->push_back(pol);
	}
	
	return model;
}

RectUtils::RectF Region::boundingBox() const {
	RectUtils::RectF box = mCurves[0].first->boundingBox();
	for (const auto& pair : mCurves) {
		Curve* c = pair.first;
		box.update(c->boundingBox());
	}
	return box;
}

bool Region::contains(QPointF pt) const {
	return pathToFill().contains(pt);
}

Region::~Region() {
	delete mMesh;
}

std::vector<std::pair<QPointF, bool>> Region::getBoundary(int subDiv) {
	
	std::vector<std::pair<QPointF, bool>> pts;
	
	for (auto pair : mCurves) {
		if (pair.second) { // begin to end
			QPointF first = pair.first->getPtStart();
			pts.emplace_back(first, true);
			
			double t = 1.0 / (double) subDiv;
			while (t < 1.0) {
				QPointF pt = pair.first->getPoint(t);
				pts.emplace_back(pt, false);
				t += 1.0 / (double) subDiv;
			}
		} else { // end to begin
			QPointF last = pair.first->getPtEnd();
			pts.emplace_back(last, true);
			
			double t = 1.0 - 1.0 / (double) subDiv;
			while (t > 0.0) {
				QPointF pt = pair.first->getPoint(t);
				pts.emplace_back(pt, false);
				t -= 1.0 / (double) subDiv;
			}
		}
	}
	return pts;
}

void Region::partitionDomain() {
	int subDivBound = 200;
	int subDivGrid = 60;
	
	DelaunayModel meshTriangles = genDelaunayMesh(subDivGrid, subDivBound);
	
	std::vector<std::pair<QPointF, bool>> boundaryPts = getBoundary(subDivBound);
	
	QVector<QPointF> totalPoints = meshTriangles.totalPoints();
	
	
}
