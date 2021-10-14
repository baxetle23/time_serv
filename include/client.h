#pragma once
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <string.h>

class ChildProc {
public:
    pid_t   pid;
    // Pipe for reading for subprocess
    int     read_pipe[2];
    // Pipe for writing to subprocess
    int     write_pipe[2];
    // Pointer to shm
    void*   ptr_shm;
    // Number proc in vector processov
    int     process_id;
    // Id semaphora
    int     semset_id;


    void WriteToSHM(const char *string, size_t size);
    int WriteToMaster(const char *string, size_t size);
    int ReadFromMaster(char *buffer, size_t size);  
};

