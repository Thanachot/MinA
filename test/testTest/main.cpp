#include <iostream>
#include <map>
#include "MinA/common/Minimizer.h"
#include "MinA/common/Result.h"
#include "TestFunction.cpp"
#include "MinA/algorithm/Simplex.h"
#include "MinA/algorithm/SimplexParallel.h"
#include <utility>

int main(void)
{
    if (!MPI::Is_initialized())
        MPI::Init();
    Minimizer mina;

    std::shared_ptr<Simplex> altest1(new Simplex(100));
    atlest1.setAdditionalInformation("name", "_Square");
    std::shared_ptr<SimplexParallel> altest2(new SimplexParallel(100));

    // Myfunction test1;
    std::shared_ptr<Myfunction> test1(new Myfunction);
    // Himmelblaufunction test2;
    // Boot_s_function test3;
    // Michalewicz_function test4;
    std::shared_ptr<Matthias_function> test5(new Matthias_function(5));
    // test5.reDimension(5);

    vector<double> stepSize;
    // stepSize.resize(test1->getParameterSize());
    //	std::cout<<"parametersize="<<test1.getParameterSize();
    // for(int i=0;i<test1.getParameterSize();i++)stepSize[i]=1.5;
    // std::cout<<stepSize.size();
    // altest1->setStepSize(stepSize);
    //	altest2->setStepSize(stepSize);
    cout << "function minimized value=" << mina.minimize(test5, altest1).result << endl;
    // cout<<"function minimized value="<<mina.minimize(test1,altest2).result<<endl;
    MPI_Finalize();
    return (0);
}
