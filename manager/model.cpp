#include <QRectF>
#include <geom/box.h>
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
	for (const Curve* c : mCurves) {
		box.update(c->boundingBox());
	}
	return box;
}

void Model::selectPick(QPointF pt, double tol, bool shiftKey) {
	if (mCurves.empty())
		return;
	
	QPointF pC{};
	int target = -1;
	double dist = tol;
	for (int i = 0; i < mCurves.size(); i++) {
		pC = pt;
		double d = mCurves[i]->closestPoint(pC);
		if (d < dist) {
			dist = d;
			target = i;
		}
	}
	
	if (target != -1)
		mCurves[target]->toggleSelection();
	
	if (!shiftKey) {
		for (int i = 0; i < mCurves.size(); i++) {
			if (i != target)
				mCurves[i]->unselect();
		}
	}
}

void Model::selectFence(const RectUtils::RectF& box, bool shiftKey) {
	if (mCurves.empty()) return;
	
	bool inFence;
	RectUtils::RectF tempBox;
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
	bool curveRemoved;
	do {
		curveRemoved = false;
		for (int i = 0; i < mCurves.size(); ++i) {
			if (mCurves[i]->isSelected()) {
				delCurve(i);
				curveRemoved = true;
				break;
			}
		}
	} while (curveRemoved);
}

bool Model::snapToCurve(QPointF& pt, double tol) {
	if (mCurves.empty()) return false;
	
	QPointF pCurr{};
	QPointF pClosest{};
	double distMin = tol;
	double dist;
	int targetCurve = -1;
	for (int i = 0; i < mCurves.size(); i++) {
		pCurr = pt;
		dist = mCurves[i]->closestPoint(pCurr);
		if (dist < distMin) {
			pClosest = pCurr;
			distMin = dist;
			targetCurve = i;
		}
	}
	
	if (targetCurve == -1)
		return false;
	
	pt = pClosest;
	return true;
}

bool Model::isEmpty() {
	return mCurves.empty();
}


