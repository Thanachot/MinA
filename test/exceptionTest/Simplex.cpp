#include "Simplex.h"


Simplex::Simplex(){
	SetOptimizationAlgorithmParameter("alpha",1);
	SetOptimizationAlgorithmParameter("beta",0.5);
	SetOptimizationAlgorithmParameter("gramma",1);
	SetOptimizationAlgorithmParameter("town",.5);
	SetOptimizationAlgorithmParameter("stepsize",0.5);	
}


Simplex::~Simplex(){}
Result Simplex::algorithm(Functiontobeoptimized* start){
	
	int dimension=start->getparametersize();
	std::map <string, double> A[dimension+1];
	cout<<"dimension ="<<dimension<<endl;
		
	//Initial

	for (auto it : start->parameters)
		A[0][it.getname()]=it.getstartingPoint();


	for(int i=1;i<=dimension;i++){
		for ( std::set<Parameter>::iterator it=start->parameters.begin(); it!=start->parameters.end(); ++it){
			if(std::distance(start->parameters.begin(),it)==i-1)
				A[i][it->getname()]=A[0][it->getname()]+GetOptimizationAlgorithmParameter("stepsize");
			else
				A[i][it->getname()]=A[0][it->getname()];
		}

	}

	//showfunc(start,A);

	for(int jj=0;jj<100;jj++){

	//Sort
		std::map <string, double> M,Ar,Ac,Ae;
		std::sort(A,A+dimension+1,[start](std::map <string, double>  & a, std::map <string, double>  & b) -> bool{
		return start->evaluate(a) < start->evaluate(b) ; });
	cout<<"jj= "<<jj<<endl;
	showfunc(start,A);

	//Mean
		for(int i=0;i<dimension;i++){
			for(auto it : start->parameters){
				M[it.getname()]+=A[i][it.getname()];		
			}
		}
		for (auto it : start->parameters){
			M[it.getname()]/=dimension;		
		}

	//calculate
		//Ar
		for (auto it : start->parameters){
			Ar[it.getname()]=M[it.getname()]+GetOptimizationAlgorithmParameter("alpha")*(M[it.getname()]-A[dimension][it.getname()]);		
		}
		//Ae
		for (auto it : start->parameters){
			Ae[it.getname()]=Ar[it.getname()]+GetOptimizationAlgorithmParameter("gramma")*(Ar[it.getname()]-M[it.getname()]);		
		}
		//Ac
		if(start->evaluate(Ar)>start->evaluate(A[dimension])){
			for (auto it : start->parameters){
				Ac[it.getname()]=M[it.getname()]+GetOptimizationAlgorithmParameter("beta")*(A[dimension][it.getname()]-M[it.getname()]);			
			}
		}
		else{
			for (auto it : start->parameters){
				Ac[it.getname()]=M[it.getname()]+GetOptimizationAlgorithmParameter("beta")*(Ar[it.getname()]-M[it.getname()]);			
			}
		}

	//case//Update vertex step3
		//case 1
		if(start->evaluate(Ar)<start->evaluate(A[0])){
			if(start->evaluate(Ae)<start->evaluate(A[0]))
				for (auto it : start->parameters){A[dimension][it.getname()]=Ae[it.getname()];}
			else
				for (auto it : start->parameters){A[dimension][it.getname()]=Ar[it.getname()];}
		}	
		//case 2
		else if(start->evaluate(Ar)<start->evaluate(A[dimension-1]))
			for (auto it : start->parameters){A[dimension][it.getname()]=Ar[it.getname()];}
		//case 3
		else if(start->evaluate(Ac)<start->evaluate(A[dimension]))
			for (auto it : start->parameters){A[dimension][it.getname()]=Ac[it.getname()];}
		//case 4
		else{
			for(int i=1;i<=dimension;i++)
				for (auto it : start->parameters)	
					A[i][it.getname()]=GetOptimizationAlgorithmParameter("town")*A[0][it.getname()]+(1-GetOptimizationAlgorithmParameter("town"))*A[i][it.getname()];
		}

	//Return result
	}
	Result rs;
		for (auto it : start->parameters)
		rs.optimizationparameter["it.getname()"]=A[0][it.getname()];
		rs.result=start->evaluate(A[0]);
	return 	rs;
}

void Simplex::showfunc(Functiontobeoptimized* start,std::map <string, double> *para){
	int dimension=start->getparametersize();
	for(int i=0;i<=dimension;i++){
		for (std::map<string, double>::iterator it=para[i].begin(); it!=para[i].end(); ++it)
    			std::cout << it->first << "=" << it->second << " ";	
		cout<<"f(A"<<i<<")="<<start->evaluate(para[i])<<endl;
	}
}
