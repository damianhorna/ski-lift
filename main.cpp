#include "mpi.h"
#include <iostream>
#include <pthread.h>
#include <time.h>
#include <algorithm>
#include <vector>
#include <numeric>
#include <zconf.h>

#include "easylogging++.h"
#include "ThreadManager.h"
#include "MessageType.h"

pthread_mutex_t mutexClock = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cond = PTHREAD_COND_INITIALIZER;

_INITIALIZE_EASYLOGGINGPP
using namespace std;

//
//void print(ThreadManager &threadState) {
//    printf("[Watek %d; zegar %d] KOLEJKA: ", threadState.getRank(), clock);
//    for (QueueElement &elem : threadState.getQueue()) {
//        printf("id: %d time: %d weight: %d; \t", elem.getId(), elem.getTime(), elem.getWeight());
//    }
//    printf("KONIEC KOLEJKI\n");
//}
//
//int checkWeights(ThreadManager &threadState) {
//    int sum = 0;
//    for (QueueElement &elem : threadState.getQueue()) {
//        if (elem.getId() == threadState.getRank()) {
//            break;
//        }
//        sum += elem.getWeight();
//    }
//    return sum;
//}
//
//void *receivingThread(void *arg) {
//
//    while (!stop) {
//        MPI_Status receivedMessage;
//        int msg[MSG_SIZE], receivedClock, receivedWeight;
//
//        MPI_Recv(msg, MSG_SIZE, MPI_INT, MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &receivedMessage);
//        printf("[Wątek %d - ack] otrzymał wiadomość od  %d o TAGU: %d.  [zegar z wiadomosci = %d]\n",
//               threadManager.getRank(),
//               receivedMessage.MPI_SOURCE, receivedMessage.MPI_TAG, receivedClock);
//
//        receivedClock = msg[0];
//        receivedWeight = msg[1];
//
//        // wstaw do kolejki
//        if (receivedMessage.MPI_TAG == REQUEST) {
//            pthread_mutex_lock(&mutexClock);
//
//            clock = (clock > receivedClock) ? clock : receivedClock;
//            clock += 1;
//
//            printf("[Wątek %d - ack] wstawia do kolejki zgłoszenie %d. [zegar = %d]\n", threadManager.getRank(),
//                   receivedMessage.MPI_SOURCE,
//                   clock);
//            threadManager.getQueue().emplace_back(receivedMessage.MPI_SOURCE, receivedClock, receivedWeight);
//            sort(threadManager.getQueue().begin(), threadManager.getQueue().end());
//            print(threadManager);
//            msg[0] = clock;
//            msg[1] = -1;
//            MPI_Send(msg, MSG_SIZE, MPI_INT, receivedMessage.MPI_SOURCE, ACK, MPI_COMM_WORLD);
//            pthread_mutex_unlock(&mutexClock);
//        } else if (receivedMessage.MPI_TAG == ACK) {
//            printf("[Wątek %d - ack] ustawia w tablicy ack od  %d. [zegar = %d]\n", threadManager.getRank(),
//                   receivedMessage.MPI_SOURCE,
//                   clock);
//
//            pthread_mutex_lock(&mutexClock);
//            threadManager.getTabAcks()[receivedMessage.MPI_SOURCE] = 1;
//
//            //print tabAcks
//            int j;
//            printf("Tab_ack: [ %d", threadManager.getTabAcks()[0]);
//            for (j = 1; j < threadManager.getSize(); j++) {
//                printf(", %d", threadManager.getTabAcks()[j]);
//            }
//            printf("]\n");
//
//
//            int success = 1;
//            int i;
//            for (i = 0; i < threadManager.getSize(); i++) {
//                if (threadManager.getTabAcks()[i] != 1) {
//                    success = 0;
//                    break;
//                }
//            }
//            if ((checkWeights(threadManager) + threadManager.getMyWeight()) > CAPACITY =) {
//
//                success = 0;
//            }
//
//            if (success) {
//                printf("[Wątek %d - ack] ACK probuje wybudzić wątek :D [zegar = %d]\n", threadManager.getRank(),
//                       clock);
//
//                pthread_cond_signal(&cond); // Should wake up *one* thread
//            }
//            pthread_mutex_unlock(&mutexClock);
//        } else if (receivedMessage.MPI_TAG == REALEASE) {
//            pthread_mutex_lock(&mutexClock);
//            printf("[Wątek %d - ack] usuwa z kolejki zgłoszenie %d.[zegar = %d]\n", threadManager.getRank(),
//                   receivedMessage.MPI_SOURCE,
//                   clock);
//
//            int idToRemove = receivedMessage.MPI_SOURCE;
//            threadManager.getQueue().erase(
//                    remove_if(threadManager.getQueue().begin(), threadManager.getQueue().end(),
//                              [&idToRemove](QueueElement &queue_element) {
//                                  return queue_element.getId() == idToRemove;
//                              }),
//                    threadManager.getQueue().end());
//            sort(threadManager.getQueue().begin(), threadManager.getQueue().end());
//            //print(threadManager);
//            int success = 1;
//            int i;
//            for (i = 0; i < threadManager.getSize(); i++) {
//                if (threadManager.getTabAcks()[i] != 1) {
//                    success = 0;
//                    break;
//                }
//            }
//
//            printf("[Wątek %d - ack] Suma wag: %d .[zegar = %d]\n", threadManager.getRank(), checkWeights(threadManager),
//                   clock);
//
//            if ((checkWeights(threadManager) + threadManager.getMyWeight()) > CAPACITY =) {
//                success = 0;
//            }
//
//            if (success) {
//                printf("[Wątek %d - ack] RELEASE probuje wybudzić wątek :D [zegar = %d]\n", threadManager.getRank(),
//                       clock);
//
//                pthread_cond_signal(&cond); // Should wake up *one* thread
//            }
//            pthread_mutex_unlock(&mutexClock); // Zmiana- unlock po signal
//        }
//    }
//    return NULL;
//}
//
void *mainThread(ThreadManager threadManager) {

    while (true) {
        threadManager.increaseClock();
        int *msg = threadManager.constructMessage();
        threadManager.sendMessageForEverybody(msg, REQUEST);
        threadManager.addOwnRequestToQueue();


        pthread_mutex_lock(&mutexClock);
        bool canGoOnLift = false;
        while (!canGoOnLift) {
            if (threadManager.isEnoughPlaceOnLift() && threadManager.isEveryAck()) {
                canGoOnLift = true;
            } else {
                // TODO Remove this log
                printf("[Wątek %d - main] zasypiam sobie... zzz... [zegar = %d]\n", threadManager.getRank(),
                       threadManager.getClock());
                pthread_cond_wait(&cond, &mutexClock);
            }
        }
        pthread_mutex_unlock(&mutexClock);
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

        pthread_mutex_lock(&mutexClock);


        // send RELEASE
        threadManager.sendMessageForEverybody(msg, REALEASE);

        //TODO Remove this log
        printf("[Wątek %d - main] wysyłał release synał do wszystkich i zjeżdża na dół. [zegar = %d]\n",
               threadManager.getRank(), threadManager.getClock());
        printf("[Wątek %d - main] usuwa swoje zgłoszenie ze swojej kolejki. [zegar = %d]\n", threadManager.getRank(),
               threadManager.getClock());

        //  usun swoje zadanie z kolejki
        threadManager.removeYourselfFromQueue();
        pthread_mutex_unlock(&mutexClock);

        int randomTime = 1 + rand() % 5;

        //TODO Remove this log
        printf("[Wątek %d - main] zjedża z góry przez %d sekund................ [zegar = %d]\n",threadManager.getRank(),randomTime,threadManager.getClock());
        sleep(randomTime); // czy to jest potzrebne?
        printf("[Wątek %d - main] zjechał i znowy  ustawia się do kolejki narciarzy. [zegar = %d]\n",threadManager.getSize(),threadManager.getClock());
    }
}


int main(int argc, char **argv) {
    srand(time(nullptr));
    int rank = 0, size = 10;
    int elo;
//
    MPI_Init_thread(&argc, &argv, MPI_THREAD_MULTIPLE, &elo);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    ThreadManager threadManager(rank, size);
//
    pthread_t receiveMessageThread, skiLiftThread;
//    pthread_create(&receiveMessageThread, nullptr, receivingThread, &threadManager);
    pthread_create(&skiLiftThread, nullptr, reinterpret_cast<void *(*)(void *)>(mainThread), &threadManager);
//
//    pthread_join(receiveMessageThread, nullptr);
    pthread_join(skiLiftThread, nullptr);
//
//    pthread_mutex_destroy(&mutexClock);
//    pthread_cond_destroy(&cond);
//    MPI_Finalize();
//    printf("Koniec programu");
    return 0;
}

#pragma clang diagnostic pop