

#ifndef COMMANDS_H_
#define COMMANDS_H_

#include<iostream>
#include <string.h>
#include <fstream>
#include <vector>
#include <sstream>
#include "HybridAnomalyDetector.h"
#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <pthread.h>
#include <thread>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

using namespace std;

class argus {
public:
    HybridAnomalyDetector* hybrid;
    vector<AnomalyReport> arVec;
};

struct anomsRange {
    long start = -1;
    long end = -1;
};

class DefaultIO{
public:
	virtual string read()=0;
	virtual void write(string text)=0;
	virtual void write(float f)=0;
	virtual void read(float* f)=0;
	virtual ~DefaultIO(){}

	// you may add additional methods here
};

// you may add here helper classes
class SocketIO:public DefaultIO {
    int clientID;
public:
    SocketIO(int ID) {
        clientID = ID;
    }
    string read() override {
        string buffer = "";
        char c = 0;
        ::read(clientID, &c, sizeof(char));
        while (c != '\n') {
            buffer += c;
            ::read(clientID, &c, sizeof(char));
        }
        return buffer;
    }
    void write(string text) override {
        ::write(clientID, text.c_str(), text.size());
    }
    void write(float f) override {
        std::ostringstream s;
        s << f;
        ::write(clientID, s.str().c_str(), s.str().size());
    }
    void read(float* f) override {
        string buffer = "";
        char c = 0;
        ::read(clientID, &c, sizeof(char));
        while (c != '\n') {
            buffer += c;
            ::read(clientID, &c, sizeof(char));
        }
        stringstream s(buffer);
        s >> *f;
    }
    ~SocketIO() {}
};

class StandardIO:public DefaultIO {
    ifstream in;
    ofstream out;
public:
    StandardIO(string inputFile, string outputFile) {
        in.open(inputFile);
        out.open(outputFile);
    }
    string read() override {
        string s;
        in >> s;
        return s;
    }
    void write(string text) override {
        out << text;
    }
    void write(float f) override {
        out << f;
    }
    void close() {
        if (in.is_open()) {
            in.close();
        }
        if (out.is_open()) {
            out.close();
        }
    }
    ~StandardIO() override {
        close();
    }
};

// you may edit this class
class Command{
	DefaultIO* dio;
	string description;
	argus* arg;
	friend class UploadCommand;
    friend class SettingsCommand;
    friend class DetectCommand;
    friend class DisplayCommand;
    friend class UploadAndAnalyzeCommand;

public:
	Command(DefaultIO* dio):dio(dio){}

    string getDescription() {
        return description;
    }
	virtual void execute()=0;
	virtual ~Command(){
	}
};

// implement here your command classes
class UploadCommand: public Command {
public:
    UploadCommand(DefaultIO *dio) : Command(dio) {
        description =  "upload a time series csv file";
    }
    UploadCommand(DefaultIO *dio, argus* arg) : Command(dio) {
        description =  "upload a time series csv file";
        this->arg = arg;
    }

        void execute() override {
        dio->write("Please upload your local train CSV file.\n");
        ofstream anomalyTraincsv;
        anomalyTraincsv.open("anomalyTrain.csv");
        string done = "done";
        string line = dio->read();
        while (!(line == done)) {
            anomalyTraincsv << line << "\n";
            line = dio->read();
        }
        anomalyTraincsv.close();
        dio->write("Upload complete.\n");
        dio->write("Please upload your local test CSV file.\n");
        ofstream anomalyTestcsv;
        anomalyTestcsv.open("anomalyTest.csv");
        line = dio->read();
        while (!(line == done)) {
            anomalyTestcsv << line << "\n";
            line = dio->read();
        }
        anomalyTestcsv.close();
        dio->write("Upload complete.\n");
    }
};

class SettingsCommand:public Command {
public:
    SettingsCommand(DefaultIO *dio) : Command(dio) {
        description = "algorithm settings";
    }
    SettingsCommand(DefaultIO *dio, argus* arg) : Command(dio) {
        description = "algorithm settings";
        this->arg = arg;
    }
    void execute() override {
        dio->write("The current correlation threshold is ");
        dio->write(arg->hybrid->getCorrThreshold());
        dio->write("\n");
        while (true) {
            dio->write("Type a new threshold\n");
            float thresh = 2;
            dio->read(&thresh);
            if (thresh <= 1 && thresh >= 0) {
                arg->hybrid->setCorrThreshold(thresh);
                return;
            }
            dio->write("please choose a value between 0 and 1\n");
            dio->read();
        }
    }
};

