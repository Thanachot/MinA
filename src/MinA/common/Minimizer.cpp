#include "MinA/common/Minimizer.h"

Result Minimizer::minimize(std::shared_ptr<FunctionToBeOptimized> start,
                           std::shared_ptr<OptimizationAlgorithm> sim)
{
    return sim->algorithm(start);
}

Result Minimizer::operator()(std::shared_ptr<FunctionToBeOptimized> start,
                             std::shared_ptr<OptimizationAlgorithm> sim)
{
    return minimize(start, sim);
}
