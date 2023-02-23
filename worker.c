/**
 * @file worker.c
 * @author Wolfe Weeks
 * @date 202-01-26
 */

#include <stdio.h>
#include <unistd.h>

#include "shared_memory.h"

#define BLOCK_SIZE 4096

int main(int argc, char* argv[]) {

  // // check if command line argument is passed
  // if (argc != 2) {
  //   printf("Error: missing command line argument for number of iterations\n");
  //   return 1;
  // }

  // // convert command line argument to integer
  // int iterations = atoi(argv[1]);

  // attach to the shared memory clock initialized in oss.c
  int* block = attach_memory_block("README.txt", sizeof(int) * 2);
  if (block == NULL) {
    printf("ERROR: couldn't get block\n");
    exit(1);
  }

  // store the contents of shared memory into clock array
  int clock[2];
  memcpy(clock, block, sizeof(int) * 2);

  printf("Reading '%d:%d'\n", clock[0], clock[1]);

  detach_memory_block(block);

  // // loop for the specified number of iterations
  // int i;
  // for (i = 1; i <= iterations; i++) {
  //   printf("WORKER PID: %d PPID: %d Iteration: %d before sleeping\n", getpid(), getppid(), i);
  //   sleep(1);
  //   printf("WORKER PID: %d PPID: %d Iteration: %d after sleeping\n", getpid(), getppid(), i);
  // }

  return 0;
}
