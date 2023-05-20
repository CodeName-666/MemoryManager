#include "EEPROM_Memory.h"
#include "MemoryManager_Debug.h"

EEPROM_Memory::EEPROM_Memory(EEPROM_Memory_Config_t &config) : Memory(), m_config(&config)
{
}

void EEPROM_Memory::init(void)
{

    EEPROM.begin(m_config->eeprom_size);
}


uint16_t EEPROM_Memory::read(uint8_t* data_ptr, uint16_t start, uint16_t len) 
{
    return EEPROM.readBytes(start,data_ptr,len);
}


uint16_t EEPROM_Memory::write(uint8_t* data_ptr, uint16_t start, uint16_t len)
{
    
    return EEPROM.writeBytes(start,data_ptr,len);
}


bool EEPROM_Memory::commit(void)
{
    DBIF_LOG_DEBUG_0("COMMIT TO EEPORM");
    return EEPROM.commit();
}