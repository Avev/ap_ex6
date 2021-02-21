
#ifndef MINCIRCLE_H_
#define MINCIRCLE_H_

#include <iostream>
#include <vector>
#include "minCircle.h"
#include <iostream>
#include <vector>
#include <cmath>
#include "anomaly_detection_util.h"

using namespace std;

// ------------ DO NOT CHANGE -----------

class Circle{
public:
	Point center;
	float radius;
	Circle(Point c,float r):center(c),radius(r){}
	Circle():center({0,0}),radius(0){};
};
// --------------------------------------

Circle findMinCircle(Point** points,size_t size);
float dis(Point p1, Point p2);
Circle twoPointsCircle(Point p1, Point p2);
Circle threePointsCircle(Point p1, Point p2, Point p3);
Circle rec(Point** points, vector<Point> r, int size);
#endif /* MINCIRCLE_H_ */
