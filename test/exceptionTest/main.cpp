#include <iostream>
#include <map>
#include "MinA/common/Minimizer.h"
#include "MinA/common/Result.h"
#include "myfunction.cpp"
#include "Simplex.h"
#include <utility>

int main(void){
        //bt.setHandle(KILL);     
	BadTen bt;
	Simplex altest;
	Minimizer mina;
        int count=0;
        for(int i=0;i<10;i++){
          try{
            mina.minimize(&bt,&altest);
          }catch(Arithmetical_Exception & ae){
            count++;
          }
        } 
        std::cout<<count<<std::endl;
    return!(count ==10);
}
