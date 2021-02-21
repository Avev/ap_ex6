
#include "HybridAnomalyDetector.h"

HybridAnomalyDetector::HybridAnomalyDetector() {
	// TODO Auto-generated constructor stub

}

HybridAnomalyDetector::~HybridAnomalyDetector() {
	// TODO Auto-generated destructor stub
}
// override from simpleAnomalyDetector, now also checks for circle condition
bool HybridAnomalyDetector::learnHelp(float p, float minCor) {
    if (p > minCor || (p < minCor && p > 0.5)) {
        return true;
    }
    return false;
}
// override from simpleAnomalyDetector, now also checks for circle condition.
void HybridAnomalyDetector::learnHelp2(float max, string feature1, string feature2, const TimeSeries& ts) {
    if (max > 0) {
        correlatedFeatures cor;
        cor.corrlation = max;
        cor.feature1 = feature1;
        cor.feature2 = feature2;
        int size = ts.map.find(feature1)->second.size();
        if (max > 0.9) {
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
                if (dev(p, cor.lin_reg) > cor.threshold) {
                    cor.threshold = dev(p, cor.lin_reg);
                }
            }
        } else {
            auto **points = new Point *[size];
            for (int i = 0; i < size; i++) {
                points[i] = new Point(ts.map.find(feature1)->second[i], ts.map.find(feature2)->second[i]);
            }
            cor.c = findMinCircle(points, size);
            cor.c.radius *= 1.1;
            cor.isSimple = false;
            for (int i = 0; i < size; i++) {
                delete points[i];
            }
            delete[] points;
        }
        cf.push_back(cor);
    }
}
