#include "mpi.h"
#include "ThreadState.h"
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <time.h>

#define MSG_SIZE 2
#define MSG_HELLO 100
#define TAG_REQ 123
#define TAG_ACK 456
#define TAG_RELEASE 789
#define Capacity 150
#define GOUPTIME 5

#include <algorithm>
#include <vector>

int clockLamport = 0;
int stop = 0;

pthread_mutex_t mutexClock = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cond = PTHREAD_COND_INITIALIZER;

using namespace std;


ThreadState threadState;

// wyswietl zawartosc kolejki
void print(const ThreadState &threadState) {
    printf("[Watek %d; zegar %d] KOLEJKA: ", threadState.getRank(), clockLamport);
    for (QueueElement &elem : threadState.getQueue()) {
        printf("id: %d time: %d weight: %d; \t", elem.getId(), elem.getTime(), elem.getWeight());
    }
    printf("KONIEC KOLEJKI\n");
}

int checkWeights(const ThreadState &threadState) {
    int sum = 0;
    for (QueueElement &elem : threadState.getQueue()) {
        if (elem.getId() == threadState.getRank()) {
            break;
        }
        sum += elem.getWeight();
    }
    return sum;
}

void *receivingThread(void *arg) {

    while (!stop) {
        MPI_Status status;
        int msg[MSG_SIZE], receivedClock, receivedWeight;

        MPI_Recv(msg, MSG_SIZE, MPI_INT, MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
        printf("[Wątek %d - ack] otrzymał wiadomość od  %d o TAGU: %d.  [zegar z wiadomosci = %d]\n",
               threadState.getRank(),
               status.MPI_SOURCE, status.MPI_TAG, receivedClock);

        receivedClock = msg[0];
        receivedWeight = msg[1];

        // wstaw do kolejki
        if (status.MPI_TAG == TAG_REQ) {
            pthread_mutex_lock(&mutexClock);

            clockLamport = (clockLamport > receivedClock) ? clockLamport : receivedClock;
            clockLamport += 1;

            printf("[Wątek %d - ack] wstawia do kolejki zgłoszenie %d. [zegar = %d]\n", threadState.getRank(),
                   status.MPI_SOURCE,
                   clockLamport);
            threadState.getQueue().push_back(queue_element(status.MPI_SOURCE, receivedClock, receivedWeight));
            sort(threadState.getQueue().begin(), threadState.getQueue().end());
            print(threadState);
            msg[0] = clockLamport;
            msg[1] = -1;
            MPI_Send(msg, MSG_SIZE, MPI_INT, status.MPI_SOURCE, TAG_ACK, MPI_COMM_WORLD);
            pthread_mutex_unlock(&mutexClock);
        } else if (status.MPI_TAG == TAG_ACK) {
            printf("[Wątek %d - ack] ustawia w tablicy ack od  %d. [zegar = %d]\n", threadState.getRank(),
                   status.MPI_SOURCE,
                   clockLamport);

            pthread_mutex_lock(&mutexClock);
            threadState.getTabAcks()[status.MPI_SOURCE] = 1;

            //print tabAcks
            int j;
            printf("Tab_ack: [ %d", threadState.getTabAcks()[0]);
            for (j = 1; j < threadState.getSize(); j++) {
                printf(", %d", threadState.getTabAcks()[j]);
            }
            printf("]\n");


            int success = 1;
            int i;
            for (i = 0; i < threadState.getSize(); i++) {
                if (threadState.getTab_ack()[i] != 1) {
                    success = 0;
                    break;
                }
            }
            if ((checkWeights(threadState) + threadState.getMyWeight()) > Capacity) {

                success = 0;
            }

            if (success) {
                printf("[Wątek %d - ack] ACK probuje wybudzić wątek :D [zegar = %d]\n", threadState.rank, clockLamport);

                pthread_cond_signal(&cond); // Should wake up *one* thread
            }
            pthread_mutex_unlock(&mutexClock);
        } else if (status.MPI_TAG == TAG_RELEASE) {
            pthread_mutex_lock(&mutexClock);
            printf("[Wątek %d - ack] usuwa z kolejki zgłoszenie %d.[zegar = %d]\n", threadState.rank, status.MPI_SOURCE,
                   clockLamport);

            int idToRemove = status.MPI_SOURCE;
            threadState.getQueue().erase(
                    remove_if(threadState.getQueue().begin(), threadState.getQueue().end(),
                              [&idToRemove](QueueElement const &queue_element) {
                                  return queue_element.getId() == idToRemove;
                              }),
                    threadState.getQueue().end());
            sort(threadState.getQueue().begin(), threadState.getQueue().end());
            //print(threadState);
            int success = 1;
            int i;
            for (i = 0; i < threadState.getSize(); i++) {
                if (threadState.getTab_ack()[i] != 1) {
                    success = 0;
                    break;
                }
            }

            printf("[Wątek %d - ack] Suma wag: %d .[zegar = %d]\n", threadState.getRank(), checkWeights(threadState),
                   clockLamport);

            if ((checkWeights(threadState) + threadState.getMyWeight()) > Capacity) {
                success = 0;
            }

            if (success) {
                printf("[Wątek %d - ack] RELEASE probuje wybudzić wątek :D [zegar = %d]\n", threadState.getRank(),
                       clockLamport);

                pthread_cond_signal(&cond); // Should wake up *one* thread
            }
            pthread_mutex_unlock(&mutexClock); // Zmiana- unlock po signal
        }
    }
    return NULL;
}

void *mainThread(void *arg) {

    while (!stop) {
        int msg[MSG_SIZE];

        int i;
        int receivedClock, receivedStatus;
        // semafor P
        pthread_mutex_lock(&mutexClock);
        clockLamport += 1;
        msg[0] = clockLamport;
        msg[1] = threadState.getMyWeight();
        printf("[Wątek %d - main] wysłała do wszystkich request. [zegar = %d]\n", threadState.getRank(), clockLamport);

        for (i = 0; i < threadState.getSize(); i++) {
            if (i != threadState.getRank()) // do not send to yourself
            {
                MPI_Send(msg, MSG_SIZE, MPI_INT, i, TAG_REQ, MPI_COMM_WORLD);

            }
        }
        // semafor V
        //wstaw do kolejki wlasne zadanie
        QueueElement newEelement = QueueElement(threadState.getRank(), clockLamport, threadState.getMyWeight());
        vector<QueueElement> queue = threadState.getQueue();
        queue.push_back(newEelement);
        threadState.setQueue(queue);
        sort(threadState.getQueue().begin(), threadState.getQueue().end());

        printf("[Wątek %d - main] wstawił do swojej kolejki swój request. [zegar = %d]\n", threadState.getRank(),
               clockLamport);

        //sprawdz warunek bazujacy na kolejce (suma wag) i czy od wszystkich ack
        printf("[Wątek %d - main] sprawdza czy wszytskie wątki odebrały wiadomości. [zegar = %d]\n",
               threadState.getRank(),
               clockLamport);
        do {
            int success = 1;
            int i;
            for (i = 0; i < threadState.getSize(); i++) {
                if (threadState.getTabAcks()[i] != 1) {
                    success = 0;
                    break;
                }
            }
            if ((checkWeights(threadState) + threadState.getMyWeight()) > Capacity) {
                success = 0;
            }

            if (success) {
                break;
            } else {
                printf("[Wątek %d - main] zasypiam sobie... zzz... [zegar = %d]\n", threadState.getRank(),
                       clockLamport);

                pthread_cond_wait(&cond, &mutexClock);
            }


        } while (1);
        // mutex unlock
        pthread_mutex_unlock(&mutexClock);

        printf("[Wątek %d - main] wszytskie wątki odebrały moją wiadomość wiadomości. [zegar = %d] Pozdrawiam, watek %d\n",
               threadState.getRank(), clockLamport, threadState.getRank());

        printf("[Wątek %d - main] wyzerowuje tablice ack i wejeżdza do góry. SEKCJA KRYTYCZNA [zegar = %d]\n",
               threadState.getRank(), clockLamport);
        // wyzerowanie ACK- mutex niepotrzebny, bo póki nie wyślę nowego REQUESTA, to nikt nie odpowie ACK
        for (i = 0; i < threadState.getSize(); i++) {
            auto vec=threadState.getTabAcks();
            vec[i]=0;
            threadState.setTabAcks(vec);
        }
        threadState.getTabAcks()[threadState.getRank()] = 1; //set ack to 1 from yourself
        // GO!
        printf("\n[Wątek %d - main] wjeżdzam do góry przez %d sekund [zegar = %d]\n\n", threadState.getRank(), GOUPTIME,
               clockLamport);
        sleep(GOUPTIME);

        // send RELEASE
        pthread_mutex_lock(&mutexClock);
        msg[0] = clockLamport;
        printf("\n[Wątek %d - main] KONIEC SEKCJI KRYTYCZNEJ [zegar = %d]\n\n", threadState.getRank(), clockLamport);

        for (i = 0; i < threadState.getSize(); i++) {
            if (i != threadState.getRank()) // do not send to yourself
            {
                MPI_Send(msg, MSG_SIZE, MPI_INT, i, TAG_RELEASE, MPI_COMM_WORLD);
            }
        }
        printf("[Wątek %d - main] wysyłał release synał do wszystkich i zjeżdża na dół. [zegar = %d]\n",
               threadState.getRank(),
               clockLamport);

        // sleep random przy zjeździe
        printf("[Wątek %d - main] usuwa swoje zgłoszenie ze swojej kolejki. [zegar = %d]\n", threadState.getRank(),
               clockLamport);

        //  usun swoje zadanie z kolejki
        int idToRemove = threadState.getRank();
        auto vec = threadState.getQueue();
        vec.erase(
                remove_if(threadState.getQueue().begin(), threadState.getQueue().end(),
                          [&idToRemove](QueueElement const &queue_element) {
                              return queue_element.getId() == idToRemove;
                          }),
                threadState.getQueue().end());
        threadState.setQueue(vec);
        sort(threadState.getQueue().begin(), threadState.getQueue().end());
        pthread_mutex_unlock(&mutexClock);

        int randomTime = 8 - (rand() % 7);
        printf("[Wątek %d - main] zjedża z góry przez %d sekund................ [zegar = %d]\n", threadState.getRank(),
               randomTime,
               clockLamport);
        sleep(randomTime); // czy to jest potzrebne?
        printf("[Wątek %d - main] zjechał i znowy  ustawia się do kolejki narciarzy. [zegar = %d]\n",
               threadState.getSize(),
               clockLamport);

    }
    return NULL;
}


int main(int argc, char **argv) {
    srand(time(NULL));
    int rank, size;

    int provided = 0;
    MPI_Init_thread(&argc, &argv, MPI_THREAD_MULTIPLE, &provided);
    if (provided < MPI_THREAD_MULTIPLE) {
        printf("ERROR: The MPI library does not have full thread support\n");

    } else {
        printf("Full support for multiple threads!\n");
    }


    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    threadState.setRank(rank);
    threadState.setSize(size);
    vector<QueueElement> queue;
    threadState.setQueue(queue);
    srand(rank);
    threadState.setMyWeight(70 + (30 - (rand() % 60)));
    int i;
    for (i = 0; i < threadState.getSize(); i++) {
        auto vec =threadState.getTabAcks();
        vec[i] = 0;
        threadState.setTabAcks(vec);
    }
    auto vec2 = threadState.getTabAcks();
    vec2[threadState.getRank()] = 1; // set ack from yourself to 1
    threadState.setTabAcks(vec2);

    printf("Wątek %d zainicjował zmienne (waga = %d) i rozpocząl działnie.\n", threadState.getRank(),
           threadState.getMyWeight());
    pthread_t watek1, watek2;
    pthread_create(&watek1, NULL, receiveAndSendAck, &threadState);
    pthread_create(&watek2, NULL, mainSkiThread, &threadState);
    pthread_join(watek1, NULL);
    pthread_join(watek2, NULL);
    pthread_mutex_destroy(&mutexClock);
    pthread_cond_destroy(&cond);
    MPI_Finalize();
    printf("Koniec programu");
    return 0;
}