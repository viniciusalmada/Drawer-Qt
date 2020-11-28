#include <geom/point.h>
#include <QRectF>
#include <utils/box.h>
#include "manager/model.h"

Model::Model() = default;

Model::~Model() {
	delAllCurves();
}

void Model::delAllCurves() {

}

RectUtils::RectF Model::boundingBox() {
	if (mCurves.empty())
		return {0, 10, 0, 10};
	
	
	RectUtils::RectF box = mCurves[0]->boundingBox();
	for (Curve* c : mCurves) {
		box.update(c->boundingBox());
	}
	return box;
}

void Model::selectPick(QPointF pt, double tol, bool shiftKey) {
	if (mCurves.empty())
		return;
	
	QPointF pC{};
	Curve* target = nullptr;
	double dist = tol;
	for (Curve* curve : mCurves) {
		pC = pt;
		double d = curve->closestPoint(pC);
		if (d < dist) {
			dist = d;
			target = curve;
		}
	}
	
	if (target != nullptr)
		target->toggleSelection();
	
	if (!shiftKey) {
		for (Curve* curve : mCurves) {
			if (curve != target)
				curve->unselect();
		}
	}
}

void Model::selectFence(RectUtils::RectF box, bool shiftKey) {
	if (mCurves.empty()) return;
	
	bool inFence;
	RectUtils::RectF tempBox{box};
	for (Curve* curve : mCurves) {
		tempBox = curve->boundingBox();
		inFence = box.contains(tempBox);
		
		if (inFence)
			curve->select();
		else {
			if (!shiftKey)
				curve->unselect();
		}
	}
}

void Model::insertCurve(Curve* curve) {
	mCurves.push_back(curve);
}

void Model::delCurve(int curveId) {
	delete mCurves[curveId];
	mCurves.erase(mCurves.begin() + curveId);
}

void Model::delSelectedCurves() {
	for (int i = 0; i < mCurves.size(); ++i) {
		if (mCurves[i]->isSelected()) {
			delCurve(i);
		}
	}
}

bool Model::snapToCurve(QPointF& pt, double tol) {
	if (mCurves.empty()) return false;
	
	QPointF pCurr{};
	QPointF pClosest{};
	double distMin = tol;
	double dist;
	Curve* targetCurve = nullptr;
	for (Curve* curve: mCurves) {
		pCurr = pt;
		dist = curve->closestPoint(pCurr);
		if (dist < distMin) {
			pClosest = pCurr;
			distMin = dist;
			targetCurve = curve;
		}
	}
	
	if (targetCurve == nullptr)
		return false;
	
	pt = pClosest;
	return true;
}

bool Model::isEmpty() {
	return mCurves.empty();
}


