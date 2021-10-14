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


//сделать обертку на системными вызовывами
void MasterWriteReadPipeNonblock(Master& , std::vector<ChildProc>& childs, int iterator) {
    for (int i = 0; i < CLIENTS_NUM; ++i) {
        write(childs[i].write_pipe[1], ("TEST PIPE" + std::to_string(iterator)).data(), SIZE_MESSAGE);
    }
    for (int i = 0; i < CLIENTS_NUM; ++i) {
        char test_pipe[SIZE_MESSAGE];
        read(childs[i].read_pipe[0], test_pipe, SIZE_MESSAGE);
        std::cout << "READ FROM PIPE: " << test_pipe << std::endl; 
    }
}