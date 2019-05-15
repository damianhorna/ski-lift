#include <utility>
#include <string>
#include <cstdlib>
#include <mpi.h>
#include <numeric>
#include <iomanip>
#include <zconf.h>
#include "ThreadManagerBase.h"
#include "easylogging++.h"
#include "MessageType.h"
#include "Logger.h"

using namespace std;

int ThreadManagerBase::getSize() {
    return size;
}

vector<QueueElement> &ThreadManagerBase::getQueue() {
    return queue;
}

vector<int> &ThreadManagerBase::getTabAcks() {
    return tabAcks;
}

void ThreadManagerBase::initTabAcks() {
    for (int i = 0; i < size; i++) {
        tabAcks.push_back(0);
    }
}

ThreadManagerBase::ThreadManagerBase(int rank, int size) : rank(rank), size(size) {
    this->initTabAcks();
    this->myWeight = 40 + rand() % 80;
    this->clock = 0;
    this->tabAcks[rank] = 1;

    this->clockMutex = PTHREAD_MUTEX_INITIALIZER;
    this->conditionalMutex = PTHREAD_COND_INITIALIZER;

    LOG(INFO) << "Start thread: " << rank << " with weight: " << myWeight;
}

void ThreadManagerBase::increaseClock() {
    this->clock += 1;
}

int *ThreadManagerBase::constructMessage() {
    static int message[ThreadManagerBase::MSG_SIZE];
    message[0] = this->clock;
    message[1] = this->myWeight;
    return message;
}

void ThreadManagerBase::sendMessageForEverybody(int *msg, MessageType type) {
    LOG(DEBUG) << MAIN_MESS << this->toString() << "Send message for everybody";
    for (int i = 0; i < this->size; i++) {
        if (i == rank) continue;
        MPI_Send(msg, MSG_SIZE, MPI_INT, i, type, MPI_COMM_WORLD);
    }
}

void ThreadManagerBase::addOwnRequestToQueue() {
    QueueElement newElement = QueueElement(this->rank, this->clock, this->myWeight);
    this->addRequestToQueue(newElement);
}

void ThreadManagerBase::addRequestToQueue(QueueElement element) {
    this->queue.push_back(element);
    this->sortQueue();
}

// TODO sort it correctly
void ThreadManagerBase::sortQueue() {
    sort(this->queue.begin(), this->queue.end());
}

int ThreadManagerBase::getSumOfACks() {
    int sum = 0;
    for (int &ack:this->tabAcks) {
        sum += ack;
    }
    return sum;
}

bool ThreadManagerBase::isEveryAck() {
    return this->getSumOfACks() == this->size;
}

bool ThreadManagerBase::isEnoughPlaceOnLift() {
    int sum = 0;
    for (QueueElement &queueElement: this->queue) {
        sum += queueElement.getWeight();
        if (queueElement.getId() == this->rank) break;
    }
    return sum <= ThreadManagerBase::CAPACITY;
}

void ThreadManagerBase::clearAcks() {
    for (int i = 0; i < this->tabAcks.size(); i++) {
        this->tabAcks[i] = i == this->rank ? 1 : 0;
    }

}

void ThreadManagerBase::removeFromQueueById(int id) {
    queue.erase(
            remove_if(queue.begin(), queue.end(),
                      [id](const QueueElement &o) { return o.getId() == id; }),
            queue.end()
    );
    this->sortQueue();
}

void ThreadManagerBase::removeYourselfFromQueue() {
    LOG(INFO) << MAIN_MESS << this->toString() << "Remove yourself from queue";
    this->removeFromQueueById(this->rank);
//    logger.queueUpdate(*this);
}

void ThreadManagerBase::updateClock(int receivedClock) {
    if (receivedClock > this->clock)
        this->clock = receivedClock;
    this->increaseClock();
}

ThreadManagerBase::~ThreadManagerBase() {
    pthread_mutex_destroy(&this->clockMutex);
    pthread_cond_destroy(&this->conditionalMutex);
}

void ThreadManagerBase::lock() {
    pthread_mutex_lock(&this->clockMutex);
}

void ThreadManagerBase::unlock() {
    pthread_mutex_unlock(&this->clockMutex);
}

void ThreadManagerBase::wait() {
    pthread_cond_wait(&this->conditionalMutex, &this->clockMutex);
}

void ThreadManagerBase::signal() {
    pthread_cond_signal(&this->conditionalMutex);
}

void ThreadManagerBase::sleepRandomTime() {
    sleep(1 + rand() % 5);
}

string ThreadManagerBase::toString() {
    std::stringstream ss;
    ss << left;
    ss << "[";
    ss << "ID:" << setw(2) << this->rank;
    ss << "WEIGHT:" << setw(4) << this->myWeight;
    ss << "CLOCK:" << setw(2) << this->clock;
    ss << "] ";
    return ss.str();
}
