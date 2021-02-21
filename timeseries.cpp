#include <fstream>
#include "timeseries.h"
#include <vector>
#include <iostream>
#include <string>
#include <sstream>
#include <map>
/**
 * opens the given csv file and saves it as columns in a map of column vectors.
 * @param CSVfileName csv file with flight data
 */
TimeSeries::TimeSeries(const char* CSVfileName) {
    fstream csv;
    csv.open(CSVfileName);
    string line;
    std::vector<vector<string>> csvRows;
    // inserts the csv data to vectors
    while (std::getline(csv, line)) {
        std::stringstream lineStream(line);
        std::string word;
        std::vector<string> row;
        while (std::getline(lineStream, word, ',')) {
            row.push_back(word);
        }
        csvRows.push_back(row);
    }
    // create columns of the csv data and put it in map
    std::vector<vector<string>> aa; //check start
    for (int i = 0; i < csvRows[0].size(); i++) {
        std::vector<string> col;
        for (int j = 0; j < csvRows.size(); j++) {
            col.push_back(csvRows[j][i]);
        }
        cols.push_back(col);
    }
    for (int i = 0; i < cols.size(); i++) {
        std::vector<float> col;
        for (int j = 1; j < cols[0].size(); j++) {
            col.push_back(stof(cols[i][j]));
        }
        this->map.insert({cols[i][0], col});
    }
}
