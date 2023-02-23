/**
 * @file worker.c
 * @author Wolfe Weeks
 * @date 202-01-26
 */

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdbool.h>

#include "shared_memory.h"

void terminate(int* block, int clockSec, int clockNano, int quitSec, int quitNano) {
  printf("WORKER PID:%d PPID:%d SysClockS:%d SysClockNano:%d TermTimeS:%d TermTimeNano:%d\n", getpid(), getppid(), clockSec, clockNano, quitSec, quitNano);
  printf("--Terminating\n");
  detach_memory_block(block);
  exit(1);
}

int main(int argc, char* argv[]) {

  // check if command line argument is passed
  if (argc != 3) {
    printf("Error: missing command line argument for time limit\n");
    return 1;
  }

  // convert command line arguments to integers
  int limit[] = { atoi(argv[1]) , atoi(argv[2]) };

  // attach to the shared memory clock initialized in oss.c
  int* block = attach_memory_block("README.txt", sizeof(int) * 2);
  if (block == NULL) {
    printf("ERROR: couldn't get block\n");
    exit(1);
  }

  // store the contents of shared memory into clock array
  int clock[2];
  memcpy(clock, block, sizeof(int) * 2);

  int quitTime[2] = { limit[0] + clock[0], limit[1] + clock[1] };
  if (quitTime[1] >= 1000000000) {
    quitTime[0] += 1;
    quitTime[1] -= 1000000000;
  }

  printf("WORKER PID:%d PPID:%d SysClockS:%d SysClockNano:%d TermTimeS:%d TermTimeNano:%d\n", getpid(), getppid(), clock[0], clock[1], quitTime[0], quitTime[1]);
  printf("--Just Starting\n");

  int prevSeconds = clock[0];
  int elapsedSeconds = 0;

  while (true) {
    memcpy(clock, block, sizeof(int) * 2); //get clock value

    if (clock[0] > quitTime[0]) terminate(block, clock[0], clock[1], quitTime[0], quitTime[1]);

    if (clock[0] == quitTime[0] && clock[1] >= quitTime[1]) terminate(block, clock[0], clock[1], quitTime[0], quitTime[1]);

    if (clock[0] != prevSeconds) {
      prevSeconds = clock[0];
      elapsedSeconds += 1;
      printf("WORKER PID:%d PPID:%d SysClockS:%d SysClockNano:%d TermTimeS:%d TermTimeNano:%d\n", getpid(), getppid(), clock[0], clock[1], quitTime[0], quitTime[1]);
      printf("--%d seconds have passed since starting\n", elapsedSeconds);
    }
  }

  // // loop for the specified number of iterations
  // int i;
  // for (i = 1; i <= iterations; i++) {
  //   printf("WORKER PID: %d PPID: %d Iteration: %d before sleeping\n", getpid(), getppid(), i);
  //   sleep(1);
  //   printf("WORKER PID: %d PPID: %d Iteration: %d after sleeping\n", getpid(), getppid(), i);
  // }

  return 0;
}
