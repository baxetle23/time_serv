#include <queue>
#include "modbus.h"
#include "cobs.h"

class Modbus;

class Fifo {
private:
    std::queue<Modbus> requests;
    COBS cobs;
public:
    Fifo() = delete;
    Fifo(int fd_write, int fd_read);
    bool RequestAvailable();
    void RemoveRequest();
    modbus_rtu_t& NextRequest();
    void WaitRequest();
    uint8_t SendResponce(Modbus&& modbus);
};