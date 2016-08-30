
#include <map>
#include <iostream>
#include<memory>
#include "MinA/common/Log.h"
#include "MinA/common/Functiontobeoptimized.h"
#include "MinA/common/OptimizationAlgorithm.h"
#include "MinA/common/Result.h"
#include "MinA/common/Result.h"
 using namespace std;
class Minimizer{
    public:
	

	Result minimize(Functiontobeoptimized *start,OptimizationAlgorithm *sim );
	Result operator()(Functiontobeoptimized *start,OptimizationAlgorithm *sim);
	private:

};


