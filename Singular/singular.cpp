#include "singular.h"
#include <array>
#include <math.h>
#include <algorithm> // find


using std::array; 
using std::find;
using std::pow;

#define PI  3.14159265358979323


Singular::Singular(QPointF pt, Triangle tri, int idxPoinc, vector <QPointF> vRpr, vector<QPointF> total)
{
	m_pt = pt;
	m_tri = tri;
	m_idxPoinc = idxPoinc;
	m_vRpr = vRpr;
	m_total = total;
}

Singular::~Singular(){


}

void Singular::Initialize(){
	QPointF p0, p1, p2;
	QPointF v0, v1, v2; 
	p0 = m_total[m_tri.nodes[0]];
	p1 = m_total[m_tri.nodes[1]];
	p2 = m_total[m_tri.nodes[2]];
	v0 = m_vRpr[m_tri.nodes[0]];
	v1 = m_vRpr[m_tri.nodes[1]];
	v2 = m_vRpr[m_tri.nodes[2]];
	BeginIntercession(p0, p1, v0, v1, &m_pEdge1, &m_dEdge1, &m_tEdge1);
	BeginIntercession(p1, p2, v1, v2, &m_pEdge2, &m_dEdge2, &m_tEdge2);
 	BeginIntercession(p2, p0, v2, v0, &m_pEdge3, &m_dEdge3, &m_tEdge3);
	int m_idx_found = m_pEdge1.size() + m_pEdge2.size() + m_pEdge3.size();
	// Check Topology
	if (m_idxPoinc == 1)
	{
		if (m_idx_found != 3)
		{
			printf("The topology of this singular point is wrong");
		}
	}
	else{
		if (m_idx_found != 5)
		{
			printf("The topology of this singular point is wrong");
		}

	}

}
void Singular::Integrate(){
	array <vector <QPointF>, 3> beginPoints = { m_pEdge1, m_pEdge2, m_pEdge3 } ;
	array <vector <QPointF>, 3> beginDirs = { m_dEdge1, m_dEdge2, m_dEdge3 };
	array <vector <double>, 3> beginTs = { m_tEdge1, m_tEdge2, m_tEdge3 };
	// para cada aresta 
	for (int i = 0; i < 3; i++)
	{
		vector<QPointF> pEi = beginPoints[i];
		vector<QPointF> dEi = beginDirs[i];
		vector<double> tEi = beginTs[i];
		if (!pEi.empty())
		{
			for (int j = 0; j < pEi.size(); j++)
			{
				vector<QPointF> curveStreamline;
				QPointF pi = pEi[j];
				QPointF di = dEi[j];
				double ti = tEi[j];
				Triangle triBef = this->m_tri;
				vector<int> edge{ triBef.nodes[i], triBef.nodes[(i + 1) % 3] };
				Triangle triNext;
				// triNext = triBef.getAdj(i);
				triNext.nodes = { 581, 604, 605 };
				QPointF xBef = this->m_pt;
				QPointF xNext = pi;
				bool f = find(m_triSing.begin(), m_triSing.end(), &triNext) != m_triSing.end();
				while (!f)
				{
					curveStreamline.push_back(xBef);
					curveStreamline.push_back(xNext);
					vector <QPointF> cross1 = CreateCross(m_vRpr[triNext.nodes[0]]);
					vector <QPointF> cross2 = CreateCross(m_vRpr[triNext.nodes[1]]);
					vector <QPointF> cross3 = CreateCross(m_vRpr[triNext.nodes[2]]);
					array <vector<QPointF>, 3> cross = { cross1, cross2, cross3 };
					double tNext = 1 - ti;
					vector<int> edgeNext{ edge[1], edge[0] };
					QPointF vi = m_vRpr[edgeNext[0]];
					QPointF vj = m_vRpr[edgeNext[1]];
					QPointF vk = Interpolate(vi, vj, tNext);
					vector<QPointF> crossk = CreateCross(vk);
					//Heun Integration
					QPointF fxi = findNearestVector(crossk, di);
					pair<double, vector<int>> res = Intercept(pi, fxi, edgeNext, triNext);
					ti = res.first; edge = res.second;
					QPointF fyi = findNearestVector(cross[edge[0]], fxi);
					QPointF fyj = findNearestVector(cross[edge[1]], fxi);
					QPointF fxi_ = Interpolate(fyi, fyj,ti);
					QPointF di = { 0.5*(fxi.x()+ fxi_.x), 0.5*(fxi.y()+ fxi_.y) };
					res = Intercept(pi, di, edgeNext, triNext);
					ti = res.first; edge = res.second;
					xBef = pi;
					QPointF pi = Interpolate(m_total[edge[0]], m_total[edge[1]],ti);
					triBef = triNext;
					//triNext = tri.getAdj[edge[0]]
					if (true)
					{
						xNext = pi;
						curveStreamline.push_back(xBef);
						curveStreamline.push_back(xNext);
						break;
					}
					edge = { triBef.nodes[edge[0]], triBef.nodes[edge[1]] };
					xNext = pi;

				}
				m_curveStreamlines.push_back(curveStreamline);
				
			}
		}
	}



}

