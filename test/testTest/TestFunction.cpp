#ifndef MYFUNCTION_CPP
#include <iostream>
#include <map>
#include <cmath>
#define PI 3.14159265358979323846
#include "MinA/common/FunctionToBeOptimized.h"
#include <string>
#define MYFUNCTION_CPP

class Myfunction : public FunctionToBeOptimized {
  public:
    Myfunction()
    {
        Parameter first, second, third;

        first.setName("x");
        first.setStartingPoint(3);
        second.setName("y");
        second.setStartingPoint(4);
        third.setName("z");
        third.setStartingPoint(5);
        // first.setBoundaryLeft(-100);
        //	first.setBoundaryRight(100);
        // third.setBoundaryLeft(-100);
        // third.setBoundaryRight(100);
        // second.setBoundaryLeft(-100);
        //	second.setBoundaryRight(100);
        parameters.insert(second);
        parameters.insert(first);
        parameters.insert(third);
    };

    double getEvaluation(std::map<string, double> para)
    {
        return pow(para["x"], 2) + pow(para["y"], 2) + pow(para["z"], 2);
    }
};

class Himmelblaufunction : public FunctionToBeOptimized {
  public:
    Himmelblaufunction()
    {
        Parameter first, second;
        first.setName("x");
        first.setStartingPoint(3);
        second.setName("y");
        second.setStartingPoint(4);
        parameters.insert(first);
        parameters.insert(second);
    };

    double getEvaluation(std::map<string, double> para)
    {

        return (para["x"] * para["x"] + para["y"] - 11) * (para["x"] * para["x"] + para["y"] - 11) +
               (para["x"] + para["y"] * para["y"] - 7) * (para["x"] + para["y"] * para["y"] - 7);
    }
};

class Boot_s_function : public FunctionToBeOptimized {
  public:
    Boot_s_function()
    {
        Parameter first, second;
        first.setName("x");
        first.setStartingPoint(3);
        second.setName("y");
        second.setStartingPoint(4);
        parameters.insert(first);
        parameters.insert(second);
    };

    double getEvaluation(std::map<string, double> para)
    {

        return pow((para["x"] + 2 * para["y"] - 7), 2) + pow((2 * para["x"] + para["y"] - 5), 2);
    }
};

class Michalewicz_function : public FunctionToBeOptimized {
  public:
    Michalewicz_function()
    {
        Parameter first, second, third;
        first.setName("x");
        first.setStartingPoint(3);
        second.setName("y");
        second.setStartingPoint(4);
        parameters.insert(first);
        parameters.insert(second);
    };

    double getEvaluation(std::map<string, double> para)
    {
        double fz;
        fz = -sin(para["x"]) * pow(sin(pow(para["x"], 2) / PI), 2) -
             sin(para["x"]) * pow(sin(2 * pow(para["y"], 2) / PI), 2);
        return fz;
    };
};

class Matthias_function : public FunctionToBeOptimized {
  public:
    Matthias_function(int dimension)
    {
        alpha = 6;
        beta = 3;
        gramma = 1;
        eata = 1;
        for (int i = 0; i < dimension; i++) {
            Parameter par;
            par.setName("x" + std::to_string(i));
            par.setStartingPoint(-0.3);
            par.setBoundaryLeft(-100);
            par.setBoundaryRight(100);
            parameters.insert(par);
        }
    };
    void reDimension(int dimension)
    {
        for (int i = 0; i < dimension; i++) {
            Parameter par;
            par.setName("x" + std::to_string(i));
            par.setStartingPoint(3);
            par.setBoundaryLeft(-9.5);
            par.setBoundaryRight(9.5);
            parameters.insert(par);
        }
    };

    double getEvaluation(std::map<string, double> para)
    {
        double sum_xs = 0;
        for (auto it : parameters)
            sum_xs += pow(para[it.getName()], 2);
        double fz = alpha * pow(cos(beta * sum_xs), 2) + gramma * exp(eata * sum_xs);
        return fz;
    }
    double alpha, beta, gramma, eata;
};

#endif
