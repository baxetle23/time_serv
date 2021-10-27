#include "../include/client.h"

int Slave::WriteToSHM(const char *string, size_t size) {
    strncpy((char *)ptr_shm, string, size);
    return 0;
}
int Slave::WriteToMaster(const char *string, size_t size) {
    size_t n_write;
    n_write = write(read_pipe[1], string, size);
    if (n_write == size) {
        return 0;
    } else {
        return 1;
    }
}
// analys read
int Slave::ReadFromMaster(char *buffer, size_t size) {
    size_t n_read;
    n_read = read(write_pipe[0], buffer, size);
    if (n_read == size) {
        return 0;
    } else {
        return 1;
    }
}