void Singular::BeginIntercession(QPointF pi, QPointF pj, QPointF vi, QPointF vj, vector <QPointF> *m_pEdgei, vector <QPointF> *m_dEdgei, vector <double> *m_tEdgei) {
	int n = 20; // magic number
	double tol = 1e-3; // magic tol
	vector<QPointF> pEdgei;
	vector<QPointF> vEdgei;
	vector<double> tEdgei;
	double t;
	double h = 1. / n;
	QPointF pk, u, vk,d;
	QPointF cross1Bef, cross2Bef, crossBef, cross1Next, cross2Next, crossNext;
	int signBef, signNext;
	vector<QPointF> crossK;


	pk = Interpolate(pi, pj, 0);
	u = { pk.x()- m_pt.x, pk.y()- m_pt.y()};
	vk = Interpolate(vi, vj, 0);


	crossK = CreateCross(vk);
	cross1Bef = crossK[0];
	cross2Bef = crossK[1];
	int sign1Bef = orient2D(cross1Bef, u);
	int sign2Bef = orient2D(cross2Bef, u);


	for (int i = 1; i <= n+1; i++)
	{
		t = i*h;
		pk = Interpolate(pi, pj, t);
		u = { pk.x()- m_pt.x, pk.y()- m_pt.y()};
		vk = Interpolate(vi, vj, t);
		crossK = CreateCross(vk);
		cross1Next = findNearestVector(crossK, cross1Bef);
		cross2Next = findNearestVector(crossK, cross2Bef);
		int sign1Next = orient2D(cross1Next, u);
		int sign2Next = orient2D(cross2Next, u);

		bool sign1 = sign1Bef != sign1Next;
		bool sign2 = sign2Bef != sign2Next;

		if (sign1 || sign2)
		{
			if (sign1)
			{
				signNext = sign1Next;
				crossNext = cross1Next;
			}
			else
			{
				signNext = sign2Next;
				crossNext = cross2Next;
			}
	

			double tBef = t - h;
			double tNext = t;

			// Bissection method
			for (int j = 0; j < n; j++)
			{
				signBef = signNext; 
				crossBef = crossNext;
				double newT = (tBef + tNext) / 2;
				pk = Interpolate(pi, pj, newT);
				u = { pk.x()- m_pt.x, pk.y()- m_pt.y()};
				vk = Interpolate(vi, vj, newT);
				crossK = CreateCross(vk);
				crossNext = findNearestVector(crossK, crossBef);
				signNext = orient2D(crossNext, u);
				if (signNext != signBef)
				{
					tBef = newT;
				}
				else
				{
					tNext = newT;
				}
			}

			t = (tNext + tBef) / 2;
			pk = Interpolate(pi, pj, t);
			u = { pk.x()- m_pt.x, pk.y()- m_pt.y()};
			u = { (1 / norm(u))*u.x, (1 / norm(u))*u.y()};
			vk = Interpolate(vi, vj, t);
			crossK = CreateCross(vk);
			vk = findNearestVector(crossK, crossBef);
			double dot = (u.x*vk.x()+ u.y*vk.y);
			double err = 1 - abs(dot);
			if ( dot > 0 )
			{
				d = vk;
			}
			else
			{
				d = { -vk.x, -vk.y()};
			}
			pEdgei.push_back(pk);
			tEdgei.push_back(t);
			vEdgei.push_back(d);

		}
		sign1Bef = sign1Next;
		sign2Bef = sign2Next;

	}

	*m_pEdgei = pEdgei;
	*m_dEdgei = vEdgei;
	*m_tEdgei = tEdgei;
	
}


