/******************************************************************
 * The helper file that contains the following helper functions:
 * check_arg - Checks if command line input is a number and returns it
 * sem_create - Create number of sempahores required in a semaphore array
 * sem_init - Initialise particular semaphore in semaphore array
 * sem_wait - Waits on a semaphore (akin to down ()) in the semaphore array
 * sem_signal - Signals a semaphore (akin to up ()) in the semaphore array
 * sem_close - Destroy the semaphore array
 ******************************************************************/

#include "helper.h"
#include <ctime>

# define SEM_TIMEOUT 20 // timeout for a blocking semaphore

int check_arg (char *buffer)
{
  int i, num = 0, temp = 0;
  if (strlen (buffer) == 0)
    return -1;
  for (i=0; i < (int) strlen (buffer); i++)
  {
    temp = 0 + buffer[i];
    if (temp > '9' || temp < '0')
      return -1;
    num += pow (10, strlen (buffer)-i-1) * (buffer[i] - '0');
  }
  return num;
}


/* 
   Create a set of semaphores of num numbers
   Returns the id of this sem set if succeed, otherwiese -1
*/
int sem_create (key_t key, int num)
{
  int id;
  if ((id = semget (key, num,  0666 | IPC_CREAT | IPC_EXCL)) < 0)
    return -1;
  return id;
}


/* 
   Initilise a semaphore (of sem id = num) within a semaphore set( set id = id), 
   seting the semephore's count = value
   Returns 0 if successfully initilised. Should be in a single sperate thread
 */
int sem_init (int id, int num, int value)
{
  union semun semctl_arg;
  semctl_arg.val = value;
  if (semctl (id, num, SETVAL, semctl_arg) < 0)
    return -1;
  return 0;
}


/* 
   Decrement the num-th semephore within the sem set (with ID=id)
   The thread waits (for at most 20 sec) untill when sem's count >= 1, then sem's count--
   retruns 0 if successful
 */
int sem_wait (int id, short unsigned int num)
{
  // SEM_UNDO flag: when the program exits the kernal automatically undos
  // your changes under this semaphore of ID=id
  struct sembuf op[] = {
    {num, -1, SEM_UNDO}
  };
  
  // Represting the 20sec timeout
  struct timespec waitingTime[] = {SEM_TIMEOUT,0};
  
  return semtimedop(id,op,1,waitingTime); // semaphore blocks at most 20 sec
}


/* 
   Release the resources of semaphore (sem id = num) in sem set (sem set id = id)
   then sem's count ++ 
*/
void sem_signal (int id, short unsigned int num)
{
  struct sembuf op[] = {
    {num, 1, SEM_UNDO}
  };
  semop (id, op, 1);
}


/* 
   Destory the semaphore set with ID=id.
   return 0 if successfully destores
 */
int sem_close (int id)
{
  if (semctl (id, 0, IPC_RMID, 0) < 0)
    return -1;
  return 0;
}


/*===== Job =====*/
Job::Job(int id, int duration):jobID(id),duration(duration){}
