//
// Created by apilia on 5/8/2019.
//

#include "ThreadState.h"

int ThreadState::getRank() const {
    return rank;
}

void ThreadState::setRank(int rank) {
    ThreadState::rank = rank;
}

int ThreadState::getSize() const {
    return size;
}

void ThreadState::setSize(int size) {
    ThreadState::size = size;
}

int ThreadState::getMyWeight() const {
    return myWeight;
}

void ThreadState::setMyWeight(int myWeight) {
    ThreadState::myWeight = myWeight;
}

vector<QueueElement> &ThreadState::getQueue() {
    return queue;
}

void ThreadState::setQueue(const vector<QueueElement> &queue) {
    ThreadState::queue = queue;
}

ThreadState::ThreadState(int rank, int size, int myWeight, vector<int> tabAcks, const vector<QueueElement> &queue) : rank(
        rank), size(size), myWeight(myWeight), tabAcks(tabAcks), queue(queue) {}

ThreadState::ThreadState() {}

vector<int> &ThreadState::getTabAcks() {
    return tabAcks;
}

void ThreadState::setTabAcks(const vector<int> &tabAcks) {
    ThreadState::tabAcks = tabAcks;
}
