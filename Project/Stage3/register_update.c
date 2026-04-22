#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <unistd.h>
#include <sys/shm.h>
#include <sys/types.h>
#include <pthread.h>
#include <semaphore.h>

#include "sem.h"
#include "em_registers.h"

#define KEY 0x12345 // Would retrieve the same if not detached

#define max(a, b) (a > b)? a: b
#define MAX_VOLTAGE 440
#define MIN_VOLTAGE 420
#define MIN_CURRENT 50
#define MAX_CURRENT 70
#define MAX_KW      100
#define MIN_KW      80
#define VOLT_TH     445
#define MAX_FREQ    62
#define MIN_FREQ    45

#define MAX_SIZE    4

static pthread_mutex_t mutex;
static pthread_mutex_t mutex_max;
static pthread_cond_t count_threshold_cv;
static sem_t sem;
static int count = 0, i_avg[MAX_SIZE], v_avg[MAX_SIZE];

void *calc_avg_max(void *arg)
{
    int sumi = 0, sumv = 0, i;
    struct em_registers *reg;

    if (arg == NULL)
    {
        printf("Invalid arg\n");
        return NULL;
    }
    reg = arg;

    while (1)
    {
        // TODO 18: Take the appropriate lock to safely read max registers
        pthread_mutex_lock(&mutex_max);
	// TODO 19: Wait for the conditional signal for calc_max thread
        while(count%4 != 0 || count == 0)
		pthread_cond_wait(&count_threshold_cv,&mutex_max);
	for (i = 0; i < MAX_SIZE; i++)
        {
            sumi += i_avg[i];
            sumv += v_avg[i];
        }
        count = 0;

        // TODO 20: Take the appropriate lock to update the avg_max registers
	reg->i_avg_max = sumi / MAX_SIZE;
        reg->vavg_max = sumv / MAX_SIZE;
        printf("I Avg Max = %d, V Avg Max = %d\n", reg->i_avg_max, reg->vavg_max);
        sumi = sumv = 0;
	pthread_mutex_unlock(&mutex_max);
    }
    return NULL;
}
 
void *calc_max(void *arg)
{
    struct em_registers *reg;

    if (arg == NULL)
    {
        printf("Argument is null\n");
        return NULL;
    }
    reg = arg;

    while (1)
    {
        // TODO 15: Wait for the registers to be updated
        sem_wait(&sem);
	// TODO 16: Get the appropriate mutex to safely read the registers
        pthread_mutex_lock(&mutex);
	reg->ia_max = max(reg->ia_max, reg->ia);
        reg->ib_max = max(reg->ib_max, reg->ib);
        reg->ic_max = max(reg->ic_max, reg->ic);
        reg->va_max = max(reg->va_max, reg->va);
        reg->vb_max = max(reg->vb_max, reg->vb);
        reg->vc_max = max(reg->vc_max, reg->vc);
        printf("Max so far is\n");
        printf("Va Max = %u, Vb Max = %u, Vc Max = %u\n", reg->va_max, reg->vb_max, reg->vc_max);
        printf("Ia Max = %u, Ib Max = %u, Ic Max = %u\n", reg->ia_max, reg->ib_max, reg->ic_max);
	
	pthread_mutex_lock(&mutex_max);	
	if (count < MAX_SIZE)
		{
			i_avg[count] = reg->ia_max;
			v_avg[count] = reg->va_max;
		}
        count++;
		sleep(2);
        // TODO 17: Notify the calc_avg_max thread when count reaches 4
    	if(count%4 == 0){
		pthread_cond_signal(&count_threshold_cv);
	}
	pthread_mutex_unlock(&mutex_max);
	pthread_mutex_unlock(&mutex);
    }
    return NULL;
}

