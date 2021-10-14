#include "client.h"

void ChildProc::WriteToSHM(const char *string, size_t size) {
    strncpy((char *)ptr_shm, string, size);
}
int ChildProc::WriteToMaster(const char *string, size_t size) {
    return write(read_pipe[1], string, size);
}
int ChildProc::ReadFromMaster(char *buffer, size_t size) {
    return read(write_pipe[0], buffer, size);
}