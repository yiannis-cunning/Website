#include <pthread.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static uint64_t tallys[4] = {0};


// 0, 1, 2, 3
void *run(void *arg){
    //printf("Running thread %d\n", *(int *)arg);
    int tid = *(int *)arg;

    //printf("Thread %d: i = %d-%d, j = %d - %d\n", tid, 10000*(tid%2 == 1), 10000 + 10000*(tid%2 == 1), 10000*(tid <= 1), 10000 + 10000*(tid <= 1));
    for(int i = 10000*(tid%2 == 1); i <10000 + 10000*(tid%2 == 1); i+=1){
        for(int j = 10000*(tid <= 1); j < 10000 + 10000*(tid <= 1); j += 1){
            if(j == i || 20000-j == i){
                tallys[tid] += i + j;
            }
        }
    }
    return NULL;
}

pthread_t *new_thread(int id) {
    pthread_attr_t attr;
    pthread_attr_init(&attr);
    pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);

    int * arg = (int *) malloc(sizeof(int));
    *arg = id;
    pthread_t *thread = (pthread_t *)malloc(sizeof(pthread_t));
    int rc = pthread_create(thread, &attr, run, arg);
    
    pthread_attr_destroy(&attr);
    return thread;
}



int main(){
    // pthread_create()
    // pthread_join()
    // pthread_exit()
    // pthread_detach()
    if(1){

        pthread_t *threads[4];
        for (int i=0; i < 4; ++i) {
            threads[i] = new_thread(i);
            if(threads[i] == NULL){
                exit(1);
            }
        }
        for(int i =0; i < 4; i++){
            pthread_join(*threads[i], NULL);
        }
        uint64_t sm = tallys[0] + tallys[1] + tallys[2] + tallys[3];
        printf("ANS: %ld\n", sm);

    }
    else{
        uint64_t ans = 0;
        for(int i = 0; i <20000; i+=1){
            for(int j = 0; j < 20000; j += 1){
                if(j == i || 20000-j == i){
                    ans += i + j;
                }
            }
        }
        printf("ANS: %ld\n", ans);
        return ans;
    }

    return 1;
}