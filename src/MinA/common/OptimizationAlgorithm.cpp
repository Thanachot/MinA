#include "MinA/common/OptimizationAlgorithm.h"

OptimizationAlgorithm::~OptimizationAlgorithm()
{
    ;
}

void OptimizationAlgorithm::setOptimizationAlgorithmParameter(string nam, double nu)
{
    optimizationAlgorithmParameter[nam] = nu;
}

double OptimizationAlgorithm::getOptimizationAlgorithmParameter(string nam)
{
    return optimizationAlgorithmParameter[nam];
}

std::map<string, string> OptimizationAlgorithm::getAdditionalInformation() const
{
    return additionalInformation;
}

void OptimizationAlgorithm::setAdditionalInformation(
  std::map<string, string> additionalInformation_re)
{
    additionalInformation = additionalInformation_re;
}

void OptimizationAlgorithm::setAdditionalInformation(string s1, string s2)
{
    additionalInformation[s1] = s2;
}
