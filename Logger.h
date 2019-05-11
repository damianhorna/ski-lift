#ifndef SKI_LIFT_LOGGER_H
#define SKI_LIFT_LOGGER_H

#include <iostream>
#include "ThreadManager.h"
#include "easylogging++.h"

using namespace std;
class Logger {
public:
    static string const main_thread;
    static string const rec;
    static string const ack;
    static string const rel;
    static string const req;

    static void afterReceiveAck(ThreadManagerBase threadManager, int i);

    static void queueUpdate(ThreadManagerBase manager);

private:
    Logger(){}

};


#endif //SKI_LIFT_LOGGER_H
