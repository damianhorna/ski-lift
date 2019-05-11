#include <cstdio>
#include <iomanip>
#include "Logger.h"
#include "ThreadManager.h"
#include "easylogging++.h"

void ThreadManager::processRequestMessage(const int receivedMessage[], MPI_Status receivedMessageStatus) {
    this->lock();
    LOG(INFO) << Logger::rec << Logger::req << this->toString() <<
              "Request from [" << receivedMessageStatus.MPI_SOURCE << "]";

    int receivedClock, receivedWeight;
    receivedClock = receivedMessage[0];
    receivedWeight = receivedMessage[1];

    this->updateClock(receivedClock);
    this->addRequestToQueue(
            QueueElement(receivedMessageStatus.MPI_SOURCE, receivedClock, receivedWeight));

    Logger::queueUpdate(*this);
    int *msg = this->constructMessage();
    MPI_Send(msg, ThreadManagerBase::MSG_SIZE, MPI_INT, receivedMessageStatus.MPI_SOURCE, ACK, MPI_COMM_WORLD);
    this->unlock();
}

void ThreadManager::processAckMessage(MPI_Status receivedMessageStatus) {
    this->lock();
    this->tabAcks[receivedMessageStatus.MPI_SOURCE] = 1;
    LOG(INFO) << Logger::rec << Logger::ack << this->toString() <<
              "Request from [" << receivedMessageStatus.MPI_SOURCE << "]";

    if (this->isEveryAck() && this->isEnoughPlaceOnLift()) {
        LOG(DEBUG) << Logger::rec << Logger::rel << "Signal";
        this->signal();
    }
    this->unlock();
    Logger::afterReceiveAck(*this, receivedMessageStatus.MPI_SOURCE);
}

void ThreadManager::processReleaseMessage(MPI_Status receivedMessageStatus) {
    this->lock();
    LOG(INFO) << Logger::rec << Logger::rel << this->toString() <<
              "Request from [" << receivedMessageStatus.MPI_SOURCE << "]";
    this->removeFromQueueById(receivedMessageStatus.MPI_SOURCE);
    Logger::queueUpdate(*this);

    //TODO Why we dont check acks tab?
    if (this->isEnoughPlaceOnLift()) {
        LOG(DEBUG) << Logger::rec << Logger::rel << "Signal";
        this->signal();
    }
    this->unlock();
}

ThreadManager::ThreadManager(int rank, int size) : ThreadManagerBase(rank, size) {}
