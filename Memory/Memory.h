#ifndef MEMROY_MANAGER_IF_H
#define MEMROY_MANAGER_IF_H
#include "../MemoryManager_Types.h"

class Memory
{
public:
    Memory() {};
    virtual void init() = 0;
    virtual uint16_t read(uint8_t* data_ptr, uint16_t start, uint16_t len) = 0;
    virtual uint16_t write(uint8_t* data_ptr, uint16_t start, uint16_t len) = 0;
    virtual bool commit(void) {return true;} 

};

#endif