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

class Master {
private:
    std::vector<Shared_memory> shared_memory_;
    std::vector<ChildProc>& slaves_;
    int semset_id;
    size_t count_;


public:
    Master(size_t count, std::vector<ChildProc>& slaves);
    ~Master();
    //memory
    int InitMemory(size_t size_sigment);
    int ClearMemory();
    void ReadFromSHM(ChildProc& child);
    //process
    int InitProcesses(std::vector<ChildProc>& childs);
    int WaitAllProc() const;
    //message to procces
    int WriteToProcess(const char *str, int number_process, size_t size);
    int ReadFromProcess(int number_process, char *buffer, size_t buffer_size);
    //sem
    int InitSem(size_t count_sem);
};