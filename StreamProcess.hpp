//
// Created by chloridecull on 9/29/15.
//

#ifndef MEDIACOURIER_STREAMPROCESS_HPP
#define MEDIACOURIER_STREAMPROCESS_HPP
#include <string>
#include <list>
#include <unordered_map>
#include <atomic>

class StreamProcess {
private:
    std::atomic<bool> killThread;
    int connid;
    std::list<StreamProcess*>* streamlists;
    std::unordered_map<std::string, std::string>* keyusermap;
    void PassMasterStream(int outside, int ffmpeg, int ffmpegstderr);
public:
    std::string StreamName;
    StreamProcess(int Connection, std::list<StreamProcess*> *ExistingStreams,
                  std::unordered_map<std::string, std::string>* KUMap);
    void RunStreamProcesses();
};


#endif //MEDIACOURIER_STREAMPROCESS_HPP
