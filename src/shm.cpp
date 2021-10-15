#include <../include/header.h>

void ChildWriteSHM(ChildProc& process) {
    // проверить значение семафора на ноль
    // записать информацию
    // установить семафор в 2

    (void)process;
    std::mt19937 gen; 
    gen.seed(time(0) + getpid()); // try to comment this string 
    std::string str = "write to shared memory " + std::to_string(getpid()) + std::string("  ") + std::to_string(gen() % 100);

    struct sembuf plus = {(ushort)process.process_id,2,0};
    struct sembuf zero = {(ushort)process.process_id,0,0};
    
    if (semop(process.semset_id, &zero, 1) == -1) {
        fprintf(stdout, "PROCESS SEM FAILED\n");
        exit (EXIT_FAILURE);
    } else {
        fprintf(stdout, "SRM IS READY TO Write\n");
        process.WriteToSHM(str.data() , SIZE_MEMORY); 
        semop(process.semset_id, &plus, 1);
        sleep(1);
    }
}

void *TreadReadFromSHM(void *arguments) {
   t_arg *arg;
   arg = (t_arg *)arguments;
   arg->master->ReadFromSHM(*(arg->slave));
   return NULL;
}

