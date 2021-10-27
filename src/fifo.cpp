#include "../include/fifo.h"

Fifo::Fifo(int fd_write, int fd_read) : cobs(fd_write, fd_read) {
}
bool Fifo::RequestAvailable() {
    return requests.size();
}
void Fifo::RemoveRequest() {
    requests.pop();
}
modbus_rtu_t& Fifo::NextRequest() {
    return requests.front().data_;
}
void Fifo::WaitRequest() {
    while(1) {
        Modbus&& modbus = cobs.GetDecodeModBusFrame();
        requests.push(std::move(modbus));
    }
}
uint8_t Fifo::SendResponce(Modbus&& modbus) {
    return cobs.SendEncodeModBusFrame(std::move(modbus));
}