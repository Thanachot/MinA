#ifndef SIMPLEXPARALLEL_H
#define SIMPLEXPARALLEL_H
#include <map>
#include <iostream>
#include <memory>
#include "mpi.h"
#include <boost/serialization/vector.hpp>
#include "MinA/common/OptimizationAlgorithm.h"
#include <boost/serialization/utility.hpp>
#include <boost/serialization/map.hpp>
#include <boost/archive/text_oarchive.hpp>
#include <boost/archive/text_iarchive.hpp>
#include "MinA/algorithm/Simplex.h"

typedef std::pair<std::map<std::string, double>, double> vertex;
typedef std::vector<std::pair<std::map<std::string, double>, double>> vertexVector;

class SimplexParallel : public Simplex {
  public:
    SimplexParallel(int stop = 100);
    ~SimplexParallel();
    Result algorithm(std::shared_ptr<FunctionToBeOptimized> start);
    void sendVertex(vertex& A, int receiver, int tag);
    vertex receiveVertex(int sender, int tag);
    void calculateAc(vertex& Ac, vertex& M, vertex& Ajp);

};

#endif
