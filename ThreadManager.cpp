#include <cstdio>
#include "ThreadManager.h"
#include "easylogging++.h"

//TODO remove this function, use logger or something like that
void print(ThreadManagerBase &threadState) {
    printf("[Watek %d; zegar %d] KOLEJKA: ", threadState.getRank(), threadState.getClock());
    for (QueueElement &elem : threadState.getQueue()) {
        printf("id: %d time: %d weight: %d; \t", elem.getId(), elem.getTime(), elem.getWeight());
    }
    printf("KONIEC KOLEJKI\n");
}

void ThreadManager::processRequestMessage(int receivedMessage[], MPI_Status receivedMessageStatus) {

    int receivedClock, receivedWeight;
    receivedClock = receivedMessage[0];
    receivedWeight = receivedMessage[1];

    LOG(INFO) << "DOSTALEM REQUESTA";
    this->lock();

    this->updateClock(receivedClock);

    //TODO remove this log
    printf("[Wątek %d - ack] wstawia do kolejki zgłoszenie %d. [zegar = %d]\n", this->getRank(),
           receivedMessageStatus.MPI_SOURCE, this->getClock());

    this->addRequestToQueue(
            QueueElement(receivedMessageStatus.MPI_SOURCE, receivedClock, receivedWeight));
    print(*this);

    int *msg;
    msg = this->constructMessage();
    MPI_Send(msg, ThreadManagerBase::MSG_SIZE, MPI_INT, receivedMessageStatus.MPI_SOURCE, ACK, MPI_COMM_WORLD);
    this->unlock();
}

void ThreadManager::processAckMessage(int receivedMessage[], MPI_Status receivedMessageStatus) {
    printf("[Wątek %d - ack] ustawia w tablicy ack od  %d. [zegar = %d]\n", this->getRank(),
           receivedMessageStatus.MPI_SOURCE, this->getClock());

    this->lock();
    this->getTabAcks()[receivedMessageStatus.MPI_SOURCE] = 1;

    //TODO Create some logger and move it from here
    //print tabAcks
    int j;
    printf("Tab_ack: [ %d", this->getTabAcks()[0]);
    for (j = 1; j < this->getSize(); j++) {
        printf(", %d", this->getTabAcks()[j]);
    }
    printf("]\n");
    cout << "Powinienem budzic!? " << this->getRank() << endl;
    cout << this->isEveryAck() << " " << this->isEnoughPlaceOnLift() << endl;
    if (this->isEveryAck() && this->isEnoughPlaceOnLift()) {
        printf("[Wątek %d - ack] ACK probuje wybudzić wątek :D [zegar = %d]\n", this->getRank(),
               this->getClock());

        this->signal(); // Should wake up *one* thread
    }
    this->unlock();
    cout << "Po funkcji " << this->getRank() << endl;

}

void ThreadManager::processReleaseMessage(int receivedMessage[], MPI_Status receivedMessageStatus) {
    this->lock();

    //TODO Remove this log
    printf("[Wątek %d - ack] usuwa z kolejki zgłoszenie %d.[zegar = %d]\n", this->getRank(),
           receivedMessageStatus.MPI_SOURCE,
           this->getClock());

    int idToRemove = receivedMessageStatus.MPI_SOURCE;

    //TODO Move it to thread manager
    this->getQueue().erase(
            remove_if(this->getQueue().begin(), this->getQueue().end(),
                      [&idToRemove](QueueElement &queue_element) {
                          return queue_element.getId() == idToRemove;
                      }),
            this->getQueue().end());
    this->sortQueue();

    //TODO logger like above
    print(*this);

    //TODO Remove this log
    printf("[Wątek %d - ack] Suma wag: %d .[zegar = %d]\n", this->getRank(),
           this->getSumOfWeights(),
           this->getClock());

    //TODO Why we dont check acks tab?
    if (this->isEnoughPlaceOnLift()) {
        printf("[Wątek %d - ack] RELEASE probuje wybudzić wątek :D [zegar = %d]\n", this->getRank(),
               this->getClock());

        this->signal();
    }
    this->unlock();
}

ThreadManager::ThreadManager(int rank, int size) : ThreadManagerBase(rank, size) {}
