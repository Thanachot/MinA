#ifndef FUNCTIONTOBEOPTIMIZED_H
#define FUNCTIONTOBEOPTIMIZED_H
#include <map>
#include <iostream>
#include <set>
#include "MinA/common/Parameter.h"
#include <algorithm>
#include <iterator>
#include <memory>
#include <sstream>
#include "MinA/common/Exception.h"
#include "MinA/common/Log.h"

enum handle_arith { MinA_EHIGH, MinA_ETHROW, MinA_EWARN };
class FunctionToBeOptimized {
  private:
    handle_arith handle;

  public:
    FunctionToBeOptimized() : handle(MinA_ETHROW)
    {
    }
    void setHandle(handle_arith ha)
    {
        handle = ha;
    }
    double evaluate(std::map<std::string, double> para)
    {
        double result;
        try {
            result = getEvaluation(para);
            if (!std::isfinite(result))
                throw Arithmetical_Exception(result);
        }
        catch (Arithmetical_Exception& ae) {
          std::stringstream err;
            switch (handle) {
                case MinA_EHIGH:
                    err << "Exception caught:" << ae.what()
                        << ". Returning very high Value (10E+7) \n";
                    Log::getLog() << err.str();
                    Log::getLog().flushLog();
                    result = 10000000.0;
                    break;
                case MinA_ETHROW:
                    err << "Exception caught:" << ae.what() << ". Rethrowing Exception";
                    throw;
                    break;
                case MinA_EWARN:
                    err << "Exception caught:" << ae.what() << ". Printing out this warning";
                    result = ae.getDoubleValue();
                    break;
            }
        }
        return result;
    }
    virtual double getEvaluation(std::map<std::string, double> para) = 0;
    int getParameterSize()
    {
        return parameters.size();
    }
    std::set<Parameter> parameters;

};

#endif
