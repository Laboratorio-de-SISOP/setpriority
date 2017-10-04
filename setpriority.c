#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <linux/sched.h>


int volatile running = 1;

void *run(void *data){
	
	while(running){}
	
	return NULL;
}


void print_sched(int policy){
	switch(policy){
		case SCHED_FIFO: 
			printf("SCHED_FIFO\n");
			break;
		case SCHED_RR: 
			printf("SCHED_RR\n");
			break;
		case SCHED_OTHER: 
			printf("SCHED_OTHER\n");
			break;
		default:
			printf("unknown\n");
	}
}

void setpriority(pthread_t *thr, int newpolicy, int newpriority){
	int policy, ret;
    struct sched_param param;
    pthread_t t;
    
    if(newpriority > sched_get_priority_max(newpolicy) ||
	   newpriority < sched_get_priority_min(newpolicy)){
		printf("Invalid priority: MIN: %d, MAX: %d", sched_get_priority_min(newpolicy), sched_get_priority_max(newpolicy));
	}

    pthread_getschedparam(*thr, &policy, &param);
    
    print_sched(policy);
    
    param.sched_priority = newpriority;
    
    ret = pthread_setschedparam(*thr, newpolicy, &param);

    if (ret != 0){
		perror("perror(): ");
	}
}

int main(int argc, char **argv){
    int timesleep;
    pthread_t thr;
    
    if (argc < 2){
		printf("usage: ./%s <execution_time>\n\n", argv[0]);
		exit(0);
	}
	
	/* Pega parâmetros */
	timesleep = atoi(argv[1]);
	
	pthread_create(&thr, NULL, run, NULL);
				
	setpriority(&thr, SCHED_FIFO, 1);
    
    sleep(timesleep);
    
    running = 0;
    
    pthread_join(thr, NULL);
    
	return 0;

}
