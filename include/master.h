#pragma once
#include <vector>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <sys/sem.h>
#include "shared_memory.h"
#include "client.h"

union semun {
    int val;
    struct semid_ds *buf;
    ushort *array;
};

struct fdpipe {
    int read;
    int write;
};

class Master {
private:
    std::vector<Shared_memory> shared_memory_;
    std::vector<fdpipe> fdpipe_;
    int semset_id;
    size_t count_;


public:
    Master(size_t count);
    ~Master();
    //memory
    int InitMemory(size_t size_sigment);
    void ReadFromSHM(ChildProc& child);
    void ClearMemory();
    //process
    int InitProcesses(std::vector<ChildProc>& childs);
    int WaitAllProc() const;
    //message to procces
    int WriteToProcess(const char *str, int number_process);
    int ReadFromProcess(int number_process);
    //sem
    int InitSem(size_t count_sem);
};