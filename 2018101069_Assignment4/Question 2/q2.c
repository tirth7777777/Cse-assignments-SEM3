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
int waitingtime[10000];
int K;
int M, N;
struct arg
{
    int i;
};
struct robots
{
    int vessels, cap;
};
struct table
{
    int state;
    int slots;
    int vessels;
    int cap;
    int status;
    pthread_mutex_t tablelock;
};
pthread_mutex_t student;
struct table tables[10000];
struct robots robot[10000];
void *studentwait(void *a)
{
    -sleep(10);
    struct timespec ts;
    clock_gettime(CLOCK_MONOTONIC, &ts);
    srand((time_t)ts.tv_nsec);
    int arrival = rand() % 10;
    sleep(arrival);
    int j = *(int *)a;

    printf("Student %d arrived at %d seconds\n", j, arrival);
    while (1)
    {
        for (int i = 0; i < N; i++)
        {
            pthread_mutex_lock(&tables[i].tablelock);
            if (tables[i].state && tables[i].slots > 0 && tables[i].status == 1)
            {
                printf("%d th student got slot ALLOCATED at %dth table \n", j, i);
                fflush(stdout);
                printf("%d th student ate at %dth table \n", j, i);
                fflush(stdout);
                tables[i].slots--;
                tables[i].cap--;
                K--;
                pthread_mutex_unlock(&tables[i].tablelock);
                pthread_exit(NULL);
            }
            pthread_mutex_unlock(&tables[i].tablelock);
        }
    }
    pthread_exit(NULL);
}
void readytoserve(int j)
{
    int nofvessel = robot[j].vessels;
    printf("%d no of vessels created by %dth chef\n", nofvessel, j);
    fflush(stdout);
    while (nofvessel)
    {
        for (int i = 0; i < N; i++)
        {
            pthread_mutex_lock(&tables[i].tablelock);
            if (tables[i].cap <= 0 && tables[i].state == 0)
            {
                printf("vessel created by %dth chef assigned to %dth table\n", j, i);
                fflush(stdout);
                nofvessel--;
                tables[i].cap = robot[j].cap;
                tables[i].state = 1;
            }
            pthread_mutex_unlock(&tables[i].tablelock);
        }
    }
    return;
}
void *tabls(void *a)
{
    sleep(10);
    struct timespec ts;
    clock_gettime(CLOCK_MONOTONIC, &ts);
    srand((time_t)ts.tv_nsec);
    int i = *(int *)a;
    while (1)
    {
        while (tables[i].state == 0)
        {
            tables[i].status = 0;
        }
        if (tables[i].cap == 0)
        {
            tables[i].state = 0;
        }
        pthread_mutex_lock(&tables[i].tablelock);
        if (tables[i].cap > 0 && tables[i].slots == 0)
        {
            tables[i].slots = 1 + (rand() % tables[i].cap);
            printf("%d slots created by %dth table %d capacity\n", tables[i].slots, i, tables[i].cap);
            fflush(stdout);
            tables[i].status = 1;
            //    pthread_mutex_unlock(&tables[i].tablelock);
        }
        pthread_mutex_unlock(&tables[i].tablelock);
    }
    pthread_exit(NULL);
}
void *startcreating(void *a)
{
    sleep(10);
    struct timespec ts;
    clock_gettime(CLOCK_MONOTONIC, &ts);
    srand((time_t)ts.tv_nsec);
    int i = *(int *)a;
    while (1)
    {
        waitingtime[i] = 2 + rand() % 4;
        robot[i].vessels = 1 + rand() % 10;
        int min = 25 + rand() % 26;
        robot[i].cap = min > K ? K : min;
        sleep(waitingtime[i]);
        printf("%d th cook cooked in %d time\n", i, waitingtime[i]);
        fflush(stdout);
        readytoserve(i);
    }
    pthread_exit(NULL);
}

int main()
{
    printf("No of chefs:");
    scanf("%d", &M);
    printf("No of Tables:");
    scanf("%d", &N);
    printf("No of Students:");
    scanf("%d", &K);
    for (int i = 0; i < 10000; i++)
    {
        tables[i].cap = 0;
        tables[i].vessels = 0;
        tables[i].state = 0;
        tables[i].slots = 0;
        pthread_mutex_init(&tables[i].tablelock, NULL);
        robot[i].vessels = 0;
        robot[i].cap = 0;
    }
    pthread_t tid1[M + 1];
    pthread_t tid2[N + 1];
    pthread_t tid3[K + 1];
    pthread_t tid4[K + 1];
    for (int i = 0; i < M; i++)
    {
        int *temp = (int *)malloc(sizeof(int *));
        *temp = i;
        pthread_create(&tid1[i], NULL, startcreating, temp);
    }
    for (int i = 0; i < N; i++)
    {
        int *temp = (int *)malloc(sizeof(int *));
        *temp = i;
        pthread_create(&tid2[i], NULL, tabls, temp);
    }
    for (int i = 0; i < K; i++)
    {
        int *temp = (int *)malloc(sizeof(int *));
        *temp = i;
        pthread_create(&tid3[i], NULL, studentwait, temp);
    }
    while (K)
    {
    }
}
