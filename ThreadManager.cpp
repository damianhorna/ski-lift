#include <utility>
#include <string>
#include <cstdlib>
#include <mpi.h>
#include <numeric>
#include "ThreadManager.h"
#include "easylogging++.h"
#include "MessageType.h"

using namespace std;

int ThreadManager::getRank() {
    return rank;
}

void ThreadManager::setRank(int rank) {
    ThreadManager::rank = rank;
}

int ThreadManager::getSize() {
    return size;
}

void ThreadManager::setSize(int size) {
    ThreadManager::size = size;
}

int ThreadManager::getMyWeight() {
    return myWeight;
}

void ThreadManager::setMyWeight(int myWeight) {
    ThreadManager::myWeight = myWeight;
}

vector<QueueElement> &ThreadManager::getQueue() {
    return queue;
}

void ThreadManager::setQueue(vector<QueueElement> &queue) {
    ThreadManager::queue = queue;
}

ThreadManager::ThreadManager(int rank, int size, int myWeight, vector<int> tabAcks, vector<QueueElement> &queue) : rank(
        rank), size(size), myWeight(myWeight), tabAcks(std::move(tabAcks)), queue(queue) {}

ThreadManager::ThreadManager() {}

vector<int> &ThreadManager::getTabAcks() {
    return tabAcks;
}

void ThreadManager::setTabAcks(vector<int> &tabAcks) {
    ThreadManager::tabAcks = tabAcks;
}

void ThreadManager::initTabAcks() {
    for (int i = 0; i < size; i++) {
        tabAcks.push_back(0);
    }
}

ThreadManager::ThreadManager(int rank, int size) : rank(rank), size(size) {
    this->initTabAcks();
    this->myWeight = 40 + rand() % 80;
    this->clock = 0;
    this->tabAcks[rank] = 1;

    LOG(INFO) << "Start thread: " << rank << " with weight: " << myWeight;
}

void ThreadManager::increaseClock() {
    this->clock += 1;
}

int* ThreadManager::constructMessage() {
    static int message[ThreadManager::MSG_SIZE];
    message[0] = this->clock;
    message[1] = this->myWeight;
    return message;
}

int ThreadManager::getClock() const {
    return clock;
}

void ThreadManager::sendMessageForEverybody(int *msg, MessageType type) {
    LOG(INFO) << "Send REQUEST, from " << this->rank << " time: " << this->clock;
    for (int i = 0; i < this->size; i++) {
        if (i == rank) continue;
        MPI_Send(msg, MSG_SIZE, MPI_INT, i, type, MPI_COMM_WORLD);
    }
    LOG(INFO) << "Request was sent";
}

void ThreadManager::addOwnRequestToQueue() {
    QueueElement newElement = QueueElement(this->rank, this->clock, this->myWeight);
    this->addRequestToQueue(newElement);
}

void ThreadManager::addRequestToQueue(QueueElement element) {
    this->queue.push_back(element);
    this->sortQueue();
}

// TODO sort it correctly
void ThreadManager::sortQueue() {
    sort(this->queue.begin(), this->queue.end());
}

int ThreadManager::getSumOfWeights() {
    return accumulate(this->queue.begin(), this->queue.end(), 0,
                      [](int acc, const QueueElement &queueElement) {
                          return queueElement.getWeight() + acc;
                      }
    );
}

bool ThreadManager::isEveryAck() {
    return this->getSumOfWeights() == this->size;
}

bool ThreadManager::isEnoughPlaceOnLift() {
    int sum = 0;
    for (QueueElement &queueElement: this->queue) {
        sum += queueElement.getWeight();
        if (queueElement.getId() == this->rank) break;
    }
    return sum <= ThreadManager::CAPACITY;
}

void ThreadManager::clearAcks() {
    for (int i = 0; i < this->tabAcks.size(); i++) {
        this->tabAcks[i] = i == this->rank ? 1 : 0;
    }

}

void ThreadManager::removeYourselfFromQueue() {
    int rank = this->rank;
    queue.erase(
            remove_if(queue.begin(), queue.end(),
                      [rank](const QueueElement &o) { return o.getId() == rank; }),
            queue.end()
    );
    this->sortQueue();

}

bool ThreadManager::IsMyRank(const QueueElement &o) {
    return o.getId() == this->rank;

}

void ThreadManager::updateClock(int receivedClock) {
    if (receivedClock > this->clock)
        this->clock = receivedClock;
    this->increaseClock();
}
