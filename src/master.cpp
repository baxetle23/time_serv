#include "../include/master.h"
#include "../include/shared_memory.h"
#include "../include/semaphore.h"

Master::Master(size_t count, std::vector<Slave>& slaves) : 
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

int Master::ClearMemory() {
    for(Shared_memory& shm : shared_memory_) {
        shm.freeShm();
    }
    return 0;
}

void Master::SetSemId(size_t semid) {
    semset_id = semid;
}

int Master::InitProcesses(std::vector<Slave>& childs_) {
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

int  Master::GetFdWrite(int number_process) {
    return slaves_[number_process].write_pipe[1];
}
int Master::GetFdRead(int number_process) {
    return slaves_[number_process].read_pipe[0];
}

int Master::WriteToProcess(const char *str, int number_process, size_t size_) {
    return write(slaves_[number_process].write_pipe[1], str, size_);
}

int Master::ReadFromProcess(int number_process, char *buffer, size_t buffer_size) {
    return read(slaves_[number_process].read_pipe[0], buffer, buffer_size);
}

void Master::ReadFromSHM(Slave& process) { 
    while(1) {
        Semaphore::TakeValue(process.semset_id, process.process_id, 1);
        std::cout << static_cast<char *>(process.ptr_shm) << std::endl;
        Semaphore::TakeValue(process.semset_id, process.process_id, 1);
    }
}

