#include <iostream>
#include <map>
#include "mpi.h"
#include "MinA/common/Minimizer.h"
#include "MinA/common/Result.h"
#include "TestFunction.cpp"
#include "MinA/algorithm/Simplex.h"
#include "MinA/algorithm/SimplexParallel.h"
#include <utility>

int main(int argc, char** argv)
{

    if (!MPI::Is_initialized())
        MPI::Init();
    Minimizer mina;
    int world_rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);

    std::shared_ptr<Simplex> altest1(new Simplex(4000));
    altest1->setFunctionName("Schwefel");
    std::shared_ptr<SimplexParallel> altest2(new SimplexParallel(5000));
    altest2->setFunctionName("Schwefel");

    std::shared_ptr<SquareFunction> test5(new SquareFunction(20));
    std::shared_ptr<McCormick_function> test2(new McCormick_function);
    std::shared_ptr<Matthias_function> test1(new Matthias_function(6));

    std::shared_ptr<Schwefel_function> test6(new Schwefel_function(3));

    vector<double> stepSize;
    stepSize.resize(test5->getParameterSize());

    for (int i = 0; i < test5->getParameterSize(); i++)
        stepSize[i] = 5 + 2 * i;
    altest1->setStepSize(stepSize);
    altest2->setStepSize(stepSize);
    if (world_rank == 0)
        cout << "function minimized value=" << mina.minimize(test5, altest2).result << endl;
    else
        auto rs = mina.minimize(test5, altest2);
    MPI_Finalize();
    return (0);
}
