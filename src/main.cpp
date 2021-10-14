#include <../include/header.h>

typedef struct s_arg {
   Master    *master;
   ChildProc *slave;
   pthread_t *tread;
}  t_arg;


void SlaveProcess(std::vector<ChildProc>& childs) {
   std::cout << "I am child witd id " << childs[0].process_id
                << " Pid " << getpid()
                << " and has memory " << childs[0].ptr_shm 
                << " has fd " <<  childs[0].read_pipe[1] << ' ' <<  childs[0].write_pipe[0] << std::endl;

   //NONBLOCK READ WRITE TO PIPE
   // int i = 0;
   // while (++i < 1000) {
   //    ChildReadWritePipeNonblock(childs[0]);
   // }

   //WRITE TO SHM
   int i = 0;
   while (++i < 100) {
      ChildWriteSHM(childs[0]);
   }
}

void *ReadFromSHM(void *arguments) {
   t_arg *arg;
   arg = (t_arg *)arguments;
   arg->master->ReadFromSHM(*(arg->slave));
   return NULL;
}



int main() {
   std::vector<ChildProc> childs(CLIENTS_NUM);
   Master master(CLIENTS_NUM); 
   pthread_t threads[CLIENTS_NUM];
   t_arg arguments[CLIENTS_NUM];

   if (master.InitMemory(SIZE_MEMORY)) {
      fprintf(stdout, "%s", strerror(errno));
      exit(EXIT_FAILURE);
   }

   if (master.InitSem(SIZE_MEMORY)) {
      fprintf(stdout, "%s", strerror(errno));
      exit(EXIT_FAILURE);
   }

   if (master.InitProcesses(childs)) {
      //to do child
      SlaveProcess(childs);
      exit(EXIT_SUCCESS);
   }

   for (int i = 0; i < CLIENTS_NUM; ++i) {
      arguments[i].master = &master;
      arguments[i].slave = &childs[i];
      arguments[i].tread = &threads[i];
      pthread_create(threads + i, NULL, ReadFromSHM, &arguments[i]);
   }
   // to do parent   

   //NONBLOCK READ WRITE TO PIPE
   // int j = 0;
   // while (++j < 1000) {
   //    MasterWriteReadPipeNonblock(master, childs, j);
   // }

   //READ TO SHM
   // int i = 0;
   // while (++i < 100) {
   //    master.ReadFromSHM(childs);
   //    usleep(100);
   // }

   int test;
   std::cin >> test;
   std::cout << "WAIT SLAVE PROC\n";
   master.WaitAllProc();
   master.ClearMemory();
   exit (EXIT_SUCCESS);
}







































