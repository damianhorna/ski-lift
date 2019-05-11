#include "mpi.h"
#include <iostream>
#include <time.h>
#include <algorithm>
#include <vector>
#include <numeric>
#include <zconf.h>

#include "easylogging++.h"
#include "ThreadManagerBase.h"
#include "ThreadManager.h"
#include "Logger.h"

using namespace std;

INITIALIZE_EASYLOGGINGPP

void *receivingThread(ThreadManager &threadManager) {

    while (true) {
        MPI_Status receivedMessageStatus;
        int receivedMessage[ThreadManagerBase::MSG_SIZE];
        MPI_Recv(receivedMessage, ThreadManagerBase::MSG_SIZE, MPI_INT, MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD,
                 &receivedMessageStatus);
        LOG(INFO) << Logger::rec << threadManager.toString() << "Message received";

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

void *mainThread(ThreadManagerBase &threadManager) {

    while (true) {
        threadManager.increaseClock();
        int *msg = threadManager.constructMessage();
        threadManager.sendMessageForEverybody(msg, REQUEST);
        threadManager.addOwnRequestToQueue();

        threadManager.lock();
        bool canGoOnLift = false;
        while (!canGoOnLift) {
            if (threadManager.isEnoughPlaceOnLift() && threadManager.isEveryAck()) {
                LOG(INFO) << Logger::main_thread << threadManager.toString() << "Can go on lift" << endl;
                canGoOnLift = true;
            } else {
                LOG(INFO) << Logger::main_thread << threadManager.toString() << "Waiting";
                threadManager.wait();
            }
        }
        threadManager.unlock();
        threadManager.clearAcks();

        LOG(INFO) << Logger::main_thread << threadManager.toString() << "On lift";
        sleep(5);

        threadManager.increaseClock();
        msg = threadManager.constructMessage();

        threadManager.lock();
        threadManager.sendMessageForEverybody(msg, REALEASE);
        threadManager.removeYourselfFromQueue();
        threadManager.unlock();
        threadManager.sleepRandomTime();
    }
}


int main(int argc, char **argv) {
    START_EASYLOGGINGPP(argc, argv);
    el::Configurations defaultConf;
    defaultConf.setToDefault();
    defaultConf.set(el::Level::Debug, el::ConfigurationType::Format, "%datetime %level %msg");
    el::Loggers::reconfigureLogger("default", defaultConf);

    //TODO fix random values
    srand(time(nullptr));
    int rank = 0, size = 10, test = 0;

    MPI_Init_thread(&argc, &argv, MPI_THREAD_MULTIPLE, &test);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    ThreadManager threadManager(rank, size);

    //TODO Use Thread class
    pthread_t receiveMessageThread, skiLiftThread;
    pthread_create(&receiveMessageThread, nullptr, reinterpret_cast<void *(*)(void *)>(receivingThread),
                   &threadManager);
    pthread_create(&skiLiftThread, nullptr, reinterpret_cast<void *(*)(void *)>(mainThread), &threadManager);

    pthread_join(receiveMessageThread, nullptr);
    pthread_join(skiLiftThread, nullptr);

    MPI_Finalize();
    return 0;
}

#pragma clang diagnostic pop