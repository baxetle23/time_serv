#include <../include/header.h>

void *ChildReadWritePipeNonblock(void* arguments) {
   t_arg *arg;
   arg = (t_arg *)arguments;
   const int flags = fcntl(arg->slave->write_pipe[0], F_GETFL, 0);
   fcntl(arg->slave->write_pipe[0], F_SETFL, flags | O_NONBLOCK);
   while(1) {
      char buffer[SIZE_MESSAGE];
      if (arg->slave->ReadFromMaster(buffer, SIZE_MESSAGE) > 0) {
         // if (errno != EWOULDBLOCK) {
         //    perror("read");
         //    exit(EXIT_FAILURE);
         // }
         arg->slave->WriteToMaster(buffer, SIZE_MESSAGE);
      } else {
            std::cout << "pipe.cpp ChildReadWritePipeNonblock ("<< getpid() << " wait)" << std::endl;
            sleep(1);
            continue ;
      }
   }
   return NULL;
}

void *TreadWriteReadPipeNonblock(void *arguments) {
   t_arg *arg;
   arg = (t_arg *)arguments;

   int j = 0;
   while (1) {
      char test_pipe[SIZE_MESSAGE];
      j++;
      arg->master->WriteToProcess(("pipe.cpp TreadWriteReadPipeNonblock (pid slave = " + std::to_string(arg->slave->pid) + " iterator = " + std::to_string(j) + ")").data(), arg->slave->process_id, SIZE_MESSAGE);
      std::cout << "отправил "<< ("pipe.cpp TreadWriteReadPipeNonblock (pid slave = " + std::to_string(arg->slave->pid) + " iterator = " + std::to_string(j) + ")").data() << std::endl;
      arg->master->ReadFromProcess( arg->slave->process_id, test_pipe, SIZE_MESSAGE);
      std::cout << "принял " << test_pipe << std::endl;
      sleep(3);
   }
   return NULL;
}