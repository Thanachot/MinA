#ifndef MYFUNCTION_CPP
#include <iostream>
#include <map>
#include <cmath>
#include "MinA/common/FunctionToBeOptimized.h"

#define PI 3.14159265358979323846
#define MYFUNCTION_CPP
using namespace std;

class BadTen : public FunctionToBeOptimized {
    int i;

  public:
    BadTen() : i(0)
    {
        Parameter first, second, third;

        first.setName("x");
        first.setStartingPoint(3);
        second.setName("y");
        second.setStartingPoint(4);
        third.setName("z");
        third.setStartingPoint(5);
        parameters.insert(second);
        parameters.insert(first);
        parameters.insert(third);
    };

    double getEvaluation(map<string, double> para)
    {
        i++;
        if (!(i % 10))
            return (1.0 / 0.0);
        return pow(para["x"], 2) + pow(para["y"], 2) + pow(para["z"], 2);
    }
};

#endif
