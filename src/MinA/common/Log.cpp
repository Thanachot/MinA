#include "MinA/common/Log.h"
void Log::pushMessage(std::string msg){
	{
	std::lock_guard<std::mutex>lck{mmutex};
	mqueue.push(msg);
	mcond.notify_one();
	}
}
void Log::startLogThread(){
	while(!stoplog){
		std::unique_lock<std::mutex>lck{mmutex};
		mcond.wait(lck);
		if(!mqueue.empty()){
			std::string msg=mqueue.front();	
			mqueue.pop();
			lck.unlock();
			std::cout<< msg;
		}
		else{	
			lck.unlock();
		}	
		if(flushlog){
		std::cout<<std::flush;
		flushlog=false;
		}
	}
	//try to print out the complete queue one last time	
	std::lock_guard<std::mutex>lck{mmutex};
	while(!mqueue.empty()){
		std::cout<<mqueue.front();	
		mqueue.pop();
	}
	std::cout <<std::flush;
}
void Log::setLogOutput(std::string filename){
	std::cout<< "redirecting stdout & stdin to "<<filename<<std::endl;
	if(std::freopen(filename.c_str(),"w",stdout)==NULL){std::bad_alloc excpt;throw excpt;}
	if(std::freopen(filename.c_str(),"w",stderr)==NULL){std::bad_alloc excpt;throw excpt;}
}
void Log::flushLog(){
	flushlog=true;
	while(flushlog==true){
		mcond.notify_one();
		std::this_thread::yield();
	}
}
Log::Log(){ 
	flushlog=stoplog=false;
	logThread=new std::thread(&Log::startLogThread,this);

}
Log::~Log(){
	stoplog=true;
	mcond.notify_all();
	logThread->join();
}
Log Log::MinALog;
