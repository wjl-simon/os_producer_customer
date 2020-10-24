/******************************************************************
 * Header file for the helper functions. This file includes the
 * required header files, as well as the function signatures and
 * the semaphore values (which are to be changed as needed).
 ******************************************************************/


# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <sys/types.h>
# include <sys/ipc.h>
# include <sys/shm.h>
# include <sys/sem.h>
# include <sys/time.h>
# include <math.h>
# include <errno.h>
# include <string.h>
# include <pthread.h>
# include <ctype.h>
# include <iostream>
# include <queue>

using namespace std;

# define SEM_KEY 0x1790516 // The last 3 digits of my CID (though it's not hex)

/*===== Semaphore =====*/


union semun {
    int val;               /* used for SETVAL only */
    struct semid_ds *buf;  /* used for IPC_STAT and IPC_SET */
    ushort *array;         /* used for GETALL and SETALL */
};

int check_arg (char *);
int sem_create (key_t, int);
int sem_init (int, int, int);
int sem_wait (int id, short unsigned int num);
void sem_signal (int, short unsigned int);
int sem_close (int);


/*===== Job =====*/
struct Job
{
  int jobID; // the unique identifier of a job, staring form 1
  int duration; // the duration of sleeping
  
  Job(int id, int duration);
};


/*===== Producer/Consumer Arguments =====*/
struct ProConArg
{
  int id; // producer/consumer id, starting from 1
  int SETID;
  int proJobNum; // num of jobs that each producer should generate
  int maxQueueSize;
};
