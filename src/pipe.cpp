#include <../include/header.h>

//класс обработчик событий 
//класс отправки сообщений
void ReadPipeFromMaster(Slave *slave, char *buffer, size_t buffer_size, int *count) {
   while (1) {
      slave->ReadFromMaster(buffer, buffer_size);
      *count = 1;
   }
}

//проверка Read Write - спрятать в класс 
void *ChildReadWritePipeNonblock(Slave *slave) {
   char buffer[SIZE_MESSAGE];
   int count = 0;
   std::thread Read_pipe(ReadPipeFromMaster, slave, buffer, SIZE_MESSAGE, &count);
   while(1) {
      if (count == 1) {
         slave->WriteToMaster(buffer, SIZE_MESSAGE);
         count = 0;
      } else {
         usleep(100);
      }
   }
   return NULL;
}

void ReadPipeFromSlave(void *arguments) {
   t_arg *arg;
   arg = (t_arg *)arguments;
   while (1) {
      char test_pipe[SIZE_MESSAGE];
      arg->master->ReadFromProcess(arg->slave->process_id, test_pipe, SIZE_MESSAGE);
      std::cout << "принял " << test_pipe;
   }
}

void *TreadWriteReadPipeNonblock(void *arguments) {
   t_arg *arg;
   arg = (t_arg *)arguments;
   std::thread Read_pipe(ReadPipeFromSlave, arguments);
   int j = 0;
   while (1) {
      j++;
      arg->master->WriteToProcess(("pipe.cpp TreadWriteReadPipeNonblock (pid slave = " + std::to_string(arg->slave->pid) + " iterator = " + std::to_string(j) + ")\n").data(), arg->slave->process_id, SIZE_MESSAGE);
      std::cout << ("отправил pipe.cpp TreadWriteReadPipeNonblock (pid slave = " + std::to_string(arg->slave->pid) + " iterator = " + std::to_string(j) + ")\n").data();
      sleep(1);
   }
   return NULL;
}