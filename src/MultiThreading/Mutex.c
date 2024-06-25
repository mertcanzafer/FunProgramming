#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

// Race Condition
//  Loop:
    // #1000000,r0
    // load mail ,r1
    // Context - switch --> if Threads are switched!!!!
    // add #1, r1
    // cmp r0,r1
    // bge Loop
    // store r1,mail

int mails = 0;
pthread_mutex_t mutex;


void* threadrun()
{
    for(int i = 0; i < 1000000; i++)
    {
        pthread_mutex_lock(&mutex);
        // Critical section begin
        mails += 1;
        // Critical seciton end
        pthread_mutex_unlock(&mutex);
    }
}

int main()
{
    int i;
    const int N = 6;
    pthread_t ts[N];
    void* status;
    pthread_mutex_init(&mutex,NULL);

    // Create threads
    for(i = 0; i < N; i++)
    {
        if(pthread_create(&ts[i],NULL,&threadrun,NULL) != 0)
        {
            return 1;
        }
    }
        
    // Wait the thread to finish its execution!!!

    for(i = 0; i < N; i++)
    {
        if(pthread_join(ts[i],&status) != 0)
        {
            return 2;
        }
    }

    pthread_mutex_destroy(&mutex);

    printf("Number of mails is %d\n",mails);

    return 0;
}
