#ifndef MESH_MAIN_TOOLS_H
#define MESH_MAIN_TOOLS_H

#include <string>
#include <fstream>
#include <vector>
#include <math.h>
#include <algorithm>
#include "triangle.h"
#include <QPointF>
#include "MEF.h"
#include "matrix.h"
#include "singular.h"

using std::vector;
namespace meshMainTools {

#define PI 3.14159265358979323
	
	inline double to2PI(double angle) {
		double twoPi = 2.0 * PI;
		return angle - twoPi * floor(angle / twoPi);
	}
	
	double orientation2D(QPointF a, QPointF b, QPointF c) {
		return (a.x() * b.y() + b.x() * c.y() + c.x() * a.y) - (a.x() * c.y() + b.x() * a.y() + c.x() * b.y);
	}
	//
	
	QPointF VFrpr(QPointF pi, QPointF pj) {
		double pix, piy, pjx, pjy;
		double v2, v1, teta;
		QPointF VFrpr;
		pix = pi.x;
		piy = pi.y;
		pjx = pj.x;
		pjy = pj.y;
		v2 = pjy - piy;
		v1 = pjx - pix;
		teta = fmod(atan2(v2, v1), 2 * PI);
		teta = 4 * teta;
		VFrpr.x() = cos(teta);
		VFrpr.y() = sin(teta);
		
		return VFrpr;
	}
	
	QPointF mean(QPointF vi, QPointF vj) {
		QPointF mean;
		mean.x() = (vi.x() + vj.x) / 2;
		mean.y() = (vi.y() + vj.y) / 2;
		return mean;
	}
	
	vector<QPointF> createVFrpr(const vector<std::pair<QPointF, bool>>& boundary) {
		
		int n;
		vector<QPointF> vf_rpr;
		QPointF vf_rprTemp, pi, pj, pk, vi, vj;
		
		n = boundary.size();
		
		for (int i = 0; i < n; i++) {
			
			pi = boundary[i].first;
			pj = boundary[(i + 1) % n].first;
			vf_rprTemp = VFrpr(pi, pj);
			
			if (boundary[i].second) {
				
				pi = boundary[i].first;
				pj = boundary[i + 1].first;
				pk = boundary[(n + i - 1) % n].first;
				vi = VFrpr(pi, pj);
				vj = VFrpr(pk, pi);
				vf_rprTemp = mean(vi, vj);
			}
			
			vf_rpr.push_back(vf_rprTemp);
		}
		
		return vf_rpr;
	}
	
	vector<double> getBb(vector<QPointF> pol) {
		vector<double> px, py, bB;
		double xmin, xmax, ymin, ymax;
		for (int i = 0; i < pol.size(); i++) {
			px.push_back(pol[i].x);
			py.push_back(pol[i].y);
		}
		
		xmin = *min_element(px.begin(), px.end());
		xmax = *max_element(px.begin(), px.end());
		ymin = *min_element(py.begin(), py.end());
		ymax = *max_element(py.begin(), py.end());
		
		bB.push_back(xmin);
		bB.push_back(xmax);
		bB.push_back(ymin);
		bB.push_back(ymax);
		
		return bB;
	}
	
	bool isInside(vector<QPointF> pol, QPointF p) {
		QPointF p1, p2;
		double x, y, orient;
		int n, ni;
		n = pol.size();
		x = p.x;
		y = p.y;
		ni = 0;
		for (int i = 0; i < n; i++) {
			p1 = pol[i];
			p2 = pol[(i + 1) % n];
			
			orient = orientation2D(p1, p2, p);
			if (p1.y() == p2.y)
				continue;
			if (p1.y() >= y && p2.y() >= y)
				continue;
			if (p1.y() <= y && p2.y() <= y)
				continue;
			if ((y >= p1.y() && orient < 0) || (y <= p1.y() && orient > 0))
				continue;
			if (p2.y() == y)
				continue;
			ni = ni++;
		}
		if (ni % 2 > 0)
			return true;
		
		return false;
	}
	
	vector<QPointF> createInside(vector<QPointF> pol, int ng) {
		
		vector<double> bB;
		vector<QPointF> inside, total;
		vector<double> linex, liney;
		QPointF p;
		double hx, hy, valuex, valuey, px, py;
		
		bB = getBb(pol);
		
		hx = (bB[1] - bB[0]) / (ng - 1);
		hy = (bB[3] - bB[2]) / (ng - 1);
		for (int i = 0; i < ng; i++) {
			valuex = bB[0] + i * hx;
			linex.push_back(valuex);
			valuey = bB[2] + i * hy;
			liney.push_back(valuey);
		}
		for (int j = 0; j < ng; j++) {
			for (int i = 0; i < ng; i++) {
				p.x() = linex[j];
				p.y() = liney[i];
				if (isInside(pol, p)) {
					inside.push_back(p);
				}
			}
		}
		
		total.reserve(pol.size() + inside.size());
		total.insert(total.end(), pol.begin(), pol.end());
		total.insert(total.end(), inside.begin(), inside.end());
		
		return total;
	}
	
	double deltaAng(double tetai, double tetaj) {
		return to2PI(tetaj - tetai + PI) - PI;
	}
	
	int idxPoin(QPointF v0, QPointF v1, QPointF v2) {
		double teta1, teta2, teta3, idx;
		teta1 = to2PI(atan2(v0.y, v0.x));
		teta2 = to2PI(atan2(v1.y, v1.x));
		teta3 = to2PI(atan2(v2.y, v2.x));
		idx = (deltaAng(teta1, teta2) + deltaAng(teta2, teta3) + deltaAng(teta3, teta1)) / (2 * PI);
		return round(idx);
	}
	
	vector<Singular>
	createSingPts(vector<Triangle> mesh, vector<QPointF> total, vector<QPointF> u, vector<Triangle*>* triSing) {
		QPointF v0, v1, v2;
		vector<int> nodes;
		Matrix Pe(3, 3);
		Matrix A(3, 3);
		Matrix Af(4, 4);
		Matrix x(4, 1);
		Matrix xP(3, 1);
		vector<Singular> ptsS;
		vector<Triangle*> mtriSing;
		int idx;
		
		int n = mesh.size();
		for (int i = 0; i < n; i++) {
			nodes = mesh[i].nodes;
			v0 = u[nodes[0]];
			v1 = u[nodes[1]];
			v2 = u[nodes[2]];
			idx = idxPoin(v0, v1, v2);
			if (idx == 0)
				continue;
			for (int j = 0; j < 3; j++) {
				Pe[{0, j}] = total[nodes[j]].x;
				Pe[{1, j}] = total[nodes[j]].y;
				Pe[{2, j}] = 1;
			}
			Matrix M(3, 2, {v0.x, v0.y, v1.x, v1.y, v2.x, v2.y()});
			A = M * M.transpose() * 2;
			Af.setSubMatrix(1, 3, 1, 3, A);
			Af.setSubMatrix(1, 3, 4, 4, Matrix(3, 1, {1, 1, 1}));
			Af.setSubMatrix(4, 4, 1, 3, Matrix(1, 3, {1, 1, 1}));
			Matrix b(4, 1, {0, 0, 0, 1});
			x = Matrix::solveLinearSystemCGS(Af, b);
			xP = Pe * (x.getSubMatrix(1, 3, 1, 1));
			// Create singular object
			QPointF pt = {xP[{0, 0}], xP[{1, 0}]};
			Singular ptS(pt, mesh[i], idx, u, total);
			mtriSing.push_back(&mesh[i]);
			ptsS.push_back(ptS);
		}
		*triSing = mtriSing;
		return ptsS;
	}
} // namespace meshMainTools

#endif