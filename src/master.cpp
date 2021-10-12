#include "../include/master.h"
#include "../include/shared_memory.h"

Master::Master(size_t count) : 
    shared_memory_(count),
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
}

int Master::InitProcesses(std::vector<ChildProc>& childs_) {
    for(size_t i = 0; i < count_; ++i) {

        //open pipe
        if (pipe(childs_[i].read_pipe) < 0 || pipe(childs_[i].write_pipe) < 0) {
            perror("pipe");
            exit(EXIT_FAILURE);
        }
        //fork
        if ((childs_[i].pid = fork()) < 0) {
            perror("fork");
            exit(EXIT_FAILURE);
        } else if (childs_[i].pid == 0) {
            //to do child
            //process 
            childs_[0].ptr_shm = shared_memory_[i].getShmptr();
            childs_[0].number_proc = i + 1;
            childs_[0].read_pipe[1] = childs_[i].read_pipe[1];
            childs_[0].write_pipe[0] = childs_[i].write_pipe[0];

            // close(0);
            // close(1);
            close(childs_[i].read_pipe[0]);
            close(childs_[i].write_pipe[1]);



            // childs_[0].semset_id = semget(9900, 1, (0666|IPC_CREAT|IPC_EXCL));
            // if (childs_[0].semset_id = -1) {
            //     perror("fork");
            //     exit(EXIT_FAILURE);
            // }
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

// int Master::WriteToProcess(const char *str, int number_process) {
    
// }

// int Master::ReadFromProcess(int number_proc) {

// }

void Master::ReadFromSHM() {
    for(auto& sh : shared_memory_) {
        std::cout << static_cast<char *>(sh.getShmptr()) << std::endl; 
    }
}