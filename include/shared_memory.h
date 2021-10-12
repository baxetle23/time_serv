#pragma once
#include <sys/shm.h>
#include <sys/ipc.h>
#include <sys/types.h>
#include <iostream>

//add shared_ptr
class Shared_memory {
    int shmid;
    size_t size;
    void* shmptr;
public:
    //get memory FIX
    Shared_memory();
    //delete memory FIX
    ~Shared_memory();
    int createShm(int size);
    void freeShm();
    void* getShmptr(int id);
    size_t getMemId() const;
    size_t getSize() const;
    void* getShmptr();
};
