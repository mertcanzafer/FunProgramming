#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
// Race Condition
//  Loop:
    // mov #1000000,r0
    // load mail ,r1
    // Context - switch --> if Threads are switched!!!!
    // add #1, r1
    // cmp r0,r1
    // bge Loop
    // store r1,mail

// Peterson Algorithm  ---> Prevent Deadlock and enables mutual exclusion!

#define TRUE 1
#define FALSE 0

int mails = 0;
int turn; // whose turn among threads
int flag[2] = {FALSE,FALSE};

void* threadrun(void* arg)
{
    // Get the Thread id!!!
    int id = *((int*)arg);

    // Critical Section Start
    for(int i = 0; i < 1000000; i++)
    { 
        // Enter Critical Section

        flag[id] = TRUE;
        turn = 1-id;

        while(flag[1 - id] == 1 && turn == 1 - id); // Busy wait -> Block the other thread
        
        // Critical Section Start
          mails += 1;
        // Critical Section End

        flag[id] = FALSE;
    }
}

int main()
{
    int i;
    pthread_t ts[2];
    void* status;
    int ids[2] = {0,1}; // thread ids
    // Create threads
    for(i = 0; i < 2; i++)
    {
        if(pthread_create(&ts[i],NULL,&threadrun,&ids[i]) != 0)
        {
            return 1;
        }
    }
        
    // Wait the thread to finish its execution!!!

    for(i = 0; i < 2; i++)
    {
        if(pthread_join(ts[i],&status) != 0)
        {
            return 2;
        }
    }

    printf("Number of mails is %d\n",mails);

    return 0;
}
