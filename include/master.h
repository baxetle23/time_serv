#pragma once
#include <vector>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <sys/sem.h>
#include "shared_memory.h"
#include "client.h"

// #define Read            0
// #define Write           1
// #define ParentRead      read_pipe[0]
// #define ParentWrite     write_pipe[1]
// #define ChildRead       write_pipe[0]
// #define ChildWrite      read_pipe[1]

struct fdpipe {
    int read;
    int write;
};

class Master {
private:
    std::vector<Shared_memory> shared_memory_;
    std::vector<fdpipe> fdpipe_;
    size_t count_;


public:
    Master(size_t count);
    ~Master();
    //memory
    int InitMemory(size_t size_sigment);
    void ReadFromSHM();
    void ClearMemory();
    //process
    int InitProcesses(std::vector<ChildProc>& childs);
    int WaitAllProc() const;
    //message to procces
    int WriteToProcess(const char *str, int number_process);
    int ReadFromProcess(int number_process);

};