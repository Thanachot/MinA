#include "MinA/algorithm/Simplex.h"
using namespace std;

constexpr int columnWidth = 8;

Simplex::Simplex(int stop)
{
    stoppingIteration = stop;
    setMetaParameter("alpha", 1);
    setMetaParameter("beta", 0.5);
    setMetaParameter("gamma", 1);
    setMetaParameter("tau", 0.5);
    setAdditionalInformation("checkboundary", "");

    currentIteration = 0;
}

Simplex::~Simplex() {}

/**
 * Implementation of the serial Nelder-Mead Simplex on the cost function passed as argument
 * The original article describing the algorithm is:
 * ``A simplex method for function minimization'', J. Nelder, and R. Mead. Computer Journal (1965).
 * Good articles describing its flow w.r.t. its parallel implementation are:
 * ``A Parallel Implementation of the Simplex Function Minimization Routine'', D. Lee, and M.
 * Wiswall. Computational Economics (2007)
 * ``Nelder-Mead Simplex Optimization Routine for Large-Scale Problems: A Distributed Memory
 * Implementation'', K. Klein, and J. Neira. Computational Economics (2013)
 */
Result Simplex::algorithm(shared_ptr<FunctionToBeOptimized> costFunction)
{

    mFunction = costFunction;
    mDimension = costFunction->getParSpaceDim();
    setStepSize();

    verticesVector A;
    // restore();
    if (Acopy.empty()) {
        A.resize(mDimension + 1);
        initializeVertices(A); // Initial
    }
    else {
        A = Acopy;
    }
    cout << "Start. Current iteration = " << currentIteration << endl;
    for (int i = 0; i <= mDimension; i++)
        checkBoundaryCondition(A[i]);
    while (checkStoppingCondition()) {

        vertex Ar, Ac, Ae, Anew, Ap; // reflection, contraction, extension, shrinked point
        for (int iVertex = 0; iVertex <= mDimension; ++iVertex)
            A[iVertex].second = costFunction->getEvaluation(A[iVertex].first);

        sort(A.begin(), A.end(),
             [](vertex& a, vertex& b) -> bool { return a.second < b.second; }); // Sort
        ofstream verticesFile;
        string outFile_vertices("nmSimplex_" + getFunctionName() + "_Vertices");
        verticesFile.open(outFile_vertices, ios::app);
        verticesFile << "  Iteration: " << currentIteration << endl;
        printOutVertices(A, verticesFile);
        verticesFile << endl;
        verticesFile.close();

        // get centroid of all vertices but the worst one
        vertex M = getCentroid(A, 1);

        int check = 0;
        Ar = getReflectedPoint(M, A[mDimension]);
        if (Ar.second < A[0].second) {
            Ae = getExtendedPoint(M, Ar);
            if (Ae.second < A[0].second) {
                Anew = Ae;
                check = 1;
            }
            else {
                Anew = Ar;
                check = 2;
            }
        } // case 1
        else if (Ar.second < A[mDimension - 1].second) {
            Anew = Ar;
            check = 3;
        } // case 2
        else {
            if (Ar.second < A[mDimension].second) {
                Ap = Ar;
            }
            else {
                Ap = A[mDimension];
            } // Ac
            Ac = getContractedPoint(M, Ap);
            if (Ac.second < A[mDimension].second) {
                Anew = Ac;
                check = 4;
            }
        } // case 3
        if (check == 0) {
            for (int i = 1; i <= mDimension; i++)
                Anew = getShrinkedPoint(Ap, A[0]);
        }

        cout << "check=" << check << endl;
        A[mDimension] = Anew;
        checkBoundaryCondition(A[mDimension]);
        Acopy = A;
        // save();

        ofstream fValueFile;
        string outFile_fValue("nmSimplex_" + getFunctionName() + "_fValue");
        fValueFile.open(outFile_fValue, ios::app);
        fValueFile << "Iteration " << setw(5) << currentIteration << "   ";
        printOutVertex(A[0], "A[0]", fValueFile);
        fValueFile.close();
    }

    Result result;
    pushResult(result, A[0]);
    return result;
}

/**
 * If the vector of step sizes is not already set, set it in each dimension to equal
 * the minimum distance between the starting point and the left/right boundary
 */
