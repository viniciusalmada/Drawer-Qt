#ifndef CIRCLEARC_H
#define CIRCLEARC_H

#include "curve.h"

class CircleArc : public Curve
{
 public:
   CircleArc();
   CircleArc(double _x1, double _y1, double _x2, double _y2);

  // virtual methods implemented
  int getType() { return (int)Curve::CIRCLE; };
  bool isUnlimited() { return false; };
  void addPoint( double _x, double _y );
  Point getPoint( double _t );
  bool isPossible();
  vector<Point> getPoints();
  vector<Point> getPointsToDraw();
  vector<Point> getPointsToDraw( Point _pt );
  double closestPoint( double* _x, double* _y );
  void getBoundBox( double* _xmin, double* _xmax,
                    double* _ymin, double* _ymax );
  double getXinit() { return m_x2; };
  double getYinit() { return m_y2; };
  double getXend() { return m_x3; };
  double getYend() { return m_y3; };

  // specific line methods
  void setPoints(double _x1, double _y1, double _x2, double _y2, double _x3, double _y3);

 private:
  double m_x1;  //center circle
  double m_y1;
  double m_x2;  // begin point
  double m_y2;
  double m_x3;  // end point
  double m_y3;
  int m_nSteps;
  double m_radius; 
  double m_relative_angle;
  double m_phase;
};

#endif // CIRCLEARC_H
