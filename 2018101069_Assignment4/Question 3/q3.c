#define _POSIX_C_SOURCE 199309L
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <wait.h>
#include <limits.h>
#include <fcntl.h>
#include <time.h>
#include <pthread.h>
#include <inttypes.h>
#include <math.h>
int N, M, K, m;
time_t begin;
struct rider
{
    int maxwaittime;
    int Ridetime;
    int Cabtype;
    int Arrivaltime;
    int End;
    int readytopay;
    pthread_mutex_t riderlock;
};
struct cab
{
    int state;
    pthread_mutex_t cablock;
};
struct cab cabdriver[10000];
struct rider riders[10000];
void *cabsthread(void *a)
{
}
void Ridend(int i, int j)
{
    sleep(riders[i].Ridetime);
    if (cabdriver[j].state == 3)
    {
        cabdriver[j].state = 2;
    }
    else
    {
        cabdriver[j].state = 0;
    }
    riders[i].readytopay = 1;
}
void *paymentserver(void *a)
{
    int j = *(int *)a;
    while (1)
    {
        for (int i = 0; i < M; i++)
        {
            pthread_mutex_lock(&riders[i].riderlock);
            if (riders[i].readytopay == 1)
            {
                printf("Payment done by %dth rider on %dth server\n", i, j);
                fflush(stdout);
                riders[i].End = 1;
                riders[i].readytopay = 0;
                pthread_mutex_unlock(&riders[i].riderlock);
                sleep(2);
                m--;
            }
            pthread_mutex_unlock(&riders[i].riderlock);
        }
    }
}
// wait state is 0
// onride Poolfull 3
//onride Pool One 2
//On ride premier 1
void *ridrs(void *a)
{
    time_t start, end;
    int i = *(int *)a;
    struct timespec ts;
    clock_gettime(CLOCK_MONOTONIC, &ts);
    srand((time_t)ts.tv_nsec);
    riders[i].Arrivaltime = 1 + rand()%10;
    riders[i].Cabtype = rand() % 2;
    riders[i].maxwaittime = 1 + rand()%10;
    riders[i].Ridetime = 1 + rand()%10;
    riders[i].readytopay = 0;
    sleep(riders[i].Arrivaltime);
    printf("%d rider arrived after %d time with %d maxwaittime \n", i, riders[i].Arrivaltime,riders[i].maxwaittime);
    fflush(stdout);
    time(&start);
    while (1)
    {
        double elapsed;
        time(&end);
        elapsed = difftime(end, start);
        if (elapsed > riders[i].maxwaittime)
        {
            m--;
            printf("%d rider left after %d seconds\n", i, riders[i].maxwaittime);
            fflush(stdout);
            riders[i].readytopay = 0;
            riders[i].End = 1;
            pthread_exit(NULL);
        }
        for (int j = 0; j < N; j++)
        {
            pthread_mutex_lock(&cabdriver[j].cablock);
            if (riders[i].Cabtype == 0)
            {
                if (cabdriver[j].state == 0)
                {
                    cabdriver[j].state = 1;
                    printf("%d driver accepted rider %d Premier will end after %d seconds\n", j, i, riders[i].Ridetime);
                    fflush(stdout);
                    pthread_mutex_unlock(&cabdriver[j].cablock);
                    Ridend(i, j);
                    printf("%dth Rider's journey ended\n", i);
                    fflush(stdout);
                    pthread_exit(NULL);
                }
            }
            else if (riders[i].Cabtype == 1)
            {
                if (cabdriver[j].state == 2)
                {
                    printf("%d driver accepted rider %d Pool with 2 will end after %d seconds \n", j, i, riders[i].Ridetime);
                    fflush(stdout);
                    cabdriver[j].state = 3;
                    pthread_mutex_unlock(&cabdriver[j].cablock);
                    Ridend(i, j);
                    printf("%dth Rider's journey ended\n", i);
                    fflush(stdout);
                    pthread_exit(NULL);
                }
            }
            pthread_mutex_unlock(&cabdriver[j].cablock);
        }
        for (int j = 0; j < N; j++)
        {
            if (riders[i].Cabtype == 1)
            {
                pthread_mutex_lock(&cabdriver[j].cablock);
                if (cabdriver[j].state == 0)
                {
                    printf("%d driver accepted rider %d Pool with 1 will end after %d seconds \n", j, i, riders[i].Ridetime);
                    fflush(stdout);
                    cabdriver[j].state = 2;
                    pthread_mutex_unlock(&cabdriver[j].cablock);
                    Ridend(i, j);
                    printf("%dth Rider's journey ended\n", i);
                    fflush(stdout);
                    pthread_exit(NULL);
                }
                else if (cabdriver[j].state == 2)
                {
                    printf("%d driver accepted rider %d Pool with 2 will end after %d seconds \n", j, i, riders[i].Ridetime);
                    fflush(stdout);
                    cabdriver[j].state = 3;
                    pthread_mutex_unlock(&cabdriver[j].cablock);
                    Ridend(i, j);
                    printf("%dth Rider's journey ended\n", i);
                    fflush(stdout);
                    pthread_exit(NULL);
                }
                pthread_mutex_unlock(&cabdriver[j].cablock);
            }
        }
    }
}
int main()
{
    time(&begin);
    printf("No of Taxi:");
    scanf("%d", &N);
    printf("No of Riders:");
    scanf("%d", &M);
    printf("No of Payment servers:");
    scanf("%d", &K);
    m = M;
    pthread_t tid1[M + 1];
    pthread_t tid2[N + 1];
    pthread_t tid3[K + 1];
    for (int i = 0; i < N; i++)
    {
        int *temp = (int *)malloc(sizeof(int *));
        *temp = i;
        pthread_create(&tid1[i], NULL, cabsthread, temp);
    }
    for (int i = 0; i < M; i++)
    {
        int *temp = (int *)malloc(sizeof(int *));
        *temp = i;
        pthread_create(&tid2[i], NULL, ridrs, temp);
    }
    for (int i = 0; i < K; i++)
    {
        int *temp = (int *)malloc(sizeof(int *));
        *temp = i;
        pthread_create(&tid3[i], NULL, paymentserver, temp);
    }
    while (m)
    {
    }
}