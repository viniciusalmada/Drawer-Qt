#include "region.h"
#include "cubicbezier.h"

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
			}
		}
	}
	return path;
}