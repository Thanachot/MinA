#include "MinA/algorithm/Simplex.h"

Simplex::Simplex(int stop)
{
    stoppingIteration = stop;
    setOptimizationAlgorithmParameter("alpha", 1);
    setOptimizationAlgorithmParameter("beta", 0.5);
    setOptimizationAlgorithmParameter("gramma", 1);
    setOptimizationAlgorithmParameter("tau", 0.5);
    setAdditionalInformation("checkboundary", "");

    currentIteration = 0;
}

Simplex::~Simplex()
{
}

Result Simplex::algorithm(std::shared_ptr<FunctionToBeOptimized> start)
{

    function = start;
    dimension = start->getParameterSize();
    setStepSize();

    vertexVector A;
    // restore();
    if (Acopy.empty()) {
        A.resize(dimension + 1);
        createInitialVertex(A); // Initial
    }
    else {
        A = Acopy;
    }
    cout << "Start with current loop= " << currentIteration << endl;
    for (int i = 0; i <= dimension; i++)
        checkBoundaryCondition(A[i]);
    while (checkStoppingCondition()) {

        vertex Ar, Ac, Ae, Anew, Ap;
        for (int i = 0; i <= dimension; i++) {
            A[i].second = start->getEvaluation(A[i].first);
        }

        std::sort(A.begin(), A.end(),
                  [](vertex& a, vertex& b) -> bool { return a.second < b.second; }); // Sort
        cout << "loop= " << currentIteration << endl;
        showVertex(A);
        vertex M;
        calculateM(A, M, 2); // Mean

        int check = 0;
        calculateAr(Ar, M, A[dimension]); // Ar
        if (Ar.second < A[0].second) {
            calculateAe(Ae, M, Ar); // Ae
            if (Ae.second < A[0].second) {
                Anew = Ae;
                check = 1;
            }
            else {
                Anew = Ar;
                check = 2;
            }
        } // case 1
        else if (Ar.second < A[dimension - 1].second) {
            Anew = Ar;
            check = 3;
        } // case 2
        else {
            if (Ar.second < A[dimension].second) {
                Ap = Ar;
            }
            else {
                Ap = A[dimension];
            } // Ac
            calculateAc(Ac, M, Ar);
            if (Ac.second < A[dimension].second) {
                Anew = Ac;
                check = 4;
            }
        } // case 3
        if (check == 0) {
            for (int i = 1; i <= dimension; i++)
                calculateNewPoint(Anew, Ap, A[0]);
        }
        showVertex(Ap);
        cout << "check=" << check << endl;
        push(A[dimension], Anew);
        checkBoundaryCondition(A[dimension]);
        Acopy = A;
        // save();

        ofstream dataFile;
        dataFile.open("Simplex" + getFunctionName(), std::ios::app);
        dataFile << "Iteration= " << currentIteration << "	f(A0)= " << A[0].second << "	";
        for (std::map<string, double>::iterator it = A[0].first.begin(); it != A[0].first.end();
             ++it)
            dataFile << it->first << " = " << it->second << "	";
        dataFile << "\n";
        dataFile.close();
    }

    // Return result*/
    Result rs;
    pushResult(rs, A[0]);
    return rs;
}

void Simplex::setStepSize()
{
    if (stepSize.empty()) {
        stepSize.resize(dimension);
        for (int i = 0; i < dimension; i++)
            stepSize[i] = 0.5;
    }
}

void Simplex::setStepSize(vector<double> s)
{

    stepSize = s;
}

void Simplex::setFunctionName(string name)
{
    FunctionName = name;
}

string Simplex::getFunctionName()
{
    return FunctionName;
}

void Simplex::setStoppingIteration(int n)
{

    stoppingIteration = n;
}

bool Simplex::checkStoppingCondition()
{

    if (currentIteration >= stoppingIteration) {
        return false;
    }

    if (getAdditionalInformation()["checkboundary"] != "") {
        std::cout << getAdditionalInformation()["checkboundary"] << "\n";
        return false;
    }

    currentIteration++;
    return true;
}

void Simplex::checkBoundaryCondition(vertex& A)
{
    int check = 0;

    for (auto it : function->parameters) {
        if (it.getBoundaryLeft() != nullptr)
            if (A.first[it.getName()] < *it.getBoundaryLeft())
                A.first[it.getName()] = *it.getBoundaryLeft();

        if (it.getBoundaryRight() != nullptr)
            if (A.first[it.getName()] > *it.getBoundaryRight())
                A.first[it.getName()] = *it.getBoundaryRight();
    }
    //	if(check!=0){setAdditionalInformation("checkboundary","out of boundary");}
}

