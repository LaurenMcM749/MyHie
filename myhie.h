#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <limits.h>
#include <errno.h> 
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <fcntl.h> 
#include <sys/stat.h> 
#include <signal.h>
#include <poll.h>

#define N 101
#define READ_END 0
#define WRITE_END 1
#define LENGTH 200