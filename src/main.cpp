#include <sys/shm.h>
#include <sys/ipc.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/sem.h>
#include <fcntl.h>
#include <iostream>
#include <cstdio>
#include <cstring>
#include <string>
#include <vector>
#include <unistd.h>
#include <random>
#include <ctime>
#include "../include/shared_memory.h"
#include "../include/master.h"
#include "../include/client.h"

#define SIZE_MEMORY 100
#define CLIENTS_NUM 10

int main() {
   std::vector<ChildProc> childs(CLIENTS_NUM);
   Master master(CLIENTS_NUM);

   
   if (master.InitMemory(SIZE_MEMORY)) {
      fprintf(stdout, "%s", strerror(errno));
      exit(EXIT_FAILURE);
   }

   if (master.InitProcesses(childs)) {
      //to do child
      //example
      //tread create
      std::cout << "I am child witd id " << childs[0].number_proc
                << " Pid " << getpid()
                << " and has memory " << childs[0].ptr_shm 
                << " has fd " <<  childs[0].read_pipe[1] << ' ' <<  childs[0].write_pipe[0] << std::endl;

      //NONBLOCK READ WRITE
      const int flags = fcntl(childs[0].write_pipe[0], F_GETFL, 0);
      fcntl(childs[0].write_pipe[0], F_SETFL, flags | O_NONBLOCK);
      while(1) {
         char buffer[10];
         if (read(childs[0].write_pipe[0], buffer, 10) < 0) {
            if (errno != EWOULDBLOCK) {
               perror("read");
               exit(EXIT_FAILURE);
            }
         } else {
            write(childs[0].read_pipe[1], buffer, 10);
         }
         usleep(50);
      }

      // while (1) {
      //    std::mt19937 gen; 
      //    gen.seed(time(0) + getpid()); // try to comment this string 
      //    std::string str = "write to shared memory " + std::to_string(getpid()) + std::string("  ") + std::to_string(gen() % 100);
      //    childs[0].WriteToSHM(str.data() , SIZE_MEMORY);
      //    sleep(3);
      // }


      exit(EXIT_SUCCESS);
   }
   // to do parent

   for (int i = 0; i < CLIENTS_NUM; ++i) {
      write(childs[i].write_pipe[1], "TEST PIPE", 9);
   }

   // sleep(3);

   for (int i = 0; i < CLIENTS_NUM; ++i) {
      char test_pipe[10];
      read(childs[i].read_pipe[0], test_pipe, 10);
      std::cout << "READ FROM PIPE: " << test_pipe << std::endl; 
   }

   int test;
   std::cin >> test;

// while(1) {
//    master.ReadFromSHM();
//    usleep(100);
// }
   master.WaitAllProc();
   master.ClearMemory();
}







