void Simplex::setStepSize()
{
    if (stepSize.empty()) {
        stepSize.resize(mDimension);
        for (int iPar = 0; iPar < mDimension; ++iPar)
            stepSize[iPar] = min(mFunction->mParameters[iPar].getStartingValue() -
                                   mFunction->mParameters[iPar].getLeftBoundary(),
                                 mFunction->mParameters[iPar].getRightBoundary() -
                                   mFunction->mParameters[iPar].getStartingValue()) /
                             2.;
    }
}

/**
 * Set the vector of step sizes to the vector passed as argument
 * @param s Step sizes to genereate nDimension vertices from the initial point
 */
void Simplex::setStepSize(vector<double> s) { stepSize = s; }

/**
 * Set the function name to the argument
 * @param name The string to be assigned as function name
 */
void Simplex::setFunctionName(string name) { FunctionName = name; }

/**
 * Get the function name
 */
string Simplex::getFunctionName() { return FunctionName; }

void Simplex::setStoppingIteration(int n) { stoppingIteration = n; }

bool Simplex::checkStoppingCondition()
{

    if (currentIteration >= stoppingIteration) {
        return false;
    }

    if (getAdditionalInformation()["checkboundary"] != "") {
        cout << getAdditionalInformation()["checkboundary"] << "\n";
        return false;
    }

    currentIteration++;
    return true;
}

void Simplex::checkBoundaryCondition(vertex& A)
{
    // Check that vertex A is within the corresponding declared boundaries
    // If not, put it on the boundary

    for (int iPar = 0; iPar < mDimension; ++iPar) {
        if (A.first[iPar] < mFunction->mParameters[iPar].getLeftBoundary())
            A.first[iPar] = mFunction->mParameters[iPar].getLeftBoundary();

        if (A.first[iPar] > mFunction->mParameters[iPar].getRightBoundary())
            A.first[iPar] = mFunction->mParameters[iPar].getRightBoundary();
    }
    //	if(check!=0){setAdditionalInformation("checkboundary","out of boundary");}
}

void Simplex::printOutVertices(verticesVector& simplexVertices, ostream& outStream)
{
    // Print out parameter values and function value at each vertex
    outStream << setw(columnWidth) << "";
    for (int iPar = 0; iPar < mDimension; ++iPar)
        outStream << setw(columnWidth) << mFunction->mParameters[iPar].getName();
    outStream << endl;
    for (int iVertex = 0; iVertex <= mDimension; ++iVertex) {
        string vertexName{ "A[" };
        vertexName += to_string(iVertex);
        vertexName += "]";
        printOutVertex(simplexVertices[iVertex], vertexName, outStream);
    }
}

void Simplex::printOutVertex(vertex& simplexVertex, string vertexName, ostream& outStream)
{
    // Print out parameters and function value at vertex
    // outStream << setw(columnWidth - 2) << vertexName << ":  ";
    outStream << setw(columnWidth) << vertexName;
    outStream << setprecision(2);
    for (int iPar = 0; iPar < mDimension; ++iPar) {
        outStream << setw(columnWidth) << simplexVertex.first[iPar];
    }
    outStream << "   f(" << vertexName << ")=" << simplexVertex.second << endl;
}

void Simplex::pushResult(Result& rs, vertex& A)
{
    for (int iPar = 0; iPar < mDimension; ++iPar) {
        rs.optimizationParameter[mFunction->mParameters[iPar].getName()] = A.first[iPar];
    }
    rs.result = A.second;
}

void Simplex::initializeVertices(verticesVector& A)
{
    // Initialize the first vertex to the starting parameters
    A[0].first.resize(mDimension);
    for (int iPar = 0; iPar < mDimension; ++iPar) {
        A[0].first[iPar] = mFunction->mParameters[iPar].getStartingValue();
    }

    // Initialize the remaining mDimension vertices
    // by adding i-th stepSize in i-th direction to A[0]
    for (int iVertex = 1; iVertex <= mDimension; ++iVertex) {
        A[iVertex].first.resize(mDimension);
        for (int iPar = 0; iPar < mDimension; ++iPar) {
            if (iPar == iVertex - 1)
                A[iVertex].first[iPar] = A[0].first[iPar] + stepSize[iVertex - 1];
            else
                A[iVertex].first[iPar] = A[0].first[iPar];
        }
    }
}

