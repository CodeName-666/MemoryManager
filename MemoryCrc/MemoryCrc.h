#ifndef IMEMORY_CRC_H
#define IMEMORY_CRC_H

#include "MemoryCrc_Types.h"

#if MEMORRY_CRC_ENABLED == true

inline static uint32_t MemoryCrc_calculate_crc(MemoryCrcType_e type, uint8_t* data, uint32_t data_len) 
{
        return Crc::calculate(static_cast<Crc::CrcType_e>(type), data,  data_len);
}
#endif
#endif