//
// Created by apilia on 5/8/2019.
//

#ifndef SKI_LIFT_THREADSTATW_H
#define SKI_LIFT_THREADSTATW_H

#include "QueueElement.h"
#include <vector>

using namespace std;

class ThreadState {
public:
    ThreadState(int rank, int size, int myWeight, vector <int> tabAcks, const vector<QueueElement> &queue);

    ThreadState();

    int getRank() const;

    void setRank(int rank);

    int getSize() const;

    void setSize(int size);

    int getMyWeight() const;

    void setMyWeight(int myWeight);

    vector<QueueElement> &getQueue();

    void setQueue(const vector<QueueElement> &queue);

    vector<int> &getTabAcks();

    void setTabAcks(const vector<int> &tabAcks);

private:
    int rank;
    int size;
    int myWeight;
    vector <int> tabAcks;
    vector <QueueElement> queue;// queue
};


#endif //SKI_LIFT_THREADSTATW_H
