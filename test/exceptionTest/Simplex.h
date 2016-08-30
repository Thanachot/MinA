#ifndef SIMPLEX_H
#define SIMPLEX_H
#include <map>
#include <iostream>
#include<memory>
#include "MinA/common/OptimizationAlgorithm.h"


class Simplex:public OptimizationAlgorithm{
    public:
	Simplex();
	~Simplex();
	Result algorithm(Functiontobeoptimized* start);
	void showfunc(Functiontobeoptimized* start,std::map <string, double> *para);

  std::shared_ptr<std::map <string, double>> AA;
	std::allocator<std::map <string, double>> alloc;
};

#endif
