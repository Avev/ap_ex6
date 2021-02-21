#include "AnomalyDetector.h"
#include "SimpleAnomalyDetector.h"
#include <iostream>
#include "minCircle.h"

SimpleAnomalyDetector::SimpleAnomalyDetector() {
	corrThreshold = 0.9;
	numRows = 0;
}

SimpleAnomalyDetector::~SimpleAnomalyDetector() {
	// TODO Auto-generated destructor stub
}
/**
 * use the param for offline learning to have correlation data, uses structs and put them in a vector.
 * @param ts data of offline flight.
 */

void SimpleAnomalyDetector::learnNormal(const TimeSeries& ts){
    // iterates on map
    for (auto it = ts.map.begin(); it != ts.map.end(); it++) {
        float max = 0;
        std::string feature1 = it->first;
        std::string feature2;
        // iterates on map after upper one
        it++;
        for (auto iter = it--; iter != ts.map.end(); iter++) {
            // creating array of column so we can use pearson function
            int size = it->second.size();
            float xArr[size];
            float yArr[size];
            // fills vector in arrays to use in pearson
            for (int i = 0; i < size; i++) {
                xArr[i] = it->second[i];
                yArr[i] = iter->second[i];
            }
            float p = abs(pearson(xArr, yArr, it->second.size()));
            //if (p > corrThreshold) {
            if (learnHelp(p, corrThreshold)) {
                if (p > max) {
                    feature2 = iter->first;
                    max = p;
                }
            }
        }
        // fills the struct with the wanted correlation and put it in cf
        learnHelp2(max, feature1, feature2, ts);/*
        if (max > 0) {
            correlatedFeatures cor;
            cor.corrlation = max;
            cor.feature1 = feature1;
            cor.feature2 = feature2;
            int size = ts.map.find(feature1)->second.size();
            float f1Arr[size];
            float f2Arr[size];
            for (int i = 0; i < size; i++) {
                f1Arr[i] = ts.map.find(feature1)->second[i];
                f2Arr[i] = ts.map.find(feature2)->second[i];
            }
            cor.lin_reg = linear_reg(f1Arr, f2Arr, size);
            // find the threshold
            cor.threshold = 0;
            for (int i = 0; i < size; i++) {
                Point p(f1Arr[i], f2Arr[i]);
                if (dev(p,cor.lin_reg) > cor.threshold) {
                    cor.threshold = dev(p,cor.lin_reg);
                }
            }
            cf.push_back(cor);
        }*/
    }
}
/**
 * checks for anomalies and if finds any add an anomaly report to a vector and returns it with all the reports.
 * @param ts flight data to check if they have anomalies.
 * @return vector of the anomaly reports.
 */
vector<AnomalyReport> SimpleAnomalyDetector::detect(const TimeSeries& ts) {
    vector<AnomalyReport> result;
    numRows = ts.map.begin()->second.size();
	int structSize = cf.size();
	// iterates on the struct
	for (int i = 0; i < structSize; i++) {
        string f1 = cf[i].feature1;
        string f2 = cf[i].feature2;
        Line line = cf[i].lin_reg;
        float threshold = cf[i].threshold + (cf[i].threshold * 0.1);
        // iterates on the column's vectors
	    for (int j = 0; j < ts.map.begin()->second.size(); j++) {
            float x = ts.map.find(f1)->second[j];
            float y = ts.map.find(f2)->second[j];
	        Point p(x,y);
	        if (cf[i].isSimple) {
                float de = dev(p, line);
                if (de > threshold) {
                    AnomalyReport ar(f1 + "-" + f2, j + 1);
                    result.push_back(ar);
                }
            }else {
	            float distance = dis(p, cf[i].c.center);
	            if (distance > cf[i].c.radius) {
	                AnomalyReport ar(f1 + "-" + f2, j + 1);
	                result.push_back(ar);
	            }
	        }
	    }
	}
	return result;
}
// checks if the pearson is high enough
bool SimpleAnomalyDetector::learnHelp(float p, float minCor) {
    if (p > minCor) {
        return true;
    }
    return false;
}
// fills the struct with the correlation details
void SimpleAnomalyDetector::learnHelp2(float max, string feature1, string feature2,
                                       const TimeSeries& ts) {
    if (max > 0) {
        correlatedFeatures cor;
        cor.corrlation = max;
        cor.feature1 = feature1;
        cor.feature2 = feature2;
        int size = ts.map.find(feature1)->second.size();
        float f1Arr[size];
        float f2Arr[size];
        for (int i = 0; i < size; i++) {
            f1Arr[i] = ts.map.find(feature1)->second[i];
            f2Arr[i] = ts.map.find(feature2)->second[i];
        }
        cor.lin_reg = linear_reg(f1Arr, f2Arr, size);
        // find the threshold
        cor.threshold = 0;
        for (int i = 0; i < size; i++) {
            Point p(f1Arr[i], f2Arr[i]);
            if (dev(p,cor.lin_reg) > cor.threshold) {
                cor.threshold = dev(p,cor.lin_reg);
            }
        }
        cf.push_back(cor);
    }
}

float SimpleAnomalyDetector::getCorrThreshold() {
    return corrThreshold;
}

void SimpleAnomalyDetector::setCorrThreshold(float corrThresh) {
    corrThreshold = corrThresh;
}

int SimpleAnomalyDetector::getNumRows() {
    return numRows;
}