class DetectCommand:public Command {
public:
    DetectCommand(DefaultIO *dio) : Command(dio) {
        description = "detect anomalies";
    }
    DetectCommand(DefaultIO *dio, argus* arg) : Command(dio) {
        description = "detect anomalies";
        this->arg = arg;
    }
    void execute() override {
        TimeSeries tsTrain("anomalyTrain.csv");
        TimeSeries tsTest("anomalyTest.csv");
        arg->hybrid->learnNormal(tsTrain);
        vector<AnomalyReport> vec = arg->hybrid->detect(tsTest);
        for (int i = 0; i < vec.size(); i++) {
            arg->arVec.push_back(vec[i]);
        }
        dio->write("anomaly detection complete.\n");
    }
};

class DisplayCommand:public Command {
public:
    DisplayCommand(DefaultIO *dio) : Command(dio) {
        description = "display results";
    }
    DisplayCommand(DefaultIO *dio, argus* arg) : Command(dio) {
        description = "display results";
        this->arg = arg;
    }
    void execute() override {
        int size = arg->arVec.size();
        for (int i = 0; i < size; i++) {
            dio->write(arg->arVec[i].timeStep);
            dio->write("    ");
            dio->write(arg->arVec[i].description + "\n");
        }
        dio->write("Done.\n");
    }
};

class UploadAndAnalyzeCommand:public Command {
public:
    UploadAndAnalyzeCommand(DefaultIO *dio) : Command(dio) {
        description = "upload anomalies and analyze results";
    }
    UploadAndAnalyzeCommand(DefaultIO *dio, argus* arg) : Command(dio) {
        description = "upload anomalies and analyze results";
        this->arg = arg;
    }

    void execute() override {
        //sort(arg->arVec.begin(), arg->arVec.end(), compareTimesSep);
        dio->write("Please upload your local anomalies file.\n");
        map<string, vector<anomsRange>> rangeMap;
        // sorts the AnomalyReports by description and time in a map(rangeMap)
        for (int i = 0; i < arg->arVec.size(); i++) {
            if (rangeMap.count(arg->arVec[i].description) == 0) {
                vector<anomsRange> vec;
                anomsRange anom;
                anom.start = arg->arVec[i].timeStep;
                anom.end = anom.start;
                vec.push_back(anom);
                rangeMap.insert({arg->arVec[i].description, vec});
            } else {
                if (arg->arVec[i].timeStep == rangeMap.find(arg->arVec[i].description)->second.back().end + 1) {
                    rangeMap.find(arg->arVec[i].description)->second.back().end++;
                } else {
                    anomsRange anom;
                    anom.start = arg->arVec[i].timeStep;
                    anom.end = anom.start;
                    rangeMap.find(arg->arVec[i].description)->second.push_back(anom);
                }
            }
        }
        float p = 0;
        float tp = 0;
        float fp = 0;
        float n = arg->hybrid->getNumRows();
        float bigN = n;
        int size = 0;
        long start = 0;
        long end = 0;
        string done = "done";
        // loops on the lines, breaks them to start and end and check if and/or how many in the range.
        while (true) {
            string line = dio->read();
            stringstream strstream(line);
            if (line == done) {
                break;
            }
            size++;
            strstream >> start;
            strstream.ignore();
            strstream >> end;
            // loops on the map(rangeMap)
            for (auto it = rangeMap.begin(); it != rangeMap.end(); it++) {
                // loops on each vector for each description on the map
                for (int i = 0; i < it->second.size(); i++) {
                    // if the element is in the range it adds 1 to tp and deletes it from vector
                    if (!(it->second[i].start > end || it->second[i].end < start)) {
                        tp++;
                        it->second.erase(it->second.begin() + i);
                    }
                }
            }
            p++;
            bigN -= (end - start + 1);
        }
        // iterates on the map and calculates fp
        for (auto it = rangeMap.begin(); it != rangeMap.end(); it++) {
            fp += it->second.size();
        }
        float tpr = floor((tp / p) * 1000);
        float fpr = floor((fp / bigN) * 1000);
        tpr /= 1000;
        fpr /= 1000;
        dio->write("Upload complete.\n");
        dio->write("True Positive Rate: ");
        dio->write(tpr);
        dio->write("\n");
        dio->write("False Positive Rate: ");
        dio->write(fpr);
        dio->write("\n");
    }
};


#endif /* COMMANDS_H_ */
