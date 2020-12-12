#include "circlearc.h"

#include <math.h>

# define m_PI           3.14159265358979323846  /* pi */

//-------------------------------------------------------------------------
CircleArc::CircleArc()
{
	m_nPts = 0;
	m_nSteps = 50;
	m_radius = 0;
}
//-------------------------------------------------------------------------

//-------------------------------------------------------------------------
CircleArc::CircleArc(double _x1, double _y1, double _x2, double _y2)
{
	m_x1 = _x1;
	m_y1 = _y1;
	m_x2 = _x2;
	m_y2 = _y2;
	m_nPts = 2;
}
//-------------------------------------------------------------------------

//-------------------------------------------------------------------------
void
CircleArc::addPoint(double _x, double _y)
{
	if (m_nPts == 0)
	{
		m_x1 = _x;
		m_y1 = _y;
		m_nPts++;
	}
	else if (m_nPts == 1)
	{
		m_x2 = _x;
		m_y2 = _y;
		m_nPts++;
	}
	else if (m_nPts == 2)
	{
		m_x3 = _x;
		m_y3 = _y;
		m_nPts++;
	}
}
//-------------------------------------------------------------------------

//-------------------------------------------------------------------------
Point
CircleArc::getPoint(double _t)
{

	Point center, pt;

	center = Point(m_x1, m_y1);

	pt = center + Point(m_radius*cos(m_relative_angle *_t + m_phase), m_radius*sin(m_relative_angle *_t + m_phase));

	return pt;
}
//-------------------------------------------------------------------------

//-------------------------------------------------------------------------
bool
CircleArc::isPossible()
{
	if (m_nPts < 3)
	{
		return false;
	}
	return true;
}
//-------------------------------------------------------------------------

//-------------------------------------------------------------------------
vector<Point>
CircleArc::getPoints()
{
	if (m_nPts == 1)
	{
		vector<Point> tempPts(1);
		tempPts[0] = Point(m_x1, m_y1);
		return tempPts;
	}
	vector<Point> tempPts(3);
	tempPts[0] = Point(m_x1, m_y1);
	tempPts[1] = Point(m_x2, m_y2);
	tempPts[2] = Point(m_x3, m_y3);

	return tempPts;
}
//-------------------------------------------------------------------------

//-------------------------------------------------------------------------
vector<Point>
CircleArc::getPointsToDraw()
{
	vector<Point> tempPts;
	Point center, begin, end, pts;
	double x, y, dot, cross;

	center = Point(m_x1, m_y1);
	begin = Point(m_x2, m_y2);
	end = Point(m_x3, m_y3);
	dot = dotprod(normalize(begin - center), normalize(end - center));
	cross = crossprod((begin - center), (end - center));
	m_relative_angle = acos(dot);
	if (cross <= 0)
	{
		m_relative_angle = acos(-dot);
		m_relative_angle = m_PI + m_relative_angle;
	}
	for (int i = 0; i <= m_nSteps; i++)
	{
		double t = m_phase + i * (m_relative_angle) / m_nSteps;
		x = m_radius*cos(t);
		y = m_radius*sin(t);
		pts = center + Point(x, y);
		tempPts.push_back(pts);

	}


	return tempPts;
}
//-------------------------------------------------------------------------

//-------------------------------------------------------------------------
vector<Point>
CircleArc::getPointsToDraw(Point _pt)
{
	vector<Point> tempPts;
	Point center, begin, pts;
	double x, y, relative_angle, dot, cross;

	center = Point(m_x1, m_y1);

	if (m_nPts == 1)
	{
		tempPts.push_back(center);
		tempPts.push_back(_pt);
	}

	else if (m_nPts == 2)
	{
		tempPts.clear();
		begin = Point(m_x2, m_y2);
		m_radius = euclidiandistance(center, begin);
		//find phase angle
		dot = dotprod(normalize(begin - center), Point(1, 0));
		cross = crossprod(Point(1, 0), (begin - center));
		m_phase = acos(dot);
		if (cross <= 0)
		{
			m_phase = acos(-dot);
			m_phase = m_PI + m_phase;
		}
		//find relative angle
		dot = dotprod(normalize(begin - center), normalize(_pt - center));
		cross = crossprod((begin - center), (_pt - center));
		relative_angle = acos(dot);
		if (cross <= 0)
		{
			relative_angle = acos(-dot);
			relative_angle = m_PI + (relative_angle);
		}
		for (int i = 0; i <= m_nSteps; i++)
		{
			double t = m_phase + i*(relative_angle) / m_nSteps;
			x = m_radius*cos(t);
			y = m_radius*sin(t);
			pts = center + Point(x, y);
			tempPts.push_back(pts);

		}

	}

	return tempPts;
}
//-------------------------------------------------------------------------

//-------------------------------------------------------------------------
double
CircleArc::closestPoint(double* _x, double* _y)
{
	Point center, begin, end,pt;
	double dist, relative_angle_pt,dot,cross, dist_begin, dist_end;

	pt = Point(*_x, *_y);
	center = Point(m_x1, m_y1);
	begin = Point(m_x2, m_y2);
	end = Point(m_x3, m_y3);

	dot = dotprod(normalize(begin - center), normalize(pt - center));
	cross = crossprod((begin - center), (pt - center));
	relative_angle_pt = acos(dot);
	if (cross < 0)
	{
		relative_angle_pt = acos(-dot);
		relative_angle_pt = m_PI + relative_angle_pt;
	}

	if ((relative_angle_pt >= 0) && (relative_angle_pt <= (m_relative_angle)) )
	{
		dist = euclidiandistance(pt, center);
		dist = abs(m_radius - dist);
		*_x = m_x1 + m_radius*cos(m_phase + relative_angle_pt);
		*_y = m_y1 + m_radius*sin(m_phase + relative_angle_pt);
	}
	else
	{
		dist_begin = euclidiandistance(pt, begin);
		dist_end = euclidiandistance(pt, end);
		if (dist_begin <= dist_end)
		{
			dist = dist_begin;
			*_x = m_x2;
			*_y = m_y2;
		}
		else
		{
			dist = dist_end;
			*_x = m_x3;
			*_y = m_y3;
		}
	}

	return dist;
}
//-------------------------------------------------------------------------

//-------------------------------------------------------------------------
void
CircleArc::getBoundBox(double* _xmin, double* _xmax,
double* _ymin, double* _ymax)
{
	*_xmin = (m_x2 < m_x3) ? m_x2 : m_x3;
	*_xmax = (m_x2 > m_x3) ? m_x2 : m_x3;
	*_ymin = (m_y2 < m_y3) ? m_y2 : m_y3;
	*_ymax = (m_y2 > m_y3) ? m_y2 : m_y3;
}
//-------------------------------------------------------------------------

//-------------------------------------------------------------------------
void
CircleArc::setPoints(double _x1, double _y1, double _x2, double _y2, double _x3, double _y3)
{
	m_x1 = _x1;
	m_x2 = _x2;
	m_y1 = _y1;
	m_y2 = _y2;
	m_x3 = _x3;
	m_y3 = _y3;
}
//-------------------------------------------------------------------------
