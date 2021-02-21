#include "minCircle.h"
#include <iostream>
#include <vector>
#include <cmath>
#include "anomaly_detection_util.h"

// implement
/**
 * uses a recursion to find the minimal circle that holds all the given points.
 * @param points given points
 * @param size number of points (size of points param)
 * @return minimal circle with all points inside
 */
Circle findMinCircle(Point** points,size_t size){
    std::vector<Point> r;
    return rec(points, r, size);
}

// you may add helper functions here
/**
 * calculates the distance between 2 points.
 * @param p1 first point
 * @param p2 second point
 * @return distance between them
 */
float dis(Point p1, Point p2) {
    return sqrtf(powf(p1.x - p2.x, 2) + powf(p1.y - p2.y, 2));
}
/**
 * calculates circle from 2 given points in a way that the points are on the circle's circumference.
 * @param p1 first point
 * @param p2 second point
 * @return circle of the 2 points
 */
Circle twoPointsCircle(Point p1, Point p2) {
    return {Point(((p1.x + p2.x) / 2), (p1.y + p2.y) / 2) , dis(p1, p2) / 2};
}
/**
 * calculates circle from 2 given points in a way that the points are on the circle's circumference.
 * @param p1 first point
 * @param p2 second point
 * @param p3 third point
 * @return circle of the 3 points
 */
Circle threePointsCircle(Point p1, Point p2, Point p3) {
    float x1 = p1.x;
    float x2 = p2.x;
    float x3 = p3.x;
    float y1 = p1.y;
    float y2 = p2.y;
    float y3 = p3.y;
    float yc = ((x1 - x2) * (x2 * x2 - x3 * x3 + y2*y2 - y3 * y3) -
                (x2 - x3) * (x1 * x1 - x2 * x2 + y1 * y1 - y2 * y2)) /
               (2*x3*y1 - 2*x3*y2 + 2*x1*y2 - 2*x2*y1 + 2*x2*y3 - 2*x1*y3);
    float xc = (-(x1*x1) - y1*y1 + 2*yc*y1 + x2*x2 + y2*y2 - 2*yc*y2) / (-2*x1 + 2*x2);
    Point center(xc, yc);
    float r = sqrtf((x1 - xc)*(x1 - xc) + (y1 - yc) * (y1 - yc));
    return {center, r};
}
/**
 * the recursion that finds the minimal circle with all points inside (or on circumference).
 * @param points unchecked points
 * @param r points we create circle from
 * @param size number of unchecked points (size of points param)
 * @return minimal circle holding all points
 */
Circle rec(Point** points, vector<Point> r, int size) {
    if (r.size() == 3 || size == 0) {
        if (r.empty()) {
            return {Point(0,0), 0};
        }
        if (r.size() == 1) {
            return {r[0], 0};
        }
        if (r.size() == 2) {
            return twoPointsCircle(r[0], r[1]);
        }
        bool flag = true;
        for (int i = 0; i < 3; i++) {
            for (int j = i + 1; j < 3; j++) {
                Circle c = twoPointsCircle(r[i], r[j]);
                for (int k = 0;k < 3; k++) {
                    if (dis(c.center, r[k]) > c.radius) {
                        flag = false;
                    }
                }
                if (flag) {
                    return c;
                }
            }
        }
        return threePointsCircle(r[0], r[1], r[2]);
    }
    int idx = std::rand() % size;
    Point p = *points[idx];
    swap(points[idx], points[size - 1]);
    Circle c = rec(points, r, size - 1);
    if (c.radius >= dis(p, c.center)) {
        return c;
    }
    r.push_back(p);
    return rec(points, r, size - 1);
}
