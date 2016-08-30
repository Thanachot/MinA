#ifndef MYFUNCTION_CPP
#include <iostream>
#include <map>
#include <cmath>
# define PI           3.14159265358979323846
#include "MinA/common/Functiontobeoptimized.h"

#define MYFUNCTION_CPP


class Myfunction:public Functiontobeoptimized{
    public:
	Myfunction(){
		Parameter first,second,third;

		first.setname("x");
		first.setstartingPoint(3);		
		second.setname("y");	
		second.setstartingPoint(4);
		third.setname("z");
		third.setstartingPoint(5);
		parameters.insert(second);
		parameters.insert(first);
		parameters.insert(third);
		
	};

	double getEvaluation(std::map <string, double> para){
	return pow(para["x"],2)+pow(para["y"],2)+pow(para["z"],2);
	}
 
};
class Himmelblaufunction:public Functiontobeoptimized{
    public:
	Himmelblaufunction(){
		Parameter first,second;
		first.setname("x");
		first.setstartingPoint(3);		
		second.setname("y");	
		second.setstartingPoint(4);
		parameters.insert(first);
		parameters.insert(second);
		//parameters.insert("x");
		//parameters.insert("y");
	};

	double getEvaluation(std::map <string, double> para){
	
	return (para["x"]*para["x"]+para["y"]-11)*(para["x"]*para["x"]+para["y"]-11)+(para["x"]+para["y"]*para["y"]-7)*(para["x"]+para["y"]*para["y"]-7);
	}
 
};
class Boot_s_function:public Functiontobeoptimized{
    public:
	Boot_s_function(){
		Parameter first,second;
		first.setname("x");
		first.setstartingPoint(3);		
		second.setname("y");	
		second.setstartingPoint(4);	
		parameters.insert(first);
		parameters.insert(second);
		
	};

	double getEvaluation(std::map <string, double> para){
	
	return pow((para["x"]+2*para["y"]-7),2)+pow((2*para["x"]+para["y"]-5),2);
	}
 
};

class Michalewicz_function:public Functiontobeoptimized{
    public:
	Michalewicz_function(){
		Parameter first,second,third;
		first.setname("x");
		first.setstartingPoint(3);		
		second.setname("y");	
		second.setstartingPoint(4);	
		//third.setname("z");
		parameters.insert(first);
		parameters.insert(second);
		//parameters.insert(third);
		
	};

	double getEvaluation(std::map <string, double> para){
	double fz;
	fz=-sin(para["x"])*pow(sin(pow(para["x"],2)/PI),2)-sin(para["x"])*pow(sin(2*pow(para["y"],2)/PI),2);//-sin(para["z"])*pow(sin(3*pow(para["z"],2)/PI),2);
	return fz;
	}
 
};

#endif 
