#define _POSIX_C_SOURCE 199309L //required for clock
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

int *shareMem(size_t size)
{
    key_t mem_key = IPC_PRIVATE;
    // printf("%ld\n",size);
    int shm_id = shmget(mem_key, size, IPC_CREAT | 0666);
    if(shm_id == -1)
    {
        perror("");
        exit(0);
    }
    return (int *)shmat(shm_id, NULL, 0);
}

int partition(int arr[], int low, int high)
{
    int pivot = arr[high];
    int i = (low - 1);

    for (int j = low; j <= high - 1; j++)
    {
        if (arr[j] < pivot)
        {
            i++;
            int t = arr[i];
            arr[i] = arr[j];
            arr[j] = t;
        }
    }
    int t = arr[i + 1];
    arr[i + 1] = arr[high];
    arr[high] = t;
    return (i + 1);
}

void quickSort(int arr[], int l, int r)
{
    if (l < r)
    {
        int pi = partition(arr, l, r);
        if (r - l + 1 <= 5)
        {
            int a[5], mi = INT_MAX, mid = -1;
            for (int i = l; i < r; i++)
            {
                int j = i + 1;
                for (; j <= r; j++)
                    if (arr[j] < arr[i] && j <= r)
                    {
                        int temp = arr[i];
                        arr[i] = arr[j];
                        arr[j] = temp;
                    }
            }
            return;
        }
        quickSort(arr, l, pi - 1);
        quickSort(arr, pi + 1, r);
    }
}
void concurrentquicksort(int *arr, int l, int r)
{
    if (l >= r)
        _exit(1);

    if (r - l + 1 <= 5)
    {
        int a[5], mi = INT_MAX, mid = -1;
        for (int i = l; i < r; i++)
        {
            int j = i + 1;
            for (; j <= r; j++)
                if (arr[j] < arr[i] && j <= r)
                {
                    int temp = arr[i];
                    arr[i] = arr[j];
                    arr[j] = temp;
                }
        }
        return;
    }
    if (l < r)
    {
        int pi = partition(arr, l, r);
        int pid1 = fork();
        int pid2;
        if (pid1 == 0)
        {
            //sort left half array
            concurrentquicksort(arr, l, pi - 1);
            _exit(1);
        }
        else
        {
            pid2 = fork();
            if (pid2 == 0)
            {
                concurrentquicksort(arr, pi + 1, r);
                _exit(1);
            }
            else
            {
                int status;
                waitpid(pid1, &status, 0);
                waitpid(pid2, &status, 0);
            }
        }
        return;
    }
}

struct arg
{
    int l;
    int r;
    int *arr;
};

void *threaded_quicksort(void *a)
{
    struct arg *args = (struct arg *)a;
    int l = args->l;
    int r = args->r;
    int *arr = args->arr;
    if (l > r)
        return NULL;

    if (r - l + 1 <= 5)
    {
        int a[5], mi = INT_MAX, mid = -1;
        for (int i = l; i < r; i++)
        {
            int j = i + 1;
            for (; j <= r; j++)
                if (arr[j] < arr[i] && j <= r)
                {
                    int temp = arr[i];
                    arr[i] = arr[j];
                    arr[j] = temp;
                }
        }
        return NULL;
    }
    if (l < r)
    {

        int pi = partition(arr, l, r);
        struct arg a1;
        a1.l = l;
        a1.r = pi - 1;
        a1.arr = arr;
        pthread_t tid1;
        pthread_create(&tid1, NULL, threaded_quicksort, &a1);

        struct arg a2;
        a2.l = pi + 1;
        a2.r = r;
        a2.arr = arr;
        pthread_t tid2;
        pthread_create(&tid2, NULL, threaded_quicksort, &a2);

        pthread_join(tid1, NULL);
        pthread_join(tid2, NULL);
    }
}

void runSorts(long long int n)
{

    struct timespec ts;

    //getting shared memory
    // printf("%lld\n",n);
    int *arr = shareMem(sizeof(int) * (n +1));
    for (int i = 0; i < n; i++)
        scanf("%d", arr + i);
int brr[n + 1];
    for (int i = 0; i < n; i++)
        brr[i] = arr[i];
quickSort(brr, 0, n - 1);

    printf("Running concurrent_quicksort for n = %lld\n", n);
    clock_gettime(CLOCK_MONOTONIC_RAW, &ts);
    long double st = ts.tv_nsec / (1e9) + ts.tv_sec;

    concurrentquicksort(arr, 0, n - 1);

    clock_gettime(CLOCK_MONOTONIC_RAW, &ts);
    long double en = ts.tv_nsec / (1e9) + ts.tv_sec;
    printf("time = %Lf\n", en - st);
    long double t1 = en - st;

    

    pthread_t tid;
    struct arg a;
    a.l = 0;
    a.r = n - 1;
    a.arr = brr;
    printf("Running threaded_concurrent_quicksort for n = %lld\n", n);
    clock_gettime(CLOCK_MONOTONIC_RAW, &ts);
    st = ts.tv_nsec / (1e9) + ts.tv_sec;

    pthread_create(&tid, NULL, threaded_quicksort, &a);
    pthread_join(tid, NULL);

    clock_gettime(CLOCK_MONOTONIC_RAW, &ts);
    en = ts.tv_nsec / (1e9) + ts.tv_sec;
    printf("time = %Lf\n", en - st);
    long double t2 = en - st;

    printf("Running normal_quicksort for n = %lld\n", n);
    clock_gettime(CLOCK_MONOTONIC_RAW, &ts);
    st = ts.tv_nsec / (1e9) + ts.tv_sec;

    quickSort(arr, 0, n - 1);

    clock_gettime(CLOCK_MONOTONIC_RAW, &ts);
    en = ts.tv_nsec / (1e9) + ts.tv_sec;
    printf("time = %Lf\n", en - st);
    long double t3 = en - st;

  //  printf("normal_quicksort ran:\n\t[ %Lf ] times faster than concurrent_quicksort\n\t[ %Lf ] times faster than threaded_concurrent_quicksort\n\n\n", t1 / t3, t2 / t3);
    shmdt((void *)arr);
    return;
}

int main()
{

    long long int n;
    //printf("No of elements");
    scanf("%lld", &n);
    runSorts(n);
    return 0;
}
