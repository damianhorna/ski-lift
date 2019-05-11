#include <utility>
#include <string>
#include <cstdlib>
#include <mpi.h>
#include <numeric>
#include "ThreadManagerBase.h"
#include "easylogging++.h"
#include "MessageType.h"

using namespace std;

int ThreadManagerBase::getRank() {
    return rank;
}

void ThreadManagerBase::setRank(int rank) {
    ThreadManagerBase::rank = rank;
}

int ThreadManagerBase::getSize() {
    return size;
}

void ThreadManagerBase::setSize(int size) {
    ThreadManagerBase::size = size;
}

int ThreadManagerBase::getMyWeight() {
    return myWeight;
}

void ThreadManagerBase::setMyWeight(int myWeight) {
    ThreadManagerBase::myWeight = myWeight;
}

vector<QueueElement> &ThreadManagerBase::getQueue() {
    return queue;
}

void ThreadManagerBase::setQueue(vector<QueueElement> &queue) {
    ThreadManagerBase::queue = queue;
}

ThreadManagerBase::ThreadManagerBase(int rank, int size, int myWeight, vector<int> tabAcks, vector<QueueElement> &queue)
        : rank(
        rank), size(size), myWeight(myWeight), tabAcks(std::move(tabAcks)), queue(queue) {}

ThreadManagerBase::ThreadManagerBase() {}

vector<int> &ThreadManagerBase::getTabAcks() {
    return tabAcks;
}

void ThreadManagerBase::setTabAcks(vector<int> &tabAcks) {
    ThreadManagerBase::tabAcks = tabAcks;
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

int ThreadManagerBase::getClock() const {
    return clock;
}

void ThreadManagerBase::sendMessageForEverybody(int *msg, MessageType type) {
    LOG(INFO) << "Send REQUEST, from " << this->rank << " time: " << this->clock;
    for (int i = 0; i < this->size; i++) {
        if (i == rank) continue;
        MPI_Send(msg, MSG_SIZE, MPI_INT, i, type, MPI_COMM_WORLD);
    }
    LOG(INFO) << "Request was sent by " << this->rank;
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

int ThreadManagerBase::getSumOfWeights() {
    return accumulate(this->queue.begin(), this->queue.end(), 0,
                      [](int acc, const QueueElement &queueElement) {
                          return queueElement.getWeight() + acc;
                      }
    );
}

int ThreadManagerBase::getSumOfACks() {
    int sum = 0;
    for (int &ack:this->tabAcks) {
        sum += ack;
    }
    return sum;
}

bool ThreadManagerBase::isEveryAck() {
    cout << "Size: " << this->size << " Sum weights: " << this->getSumOfACks() << endl;
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

void ThreadManagerBase::removeYourselfFromQueue() {
    int rank = this->rank;
    queue.erase(
            remove_if(queue.begin(), queue.end(),
                      [rank](const QueueElement &o) { return o.getId() == rank; }),
            queue.end()
    );
    this->sortQueue();

}

bool ThreadManagerBase::IsMyRank(const QueueElement &o) {
    return o.getId() == this->rank;

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
