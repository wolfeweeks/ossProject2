To compile this project, run the make command in this directory.

To run the project, run "./oss [-h] [-n proc] [-s simul] [-t iter]" where:
  -h (optional) shows a help message
  -n (optional) is the total number of child processes to create
  -s (optional) is the maximum number of concurrent child processes
  -t (optional) is the number of iterations each child process should do

Example 1: "./oss" (defaults -n -s and -t to 1)
Example 2: "./oss -n 8 -s 3 -t 3