// TODO 9: Update the signature as thread requirement
void* update_registers(void *regs)
{
    struct em_registers *reg;
    if (regs == NULL)
    {
        printf("Invalid argument\n");
        return;
    }
    reg = (struct em_registers*) regs;
	// TODO 9A: Uncomment the below for thread to run forever
    while (1)
    {
        srand(time(0));
        // TODO 13: Use appropriate mechanism to synchronize the access to registers
	pthread_mutex_lock(&mutex);
	reg->va = (rand() % (MAX_VOLTAGE - MIN_VOLTAGE + 1)) + MIN_VOLTAGE;
        reg->vb = (rand() % (MAX_VOLTAGE - MIN_VOLTAGE + 1)) + MIN_VOLTAGE;
        reg->vc = (rand() % (MAX_VOLTAGE - MIN_VOLTAGE + 1)) + MIN_VOLTAGE;
        reg->freq = (rand() % (MAX_FREQ - MAX_FREQ + 1)) + MIN_FREQ;
        reg->ia = (rand() % (MAX_CURRENT - MIN_CURRENT + 1)) + MIN_CURRENT;
        reg->ib = (rand() % (MAX_CURRENT - MIN_CURRENT + 1)) + MIN_CURRENT;
        reg->ic = (rand() % (MAX_CURRENT - MIN_CURRENT + 1)) + MIN_CURRENT;
        reg->kw_a = (rand() % (MAX_KW - MIN_KW + 1)) + MIN_KW;
        reg->kw_b = (rand() % (MAX_KW - MIN_KW + 1)) + MIN_KW;
        reg->kw_c = (rand() % (MAX_KW - MIN_KW + 1)) + MIN_KW;
        reg->va_th = VOLT_TH;
        reg->vb_th = VOLT_TH;
        reg->vc_th = VOLT_TH;
        // TODO 14: Notify calc_max thread of this update
	sem_post(&sem);
	pthread_mutex_unlock(&mutex);
	sleep(5);
    }
}

void update_shm(struct em_registers *reg, int sem_id, char *shm_addr)
{
    while (1) 
    {
        // TODO 8: Remove from here and convert into thread
        //update_registers(reg);
		// TODO 5A: Acquire the semaphore & update the shared memory
		binary_semaphore_wait(sem_id);
		memcpy(shm_addr,reg,sizeof(struct em_registers));
		// TODO 5B: Release the semaphore after updating the shared memory
	binary_semaphore_post(sem_id);
        printf("Updated...\n");
        sleep(5);
    }
}

void *shm_allocate(key_t key, size_t shm_size,
        int flags, int *shm_id, void *addr)
{
    // TODO 2: Get and attach the shared memory
    *shm_id = shmget(key,shm_size,flags);
    printf("The segment id: %d (0x%X)\n", *shm_id, *shm_id);

	// TODO 2A Attach the shared memory segment and return the pointer to it
    char* ptr = (char*)shmat(*shm_id,addr,0);
    return (void*) ptr;
}

int main()
{
    int shm_id;
    char *shm_addr = NULL;
    struct em_registers reg = {0};
    const int shm_size = sizeof(struct em_registers);
    int sem_id;
    pthread_t update_thid, calc_max_thid;

    //Init all global mutex and semaphores for STage5
    pthread_mutex_init(&mutex,NULL);
    pthread_mutex_init(&mutex_max,NULL);
    sem_init(&sem,0,0);
    pthread_cond_init(&count_threshold_cv,NULL);

    // TODO 1: Allocate the shared memory segment
    shm_addr = shm_allocate(KEY,shm_size,0666,&shm_id,NULL);
    printf("Shared memory attached at address %p\n", shm_addr);

    // TODO 3: Allocate the binary semaphore
    sem_id = binary_semaphore_get(KEY,0666); 
    printf("Semaphore created with id: %d\n", sem_id);

    // TODO 4: Initialize the binary semaphore
    //binary_semaphore_set(semid);

    // TODO 10: Create the threads for update_registers & calc_max
    if(pthread_create(&update_thid,NULL,update_registers,(void *)&reg) != 0){
	    perror("Update register thread creation failed");
	    return -1;
    }
    if(pthread_create(&calc_max_thid,NULL,calc_max,(void *)&reg) != 0){
	    perror("Update register thread creation failed");
	    return -1;
    }
    // TODO 12: Create the thread for calc_avg_max
    pthread_t calc_max_avg_thid;
    if(pthread_create(&calc_max_avg_thid,NULL,calc_avg_max,(void *)&reg) != 0){
	    perror("Calc max average thread creation failed");
	    return -1;
    }
    update_shm(&reg, sem_id, shm_addr);

    // TODO 11: Join the threads
    pthread_join(update_thid,NULL);
    pthread_join(calc_max_thid,NULL);
    pthread_join(calc_max_avg_thid,NULL);
    // TODO 6: Detach the shared memory segment
    shmdt(shm_addr);
    printf("Shared memory detached\n");
    // TODO 7: Deallocate the shared memory segment
    shmctl(shm_id,IPC_RMID,NULL);
    return 0;
}