int Singular::orient2D(QPointF u, QPointF v){

	double det = (-u.y*v.x()+ u.x*v.y);
	return (det > 0) ? 1 : ((det < 0) ? -1 : 0);

}
QPointF Singular::Interpolate(QPointF u, QPointF v,double t){
	double x = (1 - t)*u.x()+ t*v.x;
	double y = (1 - t)*u.y()+ t*v.y;
	QPointF p = { x, y };
	return p;
}
vector<QPointF> Singular::CreateCross(QPointF v){
	vector<QPointF> cross; 
	QPointF v0, v1, v2, v3;
	double teta = to2PI(atan2(v.y, v.x));
	teta = teta / 4;
	v0 = { cos(teta), sin(teta) };
	v1 = { cos(teta + PI / 2), sin(teta + PI / 2) };
	v2 = { cos(teta + 2 * PI / 2), sin(teta + 2 * PI / 2) };
	v3 = { cos(teta + 3 * PI / 2), sin(teta + 3 * PI / 2) };
	cross.push_back(v0);
	cross.push_back(v1);
	cross.push_back(v2);
	cross.push_back(v3);
	return cross;

}

QPointF Singular::findNearestVector(vector<QPointF> cross, QPointF v){
	QPointF vi;
	double dot = 0;
	int index = 0;
	for (int i = 0; i < 4; i++)
	{
		vi = cross[i];
		double dotTemp = vi.x*v.x()+ vi.y*v.y;
		if (dotTemp > dot)
		{
			index = i;
			dot = dotTemp;
		}
	}

	return cross[index];

}

pair<double, vector<int>> Singular::Intercept(QPointF pi, QPointF vk, vector<int> edgei, Triangle t){

	pair<double, vector<int>> res;
	vector <int> edge1;
	vector <int> edge2;
	for (int i = 0; i < 3; i++)
	{
		if (t.nodes[i] == edgei[0]){
			if (t.nodes[(i+1)%3] == edgei[1])
			{
				edge1 = { (i + 1) % 3, (i + 2) % 3 };
				edge2 = { (i + 2) % 3, i };
				break;
			}
		}
	}
	QPointF p0 = m_total[t.nodes[0]];
	QPointF p1 = m_total[t.nodes[1]];
	QPointF p2 = m_total[t.nodes[2]];
	QPointF e1 = { p1.x()- p0.x, p1.y()- p0.y()};
	QPointF e2 = { p2.x()- p1.x, p2.y()- p1.y()};
	double L1 = norm(e1);
	double L2 = norm(e2);
	double h = L1 + L2;
	array<vector<int>, 2> edges = { edge1, edge2 };
	QPointF C = pi;
	QPointF D = { pi.x()+ h*vk.x, pi.y()+ h*vk.y()};
	for (int i = 0; i < 2; i++)
	{
		vector<int> edgek = edges[i];
		QPointF A = m_total[t.nodes[edgek[0]]];
		QPointF B = m_total[t.nodes[edgek[1]]];
		if (orient2D(C, D, A) > 0 && orient2D(C, D, B) > 0)
			continue;
		if (orient2D(C, D, A) < 0 && orient2D(C, D, B) < 0)
			continue;
		if (orient2D(A, B, C) > 0 && orient2D(A, B, D) > 0)
			continue;
		if (orient2D(A, B, C) < 0 && orient2D(A, B, D) < 0)
			continue;
		double tk = orient2D(C, D, A) / (orient2D(C, D, A) - orient2D(C, D, B));
		res.first = tk;
		res.second = edgek;
		return res;

	}

}


double Singular::to2PI(double angle)
{
	double twoPi = 2.0 * PI;
	return angle - twoPi * floor(angle / twoPi);
}
double Singular::norm(QPointF v)
{
	return sqrt(pow(v.x,2) + pow(v.y,2));
}
double Singular::orient2D(QPointF a, QPointF b, QPointF c){
	return (a.x()* b.y()+ b.x()* c.y()+ c.x()* a.y) - (a.x()* c.y()+ b.x()* a.y()+ c.x()* b.y);

}

