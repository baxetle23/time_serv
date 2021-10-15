#include <../include/header.h>

void SlaveProcess(std::vector<ChildProc>& childs) {
   std::cout << "I am child witd id " << childs[0].process_id
                << " Pid " << getpid()
                << " and has memory " << childs[0].ptr_shm 
                << " has fd " <<  childs[0].read_pipe[1] << ' ' <<  childs[0].write_pipe[0] << std::endl;

   #ifdef FORK_PIPE
      pthread_t process_thread_PIPE;
      t_arg process_arguments_PIPE;
      process_arguments_PIPE.slave = &childs[0];
      process_arguments_PIPE.tread = &process_thread_PIPE;
      pthread_create(&process_thread_PIPE, NULL, ChildReadWritePipeNonblock, &process_arguments_PIPE);
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
   std::vector<ChildProc> childs(CLIENTS_NUM);
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
      pthread_t threads_SHM[CLIENTS_NUM];
      t_arg arguments_SHM[CLIENTS_NUM];
      for (int i = 0; i < CLIENTS_NUM; ++i) {
         arguments_SHM[i].master = &master;
         arguments_SHM[i].slave = &childs[i];
         arguments_SHM[i].tread = &threads_SHM[i];
         pthread_create(threads_SHM + i, NULL, TreadReadFromSHM, &arguments_SHM[i]);
      }
   #endif

   #ifdef FORK_PIPE
      pthread_t threads_PIPE[CLIENTS_NUM];
      t_arg arguments_PIPE[CLIENTS_NUM];
      for (int i = 0; i < CLIENTS_NUM; ++i) {
         arguments_PIPE[i].master = &master;
         arguments_PIPE[i].slave = &childs[i];
         arguments_PIPE[i].tread = &threads_PIPE[i];
         pthread_create(threads_PIPE + i, NULL, TreadWriteReadPipeNonblock, &arguments_PIPE[i]);
      }
   #endif


   int test;
   std::cout << "master wait\n";
   std::cin >> test;
   master.WaitAllProc();
   master.ClearMemory();
   exit (EXIT_SUCCESS);
}







































