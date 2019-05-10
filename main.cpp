#include "mpi.h"
#include <iostream>
#include <time.h>
#include <algorithm>
#include <vector>
#include <numeric>
#include <zconf.h>

#include "easylogging++.h"
#include "ThreadManagerBase.h"
#include "ThreadManager.h"

_INITIALIZE_EASYLOGGINGPP
using namespace std;


//TODO remove comments
void *receivingThread(ThreadManager &threadManager) {

    while (true) {
        MPI_Status receivedMessageStatus;
        int receivedMessage[ThreadManagerBase::MSG_SIZE], receivedClock, receivedWeight;
        LOG(INFO) << "Chciałbym dostać wiadomość";
        MPI_Recv(receivedMessage, ThreadManagerBase::MSG_SIZE, MPI_INT, MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD,
                 &receivedMessageStatus);
        LOG(INFO) << "Dostałem wiadomość!";
        //TODO Remove
        printf("[Wątek %d - ack] otrzymał wiadomość od  %d o TAGU: %d.  [zegar z wiadomosci = %d]\n",
               threadManager.getRank(), receivedMessageStatus.MPI_SOURCE, receivedMessageStatus.MPI_TAG, receivedClock);


        switch (receivedMessageStatus.MPI_TAG) {
            case REQUEST:
                threadManager.processRequestMessage(receivedMessage, receivedMessageStatus);
                break;
            case ACK:
                threadManager.processAckMessage(receivedMessage, receivedMessageStatus);
                break;
            case REALEASE:
                threadManager.processReleaseMessage(receivedMessage, receivedMessageStatus);
                break;
        }
    }
}

void *mainThread(ThreadManagerBase &threadManager) {

    while (true) {
        sleep(3);
        threadManager.increaseClock();
        int *msg = threadManager.constructMessage();
        threadManager.sendMessageForEverybody(msg, REQUEST);
        threadManager.addOwnRequestToQueue();

        threadManager.lock();
        bool canGoOnLift = false;
        while (!canGoOnLift) {
            if (threadManager.isEnoughPlaceOnLift() && threadManager.isEveryAck()) {
                LOG(INFO) << "Thread " << threadManager.getRank() << " can go on lift";
                canGoOnLift = true;
            } else {
                // TODO Remove this log
                printf("[Wątek %d - main] zasypiam sobie... zzz... [zegar = %d]\n", threadManager.getRank(),
                       threadManager.getClock());
                threadManager.wait();
            }
        }
        cout << "WBIJAM DO KOLEJKI!";
        threadManager.unlock();
        threadManager.clearAcks();

        //TODO Remove this log
        printf("[Wątek %d - main] wszytskie wątki odebrały moją wiadomość wiadomości. [zegar = %d] Pozdrawiam, watek %d\n",
               threadManager.getRank(), threadManager.getClock(), threadManager.getRank());
        printf("[Wątek %d - main] wyzerowuje tablice ack i wejeżdza do góry. SEKCJA KRYTYCZNA [zegar = %d]\n",
               threadManager.getRank(), threadManager.getClock());
        printf("\n[Wątek %d - main] wjeżdzam do góry przez %d sekund [zegar = %d]\n\n", threadManager.getRank(), 5,
               threadManager.getClock());

        sleep(5);
        threadManager.increaseClock();
        msg = threadManager.constructMessage();


        //TODO Remove this log
        printf("\n[Wątek %d - main] KONIEC SEKCJI KRYTYCZNEJ [zegar = %d]\n\n", threadManager.getRank(),
               threadManager.getClock());

        threadManager.lock();


        // send RELEASE
        threadManager.sendMessageForEverybody(msg, REALEASE);

        //TODO Remove this log
        printf("[Wątek %d - main] wysyłał release synał do wszystkich i zjeżdża na dół. [zegar = %d]\n",
               threadManager.getRank(), threadManager.getClock());
        printf("[Wątek %d - main] usuwa swoje zgłoszenie ze swojej kolejki. [zegar = %d]\n", threadManager.getRank(),
               threadManager.getClock());

        //  usun swoje zadanie z kolejki
        threadManager.removeYourselfFromQueue();
        threadManager.unlock();

        int randomTime = 1 + rand() % 5;

        //TODO Remove this log
        printf("[Wątek %d - main] zjedża z góry przez %d sekund................ [zegar = %d]\n",
               threadManager.getRank(), randomTime, threadManager.getClock());
        sleep(randomTime); // czy to jest potzrebne? tak jest
        printf("[Wątek %d - main] zjechał i znowy  ustawia się do kolejki narciarzy. [zegar = %d]\n",
               threadManager.getSize(), threadManager.getClock());
    }
}


int main(int argc, char **argv) {
    //TODO fix random values
    srand(time(nullptr));
    int rank = 0, size = 10, test = 0;

    MPI_Init_thread(&argc, &argv, MPI_THREAD_MULTIPLE, &test);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    ThreadManager threadManager(rank, size);

    pthread_t receiveMessageThread, skiLiftThread;
    pthread_create(&receiveMessageThread, nullptr, reinterpret_cast<void *(*)(void *)>(receivingThread),
                   &threadManager);
    pthread_create(&skiLiftThread, nullptr, reinterpret_cast<void *(*)(void *)>(mainThread), &threadManager);

    pthread_join(receiveMessageThread, nullptr);
    pthread_join(skiLiftThread, nullptr);

    MPI_Finalize();
    return 0;
}

#pragma clang diagnostic pop