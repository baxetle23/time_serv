#pragma once
#include <unistd.h>

class Semaphore {
private:
    int semset_id;
public:
    Semaphore();
    int GetAssert(size_t count);
    int TakeAsset(size_t count);
};