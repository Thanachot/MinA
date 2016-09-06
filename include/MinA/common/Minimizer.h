#include <map>
#include <iostream>
#include <memory>
#include "MinA/common/Log.h"
#include "MinA/common/FunctionToBeOptimized.h"
#include "MinA/common/OptimizationAlgorithm.h"
#include "MinA/common/Result.h"
#include "MinA/common/Result.h"
using namespace std;

class Minimizer {
  public:
    Result minimize(std::shared_ptr<FunctionToBeOptimized> start,
                    std::shared_ptr<OptimizationAlgorithm> sim);
    Result operator()(std::shared_ptr<FunctionToBeOptimized> start,
                      std::shared_ptr<OptimizationAlgorithm> sim);

};
