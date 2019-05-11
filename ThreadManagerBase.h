#ifndef SKI_LIFT_THREADSTATW_H
#define SKI_LIFT_THREADSTATW_H

#include "QueueElement.h"
#include "MessageType.h"
#include <pthread.h>
#include <vector>

using namespace std;

//TODO Clean here xDDD and change functions to private
class ThreadManagerBase {
protected:
    int rank;
    int size;
    int myWeight;
    int clock;
    vector<int> tabAcks;
    vector<QueueElement> queue;

    bool IsMyRank(const QueueElement &o);

    pthread_mutex_t clockMutex;
    pthread_cond_t conditionalMutex;

public:
    ThreadManagerBase(int rank, int size, int myWeight, vector<int> tabAcks, vector<QueueElement> &queue);

    ThreadManagerBase();

    ThreadManagerBase(int rank, int size);

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

    static const int MSG_SIZE = 2;

    static const int CAPACITY = 500;

    void increaseClock();

    void updateClock(int receivedClock);

    int *constructMessage();

    int getClock() const;

    void sendMessageForEverybody(int *msg, MessageType type);

    void addOwnRequestToQueue();

    void addRequestToQueue(QueueElement element);

    void sortQueue();

    bool isEveryAck();

    bool isEnoughPlaceOnLift();

    void clearAcks();

    void removeYourselfFromQueue();

    int getSumOfWeights();

    int getSumOfACks();

    void lock();

    void unlock();

    void wait();

    void signal();

    virtual ~ThreadManagerBase();
};


#endif //SKI_LIFT_THREADSTATW_H
