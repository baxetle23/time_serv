#include "../include/master.h"
#include "../include/shared_memory.h"

Master::Master(size_t count, std::vector<ChildProc>& slaves) : 
    shared_memory_(count),
    slaves_(slaves),
    count_(count) {
}
Master::~Master() {}

int Master::InitMemory(size_t size_sigment) {
    for(Shared_memory& shm : shared_memory_) {
        if (shm.createShm(size_sigment) == -1) {
            return (1);
        }
        std::cout << "Id: " << shm.getMemId()
        << " Pointer: " << shm.getShmptr()
        << " Size: " << shm.getSize()
        << std::endl;
    }
    return 0;
}

void Master::ClearMemory() {
    for(Shared_memory& shm : shared_memory_) {
        shm.freeShm();
    }
    semctl(semset_id, 0, IPC_RMID, NULL); // delete array sem
}

int Master::InitProcesses(std::vector<ChildProc>& childs_) {
    for(size_t i = 0; i < count_; ++i) {

        //open pipe
        if (pipe(childs_[i].read_pipe) < 0 || pipe(childs_[i].write_pipe) < 0) {
            perror("pipe");
            exit(EXIT_FAILURE);
        }

        childs_[i].ptr_shm = shared_memory_[i].getShmptr();
        childs_[i].process_id = i;
        childs_[i].semset_id = semset_id;

        //fork
        if ((childs_[i].pid = fork()) < 0) {
            perror("fork");
            exit(EXIT_FAILURE);
        } else if (childs_[i].pid == 0) {
            //to do child
            //process 
            childs_[0].ptr_shm = shared_memory_[i].getShmptr();
            childs_[0].process_id = i;
            childs_[0].semset_id = semset_id;
            childs_[0].read_pipe[1] = childs_[i].read_pipe[1];
            childs_[0].write_pipe[0] = childs_[i].write_pipe[0];

            // close(0);
            // close(1);
            close(childs_[i].read_pipe[0]);
            close(childs_[i].write_pipe[1]);

            return (1);
        }
        //to do parent
        close(childs_[i].write_pipe[0]);
        close(childs_[i].read_pipe[1]);

    }
    return 0;
}

int Master::WaitAllProc() const {
    int status;

    while (1) {
        if (waitpid(-1, &status, 0) < 0) {
            break;
        }
    }
    return 0;
}

int Master::WriteToProcess(const char *str, int number_process, size_t size_) {
    return write(slaves_[number_process].write_pipe[1], str, size_);
}

int Master::ReadFromProcess(int number_process, char *buffer, size_t buffer_size) {
    return read(slaves_[number_process].read_pipe[0], buffer, buffer_size);
}

void Master::ReadFromSHM(ChildProc& process) {
    //уменьшить значение на единцу 
    //считать информацию
    //уменьшить значение на единцу получим 0  
    while(1) {
        struct sembuf minus = {(ushort)process.process_id,-1,0};
        if (semop(process.semset_id, &minus, 1) == -1) {
            fprintf(stdout, "FREE FAILED\n");
            exit (EXIT_FAILURE);
        } else {
            std::cout << static_cast<char *>(process.ptr_shm) << std::endl;
            semop(process.semset_id, &minus, 1);
        }
    }
}

int Master::InitSem(size_t count) {
    semset_id = semget(IPC_PRIVATE, count, 0600 | IPC_CREAT | IPC_EXCL);
    for (size_t i = 0; i < count; ++i) {
        union semun initval;
        initval.val = 0; // счетчик в 0
        if (semctl(semset_id, i, SETVAL, initval) == -1) {
            return 1;
        }
    }
    return 0;
}