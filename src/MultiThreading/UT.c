#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

void* threadrun()
{
    printf("Thread workspace\n");
    sleep(3);
    printf("Ending thread\n");
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

    return 0;
}
