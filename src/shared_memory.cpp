#include "../include/shared_memory.h"

Shared_memory::Shared_memory() : shmid(0), size(0), shmptr(nullptr) {
}
//delete memory FIX
Shared_memory::~Shared_memory() {
}
int Shared_memory::createShm(int size) {
    shmid = shmget(IPC_PRIVATE, size, 0600 | IPC_CREAT | IPC_EXCL);
    if(shmid == -1) {
        perror("shmget");
        return -1;
    }
    shmptr = shmat(shmid, 0, 0);
    if(shmptr == (void *)-1) {
        perror("shmat");
        return -1;
    }
    this->size = size;
    return 1;
}
void Shared_memory::freeShm() {
    if (shmptr)
        shmctl(shmid, IPC_RMID, nullptr);
    size = 0;
    shmid = 0;
    shmptr = nullptr;
}
void* Shared_memory::getShmptr(int id) {
    shmid_ds buf;
    if(shmctl(id, IPC_STAT, &buf) == -1) {
        perror("shmctl error");
        return nullptr;
    }
    shmptr = shmat(id, 0, 0);
    if(shmptr == (void*)-1) {
        perror("shmptr");
        return nullptr;
    }
    size = buf.shm_segsz;
    return shmptr;
}
size_t Shared_memory::getMemId() const {
    return shmid;
}
size_t Shared_memory::getSize() const {
    return size;
}
void* Shared_memory::getShmptr() {
    return shmptr;
}
