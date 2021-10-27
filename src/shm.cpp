#include "../include/header.h"

void ChildWriteSHM(Slave& process) {
    // проверить значение семафора на ноль
    // записать информацию
    // установить семафор в 2

    std::mt19937 gen; 
    gen.seed(time(0) + getpid()); // try to comment this string 
    std::string str = "write to shared memory " + std::to_string(getpid()) + std::string("  ") + std::to_string(gen() % 100);
    std::cout << "shm.cpp ChildWriteSHM (wait semaphore zero - procces -" << process.process_id << std::endl;
    Semaphore::CheckZero(process.semset_id, process.process_id);
    process.WriteToSHM(str.data() , SIZE_SHARED_MEMORY); 
    Semaphore::ReturnValue(process.semset_id, process.process_id, 2);
    sleep(1);
}

void *TreadReadFromSHM(void *arguments) {
   t_arg *arg;
   arg = (t_arg *)arguments;
   arg->master->ReadFromSHM(*(arg->slave));
   return NULL;
}

