#include "client.h"

void ChildProc::WriteToSHM(const char *string, size_t size) {
    strncpy((char *)ptr_shm, string, size);
}
// int ChildProc::WriteToMaster(const char *string, size_t size) {
//     return write(fd_write[1], string, size);
// }
// int ChildProc::ReadFromMaster(char *buffer, size_t size) {
//     return read(fd_read[0], buffer, size);
// }