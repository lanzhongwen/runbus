#include <fcntl.h>
#include <semaphore.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/shm.h>
#include <time.h>
#include <unistd.h>
#include "shared-data.h"

int main() {
  bool running = true;
  void* shm = NULL;
  struct SIpcSharedData* shared_data;
  int shmid = shmget((key_t)2200,
                     sizeof(struct SIpcSharedData),
                     0666|IPC_CREAT);
  if (shmid == -1) {
    perror("shmget failed");
    exit(-1);
  }
  printf("shmid %d\n", shmid);
  struct timespec t = {10, };
  nanosleep(&t, NULL);
  // link shared memory into current process space
  shm = shmat(shmid, 0, 0);
  if (shm == (void*)-1) {
    perror("shmat failed");
    exit(-2);
  }
  printf("Memory Attached At %X\n", (unsigned int64_t)shm);
  sem_t* semaphore = sem_open("MySemaphore", O_RDWR|O_CREAT, 0777);
  if (semaphore == SEM_FAILED) {
    perror("sem_open failed");
    exit(-3);
  }
  shared_data = (struct SIpcSharedData*)shm;
  bool once = true;
  int max = 3;
  while (running) {
    printf("shared_data->value = %d\n", shared_data->value);
    if (shared_data->value < max && once == true) {
      sem_wait(semaphore);
      ++shared_data->value;
      once = false;
      sem_post(semaphore);
    } else {
      if (shared_data->value >= max) {
        running = false;
        printf("max is hit and then turning off\n");
        break;
      }
      struct timespec t = {5, 0};
      nanosleep(&t, NULL);
    }
  }
  // Execute UE business
  sem_wait(semaphore);
  if (shared_data->value > 0) {
    printf("I am good: %d\n", shared_data->value);
  }
  sem_post(semaphore);
  // clean up
  if (shared_data->value <= 0) {
    sem_unlink("MySemaphore");
    sem_close(semaphore);
    if (shmdt(shm) == -1) {
      perror("shmdt failed");
      exit(-4);
    }
    if (shmctl(shmid, IPC_RMID, 0) == -1) {
      perror("shmctl failed");
      exit(-5);
    }
  }
  printf("Done");
  return 0;
}
