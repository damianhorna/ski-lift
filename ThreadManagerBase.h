#ifndef SKI_LIFT_THREADSTATW_H
#define SKI_LIFT_THREADSTATW_H

#include "QueueElement.h"
#include "MessageType.h"
#include <pthread.h>
#include <vector>

using namespace std;

class ThreadManagerBase {
protected:
    int rank;
    int size;
    int myWeight;
    int clock;
    vector<int> tabAcks;
    vector<QueueElement> queue;

    pthread_mutex_t clockMutex;
    pthread_cond_t conditionalMutex;
public:

    //TODO Move something to protected section

    static const int MSG_SIZE = 2;

    static const int CAPACITY = 200;

    ThreadManagerBase(int rank, int size);

    int getSize();

    vector<QueueElement> &getQueue();

    vector<int> &getTabAcks();

    void initTabAcks();

    void increaseClock();

    void updateClock(int receivedClock);

    int *constructMessage();

    void sendMessageForEverybody(int *msg, MessageType type);

    void addOwnRequestToQueue();

    void addRequestToQueue(QueueElement element);

    void sortQueue();

    bool isEveryAck();

    bool isEnoughPlaceOnLift();

    void clearAcks();

    void removeYourselfFromQueue();

    int getSumOfACks();

    void lock();

    void unlock();

    void wait();

    void signal();

    virtual ~ThreadManagerBase();

    void removeFromQueueById(int id);

    string toString();

    void sleepRandomTime();

};


#endif //SKI_LIFT_THREADSTATW_H
