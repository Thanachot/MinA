#pragma once
#include <thread>
#include <iostream>
#include <mutex>
#include <condition_variable>
#include <queue>
#include <memory>
#include <fstream>
#include <exception>

class Log {
  public:
    void pushMessage(std::string msg);
    void operator<<(std::string msg)
    {
        pushMessage(msg);
    }
    void setLogOutput(std::string filename);
    static Log& getLog()
    {
        return MinALog;
    }
    void flushLog();
    ~Log();

  private:
    bool stoplog, flushlog;
    std::thread* logThread;
    static Log MinALog;
    Log();
    void startLogThread();
    std::queue<std::string> mqueue;
    std::mutex mmutex;
    std::condition_variable mcond;
    std::mutex fmutex;
    std::condition_variable flushcond;
};
