#include <../include/header.h>

void ChildReadWritePipeNonblock(ChildProc& process) {
   const int flags = fcntl(process.write_pipe[0], F_GETFL, 0);
   fcntl(process.write_pipe[0], F_SETFL, flags | O_NONBLOCK);
   while(1) {
      char buffer[SIZE_MESSAGE];
      if (process.ReadFromMaster(buffer, SIZE_MESSAGE) < 0) {
         if (errno != EWOULDBLOCK) {
            perror("read");
            exit(EXIT_FAILURE);
         }
      } else {
         process.WriteToMaster(buffer, SIZE_MESSAGE);
         break ;
      }
      std::cout << getpid() << " wait" << std::endl;
      usleep(50);
   }
}

void *TreadWriteReadPipeNonblock(void *arguments) {
   t_arg *arg;
   arg = (t_arg *)arguments;

   int j = 0;
   while (++j < 5) {
      char test_pipe[SIZE_MESSAGE];
      arg->master->WriteToProcess(("TEST PIPE" + std::to_string(j)).data(), arg->slave->process_id, SIZE_MESSAGE);
      arg->master->ReadFromProcess( arg->slave->process_id, test_pipe, SIZE_MESSAGE);
      std::cout << test_pipe << std::endl;
   }
   return NULL;
}