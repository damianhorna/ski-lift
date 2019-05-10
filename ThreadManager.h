#ifndef SKI_LIFT_THREADMANAGER_H
#define SKI_LIFT_THREADMANAGER_H


#include "ThreadManagerBase.h"

#include <mpi.h>

class ThreadManager : public ThreadManagerBase {
public:
    ThreadManager(int rank, int size);

    void processRequestMessage(int receivedMessage[], MPI_Status receivedMessageStatus);

    void processAckMessage(MPI_Status receivedMessageStatus);

    void processReleaseMessage(MPI_Status receivedMessageStatus);
};


#endif //SKI_LIFT_THREADMANAGER_H
