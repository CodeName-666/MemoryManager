#ifndef EEPROM_STORAGE_H
#define EEPROM_SORAGE_H
#include "Memory.h"
#include <EEPROM.h>

struct EEPROM_Memory_Config_t
{
    size_t eeprom_size;
};

class EEPROM_Memory : public Memory
{

    public:
        EEPROM_Memory(EEPROM_Memory_Config_t &config);
        void init(void);
        uint16_t read(uint8_t* data_ptr, uint16_t start, uint16_t len);
        uint16_t write(uint8_t* data_ptr, uint16_t start, uint16_t len);
        bool commit(void); 

    private:
        EEPROM_Memory_Config_t *m_config;
    };

#endif