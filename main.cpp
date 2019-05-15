#include "mpi.h"
#include <iostream>
#include <time.h>
#include <algorithm>
#include <vector>
#include <numeric>
#include <zconf.h>
#include <thread>

#include "easylogging++.h"
#include "ThreadManager.h"
#include "Logger.h"

using namespace std;

INITIALIZE_EASYLOGGINGPP

void mainThread(ThreadManagerBase &threadManager) {

    while (true) {
        cout<<"Przed lockiem"<<endl;
        threadManager.lock();
        threadManager.increaseClock();
        int *msg = threadManager.constructMessage();
        threadManager.sendMessageForEverybody(msg, REQUEST);
        threadManager.addOwnRequestToQueue();

        bool canGoOnLift = false;
        while (!canGoOnLift) {
            if (threadManager.isEnoughPlaceOnLift() && threadManager.isEveryAck()) {
                canGoOnLift = true;
            } else {
                LOG(INFO) << MAIN_MESS << threadManager.toString() << "Waiting";
                threadManager.wait();
            }
        }
        threadManager.unlock();
        threadManager.clearAcks();

        LOG(INFO) << MAIN_MESS << threadManager.toString() << "On lift";
        sleep(7+rand()%5);

        threadManager.increaseClock();
        msg = threadManager.constructMessage();

        threadManager.lock();
        threadManager.sendMessageForEverybody(msg, REALEASE);
        threadManager.removeYourselfFromQueue();
        threadManager.unlock();

        threadManager.sleepRandomTime();
    }
}

void receivingThread(ThreadManager &threadManager) {

    while (true) {
        MPI_Status receivedMessageStatus;
        int receivedMessage[ThreadManagerBase::MSG_SIZE];
        MPI_Recv(receivedMessage, ThreadManagerBase::MSG_SIZE, MPI_INT, MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD,
                 &receivedMessageStatus);
        LOG(INFO) << REC_MESS << threadManager.toString() << "Message received";

        switch (receivedMessageStatus.MPI_TAG) {
            case REQUEST:
                threadManager.processRequestMessage(receivedMessage, receivedMessageStatus);
                break;
            case ACK:
                threadManager.processAckMessage(receivedMessageStatus);
                break;
            case REALEASE:
                threadManager.processReleaseMessage(receivedMessageStatus);
                break;
        }
    }
}

int main(int argc, char **argv) {
    //TODO set settings in logs to remove unnecessary information
    START_EASYLOGGINGPP(argc, argv);

    int rank = 0, size = 10, test = 0, nameLength;
    char processorName[20];

    MPI_Init_thread(&argc, &argv, MPI_THREAD_MULTIPLE, &test);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_size( MPI_COMM_WORLD, &size );
    MPI_Get_processor_name(processorName,&nameLength);

    if(test == MPI_THREAD_MULTIPLE){
        cout<<"Jest ok" <<endl;
    } else {
        cout<<" Nie jest ok" <<endl;
    }

    srand(time(nullptr) + rank);

    ThreadManager threadManager(rank, size, processorName);

    thread t[2];
    t[0] = thread(mainThread, ref(threadManager));
    t[1] = thread(receivingThread, ref(threadManager));
    t[0].join();
    t[1].join();

    MPI_Finalize();
    return 0;
}

#pragma clang diagnostic pop
