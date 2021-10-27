#include "../include/header.h"

void WaitRequest(void *request_handler_slave) {
   RequestHandlerSlave * rh = static_cast<RequestHandlerSlave *>(request_handler_slave);
   rh->WaitRequest();
}

void *SlaveExecuteRequest(Slave *slave) {
  RequestHandlerSlave request_handler_slave_(slave->read_pipe[1], slave->write_pipe[0]);
  std::thread Read_pipe(WaitRequest, (void *)(&request_handler_slave_));
  while(1) {
      request_handler_slave_.ExecuteRequest();
      usleep(100);
  }
  return NULL;
}

//Slave
//-----------------------------------------------------------------------
//MASTER

void WaitResponces(void *request_handler) {
   RequestHandlerMaster* rh = static_cast<RequestHandlerMaster *>(request_handler);
   rh->WaitResponces();
}

void *Requests(void *arguments) {
   t_arg *arg;
   arg = (t_arg *)arguments;
   RequestHandlerMaster request_handler(arg->master->GetFdWrite(arg->slave->process_id),
                                 arg->master->GetFdRead(arg->slave->process_id));
   std::thread Read_pipe(WaitResponces, static_cast<void *>(&request_handler));
   std::vector<uint8_t> data {1};
   while (1)
   {  
      sleep(1);
      #ifdef TIME
         LOG_DURATION("send->responce id=" + std::to_string(arg->slave->process_id));
      #endif 
      uint8_t id;
      modbus_rtu_t responce;
      if (request_handler.SendRequest(MODBUS_FUNC_WRITE,64,data.size(),data, id)) {
         perror("pipe.cpp SendRequest");
      }
      if (request_handler.SendRequest(MODBUS_FUNC_READ,64,1,id)) {
         perror("pipe.cpp SendRequest");
      } 
      while (!responce.id) {
         usleep(100);
         request_handler.ResponceRequest(id, responce);
      }
   }
}
