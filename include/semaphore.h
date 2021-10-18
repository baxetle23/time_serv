#pragma once
#include <unistd.h>
#include <sys/sem.h>

#define  SEM_KEY 9900

union semun {
    int val;
    struct semid_ds *buf;
    ushort *array;
};

class Semaphore {
private:
    const size_t sem_id_;
public:
    Semaphore(int count_sem,int semflg) : 
        sem_id_ (semget(SEM_KEY, count_sem, semflg)) {
    }
    ~Semaphore() {
        semctl(sem_id_, 0, IPC_RMID, NULL);
    }
    int SetSemValue(int nsem, int value) {
        union semun initval;
        initval.val = value;
        if (semctl(sem_id_, nsem, SETVAL, initval) == - 1) {
            return 1;
        }
        return 0;
    }
    size_t GetId() const {
        return sem_id_;
    }
};