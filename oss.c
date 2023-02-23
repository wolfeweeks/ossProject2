/**
 * @file oss.c
 * @author Wolfe Weeks
 * @date 2023-01-26
 */

#include <stdio.h>
#include <unistd.h>
#include <getopt.h>
#include <sys/wait.h>
#include <string.h>

#include "shared_memory.h"

#define BLOCK_SIZE 4096

int main(int argc, char* argv[]) {
  // // Initialize variables to hold command line options
  // int proc = 1, simul = 1, iter = 1;

  // // iterStr used for execl command
  // char iterStr[2];

  // // Use getopt() function to parse command line options
  // int opt;
  // while ((opt = getopt(argc, argv, "n:s:t:h")) != -1) {
  //   switch (opt) {
  //   case 'n':
  //     proc = atoi(optarg);
  //     break;
  //   case 's':
  //     simul = atoi(optarg);
  //     break;
  //   case 't':
  //     iter = atoi(optarg);
  //     break;
  //   case 'h':
  //     printf("Usage: oss [-h] [-n proc] [-s simul] [-t iter]\n");
  //     printf("\t-h (optional) shows a help message\n");
  //     printf("\t-n (optional) is the total number of child processes to create\n");
  //     printf("\t-s (optional) is the maximum number of concurrent child processes\n");
  //     printf("\t-t (optional) is the number of iterations each child process should do\n");
  //     return 0;
  //   default:
  //     printf("Invalid option\n");
  //     return 1;
  //   }
  // }

  // // convert iteration number to a string for execl command
  // snprintf(iterStr, sizeof(iterStr), "%d", iter);

  // initialize the clock that will be put in shared memory
  int clock[] = { 10, 20 };

  // get shared memory block for clock
  char* block = attach_memory_block("README.txt", sizeof(int) * 2);
  if (block == NULL) {
    printf("ERROR: couldn't get shared memory block\n");
    exit(1);
  }

  printf("Initializing clock in shared memory\n");
  // put clock into shared memory
  memcpy(block, clock, sizeof(int) * 2);

  detach_memory_block(block);

  // fork a child process
  pid_t pid = fork();
  if (pid < 0) {
    printf("Fork failed!\n");
    exit(1);
  } else if (pid == 0) {
    execl("./worker", "./worker", NULL);
  } else {
    wait(0);
    if (destroy_memory_block("README.txt")) {
      printf("Destroyed block\n");
    } else {
      printf("Couldn't destroy block\n");
    }
  }

  // int running = 0;
  // int remaining = proc;

  // while (remaining > 0) {
  //   // if there is room for more child processes
  //   if (running < simul) {
  //     remaining--;
  //     running++;

  //     // fork a child process
  //     pid_t pid = fork();

  //     if (pid < 0) {
  //       // fork failed
  //       printf("Fork failed!\n");
  //       exit(1);
  //     } else if (pid == 0) {
  //       // exec .worker process
  //       execl("./worker", "./worker", iterStr, NULL);
  //     }
  //   } else {
  //     // wait for a child to finish if there is no more room
  //     wait(0);
  //     running--;
  //   }
  // }

  // // wait for all remaining child processes to finish
  // // idea found from here: https://stackoverflow.com/questions/19461744/how-to-make-parent-wait-for-all-child-processes-to-finish
  // while (wait(0) > 0);

  return 0;
}
