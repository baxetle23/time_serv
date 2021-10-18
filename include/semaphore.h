#pragma once
#include <unistd.h>
#include <sys/sem.h>

union semun {
    int val;
    struct semid_ds *buf;
    ushort *array;
};

class Semaphore {
private:
    const size_t sem_id_;
public:
    Semaphore(int count_sem, int semflg, int value) : 
        sem_id_ (semget(IPC_PRIVATE, count_sem, semflg)) { //semflg_default =  0600 | IPC_CREAT | IPC_EXCL 
        union semun initval;
        initval.val = value;
        for (int i = 0; i < count_sem; ++i) {
            if (semctl(sem_id_, i, SETVAL, initval) == - 1) {
                fprintf(stderr, "PROCESS SEM FAILED\n");
                exit (EXIT_FAILURE);
            }
        }
    }
    ~Semaphore() {
        semctl(sem_id_, 0, IPC_RMID, NULL);
    }
    static int ReturnValue(size_t semset_id, size_t id_sem, short int value) {
        struct sembuf plus = {(ushort)id_sem,value, 0};
        if (semop(semset_id, &plus, 1) == -1) {
            fprintf(stderr, "PROCESS SEM FAILED\n");
            return 1;
        } else {
            return 0;
        }
    }
    static int TakeValue(size_t semset_id, size_t id_sem, short int value) {
        short int minus_value = value * (-1);
        struct sembuf minus = {(ushort)id_sem, minus_value, 0};
        if (semop( semset_id, &minus, 1) == -1) {
            fprintf(stderr, "PROCESS SEM FAILED\n");
            return 1;
        } else {
            return 0;
        }
    }
    static int CheckZero(size_t semset_id, size_t id_sem) {
        struct sembuf zero = {(ushort)id_sem,0,0};
        if (semop(semset_id, &zero, 1) == -1) {
            fprintf(stderr, "PROCESS SEM FAILED\n");
            return 1;
        } else {
            return 0;
        }
    }
    size_t GetId() const {
        return sem_id_;
    }
};