#include "ThreadState.h"

int ThreadState::getRank() {
    return rank;
}

void ThreadState::setRank(int rank) {
    ThreadState::rank = rank;
}

int ThreadState::getSize() {
    return size;
}

void ThreadState::setSize(int size) {
    ThreadState::size = size;
}

int ThreadState::getMyWeight() {
    return myWeight;
}

void ThreadState::setMyWeight(int myWeight) {
    ThreadState::myWeight = myWeight;
}

vector<QueueElement> &ThreadState::getQueue() {
    return queue;
}

void ThreadState::setQueue(vector<QueueElement> &queue) {
    ThreadState::queue = queue;
}

ThreadState::ThreadState(int rank, int size, int myWeight, vector<int> tabAcks, vector<QueueElement> &queue) : rank(
        rank), size(size), myWeight(myWeight), tabAcks(tabAcks), queue(queue) {}

ThreadState::ThreadState() {}

vector<int> &ThreadState::getTabAcks() {
    return tabAcks;
}

void ThreadState::setTabAcks(vector<int> &tabAcks) {
    ThreadState::tabAcks = tabAcks;
}
