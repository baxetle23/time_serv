#pragma once
#include <sys/shm.h>
#include <sys/ipc.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/sem.h>
#include <fcntl.h>
#include <thread>
#include <iostream>
#include <cstdio>
#include <cstring>
#include <string>
#include <vector>
#include <unistd.h>
#include <random>
#include <ctime>
#include <time.h>
#include "../include/shared_memory.h"
#include "../include/master.h"
#include "../include/client.h"
#include "../include/semaphore.h"
#include "../include/request_handler.h"
#include "../include/log_duration.h"

#define CLIENTS_NUM 1
#define SIZE_SHARED_MEMORY 100
#define SIZE_MESSAGE 100

typedef struct s_arg {
   Master      *master;
   Slave       *slave;
}  t_arg;

//pipe
void *SlaveExecuteRequest(Slave *slave);
void *Requests(void *arguments);

//shm
void ChildWriteSHM(Slave& process);
void *TreadReadFromSHM(void *arguments);
