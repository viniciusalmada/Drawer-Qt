#include "manager/model.h"

Model::Model() = default;

Model::~Model() {
	delAllCurves();
}

void Model::delAllCurves() {

}

Box Model::boundingBox() {
	if (mCurves.empty())
		return {0.0, 10.0, 0.0, 10.0};
	
	
	Box box = mCurves[0]->boundingBox();
	for (Curve* c : mCurves) {
		Box temp = c->boundingBox();
		box.update(temp);
	}
	return box;
}

void Model::selectPick(Point pt, double tol, bool shiftKey) {
	if (mCurves.empty())
		return;
	
	Point pC{};
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

void Model::selectFence(Box box, bool shiftKey) {
	if (mCurves.empty()) return;
	
	bool inFence;
	Box tempBox{box};
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

bool Model::snapToCurve(Point& pt, double tol) {
	if (mCurves.empty()) return false;
	
	Point pCurr{};
	Point pClosest{};
	double distMin = tol;
	double dist;
	Curve* targetCurve = nullptr;
	for (Curve* curve: mCurves) {
		pCurr = curve->getPtStart();
		if (pt.dist(pCurr) < tol) {
			dist = pt.dist(pCurr);
			if (dist < distMin) {
				pClosest = pCurr;
				distMin = dist;
				targetCurve = curve;
			}
			continue;
		}
		
		pCurr = curve->getPtEnd();
		if (pt.dist(pCurr) < tol) {
			dist = pt.dist(pCurr);
			if (dist < distMin) {
				pClosest = pCurr;
				distMin = dist;
				targetCurve = curve;
			}
			continue;
		}
		
		pCurr = pt;
		dist = curve->closestPoint(pCurr);
		if (dist < distMin) {
			pClosest = pt;
			distMin = dist;
			targetCurve = curve;
		}
	}
	
	if (targetCurve == nullptr)
		return false;
	
	pt = pClosest;
	return true;
}


