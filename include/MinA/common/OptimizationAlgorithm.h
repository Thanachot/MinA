#ifndef OPTIMiZATIONALGORITHM_H
#define OPTIMiZATIONALGORITHM_H
#include <map>
#include <iostream>
#include <memory>
#include "MinA/common/FunctionToBeOptimized.h"
#include "MinA/common/Result.h"
using namespace std;

class OptimizationAlgorithm {
  public:
    virtual ~OptimizationAlgorithm();
    virtual Result algorithm(std::shared_ptr<FunctionToBeOptimized> start) = 0;

    void setOptimizationAlgorithmParameter(string nam, double nu);
    double getOptimizationAlgorithmParameter(string nam);
    virtual void save() const = 0;
    virtual void restore() = 0;
    std::map<string, string> getAdditionalInformation() const;
    void setAdditionalInformation(std::map<string, string> additionalInformation_re);
    void setAdditionalInformation(string s1, string s2);

  private:
    std::map<string, double> optimizationAlgorithmParameter;
    std::map<string, string> additionalInformation;
};

#endif
