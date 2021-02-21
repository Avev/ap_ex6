

#ifndef TIMESERIES_H_
#define TIMESERIES_H_

#include <vector>
#include <map>

using namespace std;

class TimeSeries {
    vector<vector<string>> cols;
public:
    std::map<string, vector<float>> map;
	explicit TimeSeries(const char* CSVfileName);
};



#endif /* TIMESERIES_H_ */
