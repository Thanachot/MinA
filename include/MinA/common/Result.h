#ifndef RESULT_H
#include <iostream>
#include <map>
#include <string>

#define RESULT_H

class Result {
  public:
    std::map<std::string, double> optimizationParameter;
    std::map<std::string, std::string> additionalInformation;
    double result;
};
#endif
