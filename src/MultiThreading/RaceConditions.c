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

void* threadrun()
{
    for(int i = 0; i < 1000000; i++)
        mails += 1;
}

int main()
{
    int i;
    pthread_t ts[2];
    void* status;

    // Create threads
    for(i = 0; i < 2; i++)
    {
        if(pthread_create(&ts[i],NULL,&threadrun,NULL) != 0)
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