void Simplex::showVertex(vertexVector& para)
{
    int ii = 0;
    for (auto it : para) {
        for (auto itx : it.first) {
            std::cout << itx.first << "=" << itx.second << " ";
        }
        cout << "f(A" << ii << ")=" << it.second << "\n";
        ii++;
    }
}

void Simplex::showVertex(vertex& para)
{

    for (std::map<string, double>::iterator it = para.first.begin(); it != para.first.end(); ++it)
        std::cout << it->first << "=" << it->second << " ";
    cout << "f()=" << para.second << endl;
}

void Simplex::push(vertex& a, vertex& b)
{
    for (auto it : function->parameters)
        a.first[it.getName()] = b.first[it.getName()];
    a.second = b.second;
}

void Simplex::pushResult(Result& rs, vertex& A)
{
    for (auto it : function->parameters)
        rs.optimizationParameter["it.getName()"] = A.first[it.getName()];
    rs.result = A.second;
}

void Simplex::createInitialVertex(vertexVector& A)
{
    for (auto it : function->parameters)
        A[0].first[it.getName()] = *(it.getStartingPoint());

    for (int i = 1; i <= dimension; i++) {
        for (std::set<Parameter>::iterator it = function->parameters.begin();
             it != function->parameters.end(); ++it) {
            if (std::distance(function->parameters.begin(), it) == i - 1)
                A[i].first[it->getName()] = A[0].first[it->getName()] + stepSize[i - 1];
            else
                A[i].first[it->getName()] = A[0].first[it->getName()];
        }
    }
}

void Simplex::calculateM(vertexVector& A, vertex& M, int world_size)
{
    for (int i = 0; i <= dimension - world_size + 1; i++) {
        for (auto it : function->parameters) {
            M.first[it.getName()] += A[i].first[it.getName()];
        }
    }
    for (auto it : function->parameters) {
        M.first[it.getName()] /= (dimension - world_size + 2);
    }
}

void Simplex::calculateAr(vertex& Ar, vertex& M, vertex& Aj)
{
    for (auto it : function->parameters)
        Ar.first[it.getName()] = M.first[it.getName()] +
                                 getOptimizationAlgorithmParameter("alpha") *
                                   (M.first[it.getName()] - Aj.first[it.getName()]);
    Ar.second = function->evaluate(Ar.first);
}

void Simplex::calculateAe(vertex& Ae, vertex& M, vertex& Ar)
{
    for (auto it : function->parameters)
        Ae.first[it.getName()] = Ar.first[it.getName()] +
                                 getOptimizationAlgorithmParameter("gramma") *
                                   (Ar.first[it.getName()] - M.first[it.getName()]);
    Ae.second = function->evaluate(Ae.first);
}

void Simplex::calculateAc(vertex& Ac, vertex& M, vertex& Ajp)
{
    for (auto it : function->parameters)
        Ac.first[it.getName()] = M.first[it.getName()] +
                                 getOptimizationAlgorithmParameter("beta") *
                                   (Ajp.first[it.getName()] - M.first[it.getName()]);
    Ac.second = function->evaluate(Ac.first);
}

void Simplex::calculateNewPoint(vertex& Anew, vertex& Ap, vertex& A0)
{
    for (auto it : function->parameters)
        Anew.first[it.getName()] =
          getOptimizationAlgorithmParameter("tau") * A0.first[it.getName()] +
          (1 - getOptimizationAlgorithmParameter("tau")) * Ap.first[it.getName()];
}

void Simplex::createNewVertex(vertexVector& A)
{
    for (int i = 1; i <= dimension; i++) {
        cout << i << "before----->" << getOptimizationAlgorithmParameter("tau");
        showVertex(A[i]);
        for (auto it : function->parameters)
            A[i].first[it.getName()] =
              getOptimizationAlgorithmParameter("tau") * A[0].first[it.getName()] +
              (1 - getOptimizationAlgorithmParameter("tau")) * A[i].first[it.getName()];

        cout << endl;
        cout << i << "After----->";
        showVertex(A[i]);
    }
}

void Simplex::restore()
{
    std::ifstream f(".Simplex.save");
    if (f.good()) {
        Log::getLog() << "Loading from file .Simplex.save\n";
        Log::getLog().flushLog();
        std::ifstream infile(".Simplex.save");
        boost::archive::text_iarchive ia(infile);
        ia >> Acopy;
        ia >> currentIteration;
    }
}

void Simplex::save() const
{
    if (!Acopy.empty()) {
        std::ofstream outfile(".Simplex.save");
        boost::archive::text_oarchive oa(outfile);
        oa << Acopy;
        oa << currentIteration;
    }
}
