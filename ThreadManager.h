//
// Created by pluto on 11.05.19.
//

#ifndef SKI_LIFT_THREADMANAGER_H
#define SKI_LIFT_THREADMANAGER_H


#include "ThreadManagerBase.h"

class ThreadManager : public ThreadManagerBase {
public:
    ThreadManager(int rank, int size);

};


#endif //SKI_LIFT_THREADMANAGER_H
