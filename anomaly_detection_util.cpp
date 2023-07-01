
#include <math.h>
#include "anomaly_detection_util.h"


float avg(float* x, int size){
	float sum = 0;
	for (int i = 0; i < size; i++) {
		sum += x[i];
	}
	return sum / size;
}

// returns the variance of X and Y
float var(float* x, int size){
	float sum = 0;
	for (int i = 0; i < size; i++) {
		sum += pow(x[i], 2);
	}
	float avrg = sum / size;
	return avrg - pow(avg(x, size), 2);
}

// returns the covariance of X and Y
float cov(float* x, float* y, int size){
	float sum = 0;
	for (int i = 0; i < size; i++) {
		sum += (x[i] - avg(x, size)) * (y[i] - avg(y, size));
	}
	return sum / size;
}


// returns the Pearson correlation coefficient of X and Y
float pearson(float* x, float* y, int size){
	float xySig = var(x, size) * var(y, size);
	return cov(x, y, size) / sqrt(xySig);
}

// performs a linear regression and returns the line equation
Line linear_reg(Point** points, int size){
	float xArr[size];
	float yArr[size];
	for (int i = 0; i < size; i++) {
		xArr[i] = points[i]->x;
		yArr[i] = points[i]->y;
	}
	float a = cov(xArr,yArr, size) / var(xArr, size);
	float b = avg(yArr, size) - (a * avg(xArr, size));
	return Line(a,b);
}

// performs a linear regression and returns the line equation
Line linear_reg(float* x, float* y, int size){
    float xArr[size];
    float yArr[size];
    for (int i = 0; i < size; i++) {
        xArr[i] = x[i];
        yArr[i] = y[i];
    }
    float a = cov(xArr,yArr, size) / var(xArr, size);
    float b = avg(yArr, size) - (a * avg(xArr, size));
    return Line(a,b);
}

// returns the deviation between point p and the line equation of the points
float dev(Point p,Point** points, int size){
	float a = linear_reg(points, size).f(p.x) - p.y;
	if (a < 0) {
		return -a;
	}
	return a;
}

// returns the deviation between point p and the line
float dev(Point p,Line l){
	float a = l.f(p.x) - p.y;
		if (a < 0) {
		return -a;
	}
	return a;
}