vertex Simplex::getCentroid(verticesVector& A, int nExcluded)
{
    // Return centroid M of all vertices, but the nExcluded worst ones, that is the centroid of
    // the best (mDimension + 1 - nExcluded) vertices of the vector of vertices A
    // In the serial case the centroid is calculated on mDimension vertices, that is all but
    // the worst one.
    // The vertices of A are supposed to be already ordered by function value, from best to worst
    //
    vertex M;
    M.first.resize(mDimension);
    for (int iPar = 0; iPar < mDimension; ++iPar) {
        for (int iVertex = 0; iVertex < (mDimension + 1 - nExcluded); ++iVertex) {
            M.first[iPar] += A[iVertex].first[iPar];
        }
        M.first[iPar] /= (mDimension + 1 - nExcluded);
    }
    M.second = mFunction->evaluate(M.first);
    return M;
}

vertex Simplex::getReflectedPoint(vertex& M, vertex& Aj)
{
    // Return reflection point Ar of initial point Aj w.r.t. centroid M of remaining points
    // Ar = M + alpha * (M - Aj)
    //
    vertex Ar;
    Ar.first.resize(mDimension);
    for (int iPar = 0; iPar < mDimension; ++iPar)
        Ar.first[iPar] =
          M.first[iPar] + getMetaParameter("alpha") * (M.first[iPar] - Aj.first[iPar]);
    Ar.second = mFunction->evaluate(Ar.first);
    return Ar;
}

vertex Simplex::getExtendedPoint(vertex& M, vertex& Ar)
{
    // Return extension point Ae of initial point Ar w.r.t. centroid M
    // Ae = Ar + gamma * (Ar - M)
    //
    vertex Ae;
    Ae.first.resize(mDimension);
    for (int iPar = 0; iPar < mDimension; ++iPar)
        Ae.first[iPar] =
          Ar.first[iPar] + getMetaParameter("gamma") * (Ar.first[iPar] - M.first[iPar]);
    Ae.second = mFunction->evaluate(Ae.first);
    return Ae;
}

vertex Simplex::getContractedPoint(vertex& M, vertex& Ajp)
{
    // Return contraction point Ac of initial point Ajp w.r.t. centroid M
    // Ac = M + beta * (Ajp - M)
    //
    vertex Ac;
    Ac.first.resize(mDimension);
    for (int iPar = 0; iPar < mDimension; ++iPar)
        Ac.first[iPar] =
          M.first[iPar] + getMetaParameter("beta") * (Ajp.first[iPar] - M.first[iPar]);
    Ac.second = mFunction->evaluate(Ac.first);
    return Ac;
}

vertex Simplex::getShrinkedPoint(vertex& Ap, vertex& A0)
{
    // Return shrinked point Anew of initial point Ap w.r.t. fixed (best) point A0
    // Anew = tau * A0 + (1-tau) * Ap
    //
    vertex Anew;
    Anew.first.resize(mDimension);
    for (int iPar = 0; iPar < mDimension; ++iPar)
        Anew.first[iPar] =
          getMetaParameter("tau") * A0.first[iPar] + (1 - getMetaParameter("tau")) * Ap.first[iPar];
    Anew.second = mFunction->evaluate(Anew.first);
    return Anew;
}

void Simplex::restore()
{
    ifstream f(".Simplex.save");
    if (f.good()) {
        Log::getLog() << "Loading from file .Simplex.save\n";
        Log::getLog().flushLog();
        ifstream infile(".Simplex.save");
        boost::archive::text_iarchive ia(infile);
        ia >> Acopy;
        ia >> currentIteration;
    }
}

void Simplex::save() const
{
    if (!Acopy.empty()) {
        ofstream outfile(".Simplex.save");
        boost::archive::text_oarchive oa(outfile);
        oa << Acopy;
        oa << currentIteration;
    }
}

double Simplex::getSimplexSize(verticesVector& simplexVertices)
{
    // calculate the simplex size as the average vertex-centroid distance
    double simplexSize = 0.;
    vertex M = getCentroid(simplexVertices, 0); // the centroid of the simplex, no vertex excluded

    for (int iVertex = 0; iVertex <= mDimension; ++iVertex) { // loop over vertices
        double vertexCentroidDistance = 0.;
        for (int iPar = 0; iPar < mDimension; ++iPar) { // loop over vertex' parameters
            vertexCentroidDistance += pow(M.first[iPar] - simplexVertices[iVertex].first[iPar], 2);
        }
        simplexSize += sqrt(vertexCentroidDistance);
    }
    simplexSize /= (mDimension + 1);
    return simplexSize;
}
