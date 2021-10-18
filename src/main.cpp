#include <../include/header.h>

void SlaveProcess(std::vector<Slave>& childs) {
   std::cout << "I am child witd id " << childs[0].process_id
                << " Pid " << getpid()
                << " and has memory " << childs[0].ptr_shm 
                << " has fd " <<  childs[0].read_pipe[1] << ' ' <<  childs[0].write_pipe[0] << std::endl;

   #ifdef FORK_PIPE
      std::thread thread_pipe_read(ChildReadWritePipeNonblock, childs[0]);
   #endif

   #ifdef FORK_SHM_SEMAPHORE
      int i = 0;
      while (++i < 100) {
         ChildWriteSHM(childs[0]);
      }
   #endif
   int test;
   std::cout << "process wait\n";
   std::cin >> test;
}

int main() {
   std::vector<Slave> childs(CLIENTS_NUM);
   Master master(CLIENTS_NUM, childs); 


   #ifdef FORK_SHM_SEMAPHORE
      if (master.InitMemory(SIZE_MEMORY)) {
         fprintf(stdout, "%s", strerror(errno));
         exit(EXIT_FAILURE);
      }
      if (master.InitSem(SIZE_MEMORY)) {
         fprintf(stdout, "%s", strerror(errno));
         exit(EXIT_FAILURE);
      }
   #endif

   if (master.InitProcesses(childs)) {
      SlaveProcess(childs);
      exit(EXIT_SUCCESS);
   }

   //init treads
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
         threads_PIPE.push_back(std::thread(TreadWriteReadPipeNonblock, &arguments_PIPE[i]));
      }
   #endif


   int test;
   std::cout << "master wait\n";
   std::cin >> test;
   master.WaitAllProc();
   master.ClearMemory();
   exit (EXIT_SUCCESS);
}







































