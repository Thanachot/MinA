#ifndef SIMPLEX_H
#define SIMPLEX_H
#include <map>
#include <iostream>
#include <memory>
#include "MinA/common/OptimizationAlgorithm.h"
#include "MinA/common/Log.h"
#include <boost/serialization/utility.hpp>
#include <boost/serialization/map.hpp>
#include <boost/serialization/vector.hpp>
#include <boost/archive/text_oarchive.hpp>
#include <boost/archive/text_iarchive.hpp>
#include <string>
#include <fstream>
#include <cstdio>

typedef std::pair<std::map<std::string, double>, double> vertex;
typedef std::vector<std::pair<std::map<std::string, double>, double>> vertexVector;
class Simplex : public OptimizationAlgorithm {
  public:
    Simplex(int stop = 100);
    ~Simplex();
    virtual Result algorithm(std::shared_ptr<FunctionToBeOptimized> start);
    void showVertex(vertexVector& para);
    void showVertex(vertex& para);
    void checkBoundaryCondition(vertex& A);
    void setStepSize(vector<double> s);
    void setStepSize();
    void setStoppingIteration(int n);
    void setFunctionName(string name);
    string getFunctionName();

  protected:
    int dimension;
    std::shared_ptr<FunctionToBeOptimized> function;
    std::vector<double> stepSize;
    bool checkStoppingCondition();
    void push(vertex& a, vertex& b);
    void pushResult(Result& rs, vertex& A);
    void createInitialVertex(vertexVector& A);
    void calculateM(vertexVector& A, vertex& M, int world_size);
    void calculateAr(vertex& Ar, vertex& M, vertex& Aj);
    void calculateAe(vertex& Ae, vertex& M, vertex& Ar);
    virtual void calculateAc(vertex& Ac, vertex& M, vertex& Ajp);
    void createNewVertex(vertexVector& A);
    void calculateNewPoint(vertex& Anew, vertex& Ap, vertex& A0);
    void save() const;
    void restore();
    vertexVector Acopy;
    int currentIteration;
    double difference;
    int stoppingIteration;
    string FunctionName;

};

#endif
