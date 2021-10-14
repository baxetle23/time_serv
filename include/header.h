#pragma once
#include <sys/shm.h>
#include <sys/ipc.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/sem.h>
#include <fcntl.h>
#include <pthread.h>
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


//pipe
void ChildReadWritePipeNonblock(ChildProc& process);
void MasterWriteReadPipeNonblock(Master& master, std::vector<ChildProc>& childs, int iterator);

//sem

//shm
void ChildWriteSHM(ChildProc& process);