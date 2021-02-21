

#ifndef SIMPLEANOMALYDETECTOR_H_
#define SIMPLEANOMALYDETECTOR_H_

#include "anomaly_detection_util.h"
#include "AnomalyDetector.h"
#include <vector>
#include <algorithm>
#include <string.h>
#include <math.h>
#include "minCircle.h"

struct correlatedFeatures{
	string feature1,feature2;  // names of the correlated features
	float corrlation; // what we get from pearson func
	Line lin_reg; // the line of feature1 and feature 2
	float threshold; // the max dev allowed from a point to the line
	bool isSimple = true;
	Circle c;
};


class SimpleAnomalyDetector:public TimeSeriesAnomalyDetector{
protected:
    vector<correlatedFeatures> cf;
    float corrThreshold;
    int numRows;
public:
	SimpleAnomalyDetector();
	virtual ~SimpleAnomalyDetector();

	virtual void learnNormal(const TimeSeries& ts);
	virtual vector<AnomalyReport> detect(const TimeSeries& ts);

	vector<correlatedFeatures> getNormalModel(){
		return cf;
	}

    virtual bool learnHelp(float p, float minCor);

    virtual void learnHelp2(float max, string feature1, string feature2,
                            const TimeSeries& ts);
    virtual float getCorrThreshold();
    virtual void setCorrThreshold(float corrThresh);
    virtual int getNumRows();
};



#endif /* SIMPLEANOMALYDETECTOR_H_ */
