

#ifndef HYBRIDANOMALYDETECTOR_H_
#define HYBRIDANOMALYDETECTOR_H_

#include "SimpleAnomalyDetector.h"
#include "minCircle.h"

class HybridAnomalyDetector:public SimpleAnomalyDetector {
public:
	HybridAnomalyDetector();
	virtual ~HybridAnomalyDetector();
    bool learnHelp(float p, float minCor) override;
    void learnHelp2(float max, string feature1, string feature2, const TimeSeries& ts) override;

};

#endif /* HYBRIDANOMALYDETECTOR_H_ */
