#pragma once
#include <sys/shm.h>
#include <sys/ipc.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/sem.h>
#include <fcntl.h>
#include <pthread.h>
#include <thread>
#include <iostream>
#include <cstdio>
#include <cstring>
#include <string>
#include <vector>
#include <unistd.h>
#include <random>
#include <ctime>
#include "../include/shared_memory.h"
#include "../include/master.h"
#include "../include/client.h"

#define SIZE_MEMORY 100
#define CLIENTS_NUM 10
#define SIZE_MESSAGE 100

typedef struct s_arg {
   Master    *master;
   Slave *slave;
   pthread_t *tread;
}  t_arg;

//pipe
void *ChildReadWritePipeNonblock(Slave slave);
void *TreadWriteReadPipeNonblock(void *arguments);

//shm
void ChildWriteSHM(Slave& process);
void *TreadReadFromSHM(void *arguments);