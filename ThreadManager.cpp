#include <cstdio>
#include <iomanip>
#include "ThreadManager.h"
#include "easylogging++.h"
#include "Logger.h"

void ThreadManager::processRequestMessage(const int receivedMessage[], MPI_Status receivedMessageStatus) {
    LOG(INFO) << REC_MESS << REQ_MESS << this->toString() <<
              "Request from [" << receivedMessageStatus.MPI_SOURCE << "]";

    int receivedClock, receivedWeight;
    receivedClock = receivedMessage[0];
    receivedWeight = receivedMessage[1];

    this->lock();
    this->updateClock(receivedClock);
    this->addRequestToQueue(
            QueueElement(receivedMessageStatus.MPI_SOURCE, receivedClock, receivedWeight));

    //TODO It could be moved outside this function
    std::stringstream s2;
    s2 << REC_MESS << this->toString() << "Queue: ";
    for (QueueElement &elem : this->getQueue()) {
        s2 << elem.toString() << ",";
    }
    LOG(DEBUG) << s2.str();
    

    int *msg = this->constructMessage();
    LOG(DEBUG) << REC_MESS << REQ_MESS << this->toString() << "I will send ACK";
    MPI_Send(msg, ThreadManagerBase::MSG_SIZE, MPI_INT, receivedMessageStatus.MPI_SOURCE, ACK, MPI_COMM_WORLD);
    LOG(DEBUG) << REC_MESS << REQ_MESS << this->toString() << "ACK was sent";
    this->unlock();
}

void ThreadManager::processAckMessage(MPI_Status receivedMessageStatus) {
    LOG(INFO) << REC_MESS << ACK_MESS << this->toString() <<
              "Request from [" << receivedMessageStatus.MPI_SOURCE << "]";
    this->lock();
    this->tabAcks[receivedMessageStatus.MPI_SOURCE] = 1;

    if (this->isEveryAck() && this->isEnoughPlaceOnLift()) {
        LOG(DEBUG) << REC_MESS << REL_MESS << this->toString() << "Signal";
        this->signal();
    }

    //TODO It could be moved outside this function
    std::stringstream s1, s2, s3;
    s1 << REC_MESS << ACK_MESS << this->toString() << "Ack was received from [" << receivedMessageStatus.MPI_SOURCE
       << "] and ACK Table was updated" << endl;
    s2 << REC_MESS << ACK_MESS << this->toString() << left << setw(4) << "ID:";
    s3 << REC_MESS << ACK_MESS << this->toString() << left << setw(4) << "TAB:";

    for (int i = 0; i < this->getSize(); i++) {
        s2 << setw(2) << i;
        s3 << setw(2) << this->getTabAcks().at(i);
    }

    s2 << endl;
    string result = s1.str() + s2.str() + s3.str();
    LOG(DEBUG) << result;
    //
    this->unlock();
}

void ThreadManager::processReleaseMessage(MPI_Status receivedMessageStatus) {
    this->lock();
    LOG(INFO) << REC_MESS << REL_MESS << this->toString() <<
              "Request from [" << receivedMessageStatus.MPI_SOURCE << "]";
    this->removeFromQueueById(receivedMessageStatus.MPI_SOURCE);

    //TODO It could be moved outside this function
    std::stringstream s2;
    s2 << REC_MESS << this->toString() << "Queue: ";
    for (QueueElement &elem : this->getQueue()) {
        s2 << elem.toString() << ",";
    }
    LOG(DEBUG) << s2.str();

    //TODO Why we dont check acks tab?
    if (this->isEnoughPlaceOnLift()) {
        LOG(DEBUG) << REC_MESS << REL_MESS << this->toString() << "Signal";
        this->signal();
    }
    this->unlock();
}

ThreadManager::ThreadManager(int rank, int size, char processorName[]) : ThreadManagerBase(rank, size, processorName) {}
