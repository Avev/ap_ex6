cmake_minimum_required(VERSION 3.17)
project(ap_ex6)

set(CMAKE_CXX_STANDARD 11)
set(GCC_COVERAGE_COMPILE_FLAGS "-pthread -std=c++11")

set(CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} ${GCC_COVERAGE_COMPILE_FLAGS}")

add_executable(ap_ex6 MainTrain.cpp anomaly_detection_util.cpp anomaly_detection_util.h AnomalyDetector.h
        SimpleAnomalyDetector.cpp SimpleAnomalyDetector.h timeseries.cpp timeseries.h HybridAnomalyDetector.cpp
        HybridAnomalyDetector.h minCircle.h minCircle.cpp CLI.cpp CLI.h commands.h Server.cpp Server.h)