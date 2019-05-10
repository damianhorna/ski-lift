#ifndef SKI_LIFT_THREADSTATW_H
#define SKI_LIFT_THREADSTATW_H

#include "QueueElement.h"
#include <vector>

using namespace std;

class ThreadState {
public:
    ThreadState(int rank, int size, int myWeight, vector<int> tabAcks, vector<QueueElement> &queue);

    ThreadState();

    int getRank();

    void setRank(int rank);

    int getSize();

    void setSize(int size);

    int getMyWeight();

    void setMyWeight(int myWeight);

    vector<QueueElement> &getQueue();

    void setQueue(vector<QueueElement> &queue);

    vector<int> &getTabAcks();

    void setTabAcks(vector<int> &tabAcks);

    void initTabAcks();

private:
    int rank;
    int size;
    int myWeight;
    vector<int> tabAcks;
    vector<QueueElement> queue;// queue
};


#endif //SKI_LIFT_THREADSTATW_H
