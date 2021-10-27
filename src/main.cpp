#include "../include/header.h"

void SlaveProcess(std::vector<Slave>& childs) {
   #ifdef DEBUG
   std::string name_file_debug("debug_" + std::to_string(getpid()) + "_file.txt"); 
   int fd_debug = open(name_file_debug.data(), O_RDWR | O_CREAT | O_TRUNC, 0666);
   if (fd_debug == -1) {
      perror("open");
   }
   dup2(fd_debug, 1);
   std::cout << "I am child witd id " << childs[0].process_id
                << " Pid " << getpid()
                << " and has memory " << childs[0].ptr_shm 
                << " has fd " <<  childs[0].read_pipe[1] << ' ' <<  childs[0].write_pipe[0] << std::endl;
   #endif
   
   #ifdef FORK_PIPE
      SlaveExecuteRequest(&childs[0]);
   #endif

   #ifdef FORK_SHM_SEMAPHORE
      int i = 0;
      while (++i < 100) {
         ChildWriteSHM(childs[0]);
      }
   #endif

   int test;
   std::cin >> test;
}

int main() {
   int fd_err = open("error_file.txt", O_RDWR | O_CREAT | O_TRUNC, 0666);
   if (fd_err == -1) {
      perror("open");
      exit (1);
   }
   dup2(fd_err, 2);
   std::cerr << "begin error information\n";
   std::vector<Slave> childs(CLIENTS_NUM);
   Semaphore semaphores(CLIENTS_NUM, 0600 | IPC_CREAT | IPC_EXCL, 0);
   Master master(CLIENTS_NUM, childs); 


   #ifdef FORK_SHM_SEMAPHORE
      if (master.InitMemory(SIZE_SHARED_MEMORY)) {
         fprintf(stdout, "%s", strerror(errno));
         exit(EXIT_FAILURE);
      }
      master.SetSemId(semaphores.GetId());

   #endif

   if (master.InitProcesses(childs)) {
      SlaveProcess(childs);
      exit(EXIT_SUCCESS);
   }

   #ifdef FORK_SHM_SEMAPHORE
      std::vector<std::thread> threads_SHM;
      t_arg arguments_SHM[CLIENTS_NUM];
      for (int i = 0; i < CLIENTS_NUM; ++i) {
         arguments_SHM[i].master = &master;
         arguments_SHM[i].slave = &childs[i];
         threads_SHM.push_back(std::thread(TreadReadFromSHM, &arguments_SHM[i]));
      }
   #endif

   #ifdef FORK_PIPE
      std::vector<std::thread> threads_PIPE;
      t_arg arguments_PIPE[CLIENTS_NUM];
      for (int i = 0; i < CLIENTS_NUM; ++i) {
         arguments_PIPE[i].master = &master;
         arguments_PIPE[i].slave = &childs[i];
         threads_PIPE.push_back(std::thread(Requests, &arguments_PIPE[i]));
         threads_PIPE[i].detach();
      }
   #endif

   master.WaitAllProc();
   master.ClearMemory();
   exit (EXIT_SUCCESS);
}







































