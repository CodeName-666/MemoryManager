#ifndef IMEMORY_CRC_TYPES_H
#define IMEMORY_CRC_TYPES_H

#include <stdio.h>
#include "Crc.h"

#include "../MemoryManager_Feature.h"



#if MEMORRY_CRC_ENABLED == true

enum MemoryCrcType_e {
    MEMORY_CRC_8   =  Crc::CRC_8,
    MEMORY_CRC_8H2F = Crc::CRC_8H2F,
    MEMORY_CRC_16  =  Crc::CRC_16, 
    MEMORY_CRC_32  =  Crc::CRC_32,
};


struct MemoryCrc_t {
    MemoryCrcType_e type;
    uint32_t crc;
};

#endif
#endif