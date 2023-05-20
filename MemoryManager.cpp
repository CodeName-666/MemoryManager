#include "MemoryManager.h"
#include "MemoryManager_Types.h"
#include "MemoryManager_Debug.h"


static MemoryManagerHeader_t header = {
    .version{0, 0, 0},
    .key = MemoryManagerStatus_e::STATUS_UNINIT,

#if MEMORRY_CRC_ENABLED == true
    .crc = 0x00000000,
#endif

};

static MemoryManagerHeader_t eeprom_header = {
    .version{0, 0, 0},
    .key = MemoryManagerStatus_e::STATUS_UNINIT,

#if MEMORRY_CRC_ENABLED == true
    .crc = 0x00000000,
#endif
};

static MemoryManagerHeader_t default_header = {
    .version{0, 0, 0},
    .key = MemoryManagerStatus_e::STATUS_UNINIT,

#if MEMORRY_CRC_ENABLED == true
    .crc = 0x00000000,
#endif
};

static MemoryManagerBlock header_block((uint8_t *)&header,
                                       (uint8_t *)&eeprom_header,
                                       sizeof(MemoryManagerHeader_t),
                                       (uint8_t *)&default_header);
                                      

MemoryManager::MemoryManager() : m_memory_interface(NULL)
{
    add_block(header_block);
    DBIF_INIT(9600);
}

MemoryManager::MemoryManager(Memory &interface, MemoryManagerConfig_t &config) : m_memory_interface(&interface)
{
    add_block(header_block);
    add_config(config);
}

void MemoryManager::add_config(MemoryManagerConfig_t &config)
{
    m_current_version = config.version;
    for (uint16_t i = 0; i < config.number_of_storage; i++)
    {
        add_block(config.storage[i]);
    }
}

void MemoryManager::add_interface(Memory &interface)
{
    m_memory_interface = &interface;
}

void MemoryManager::begin(void)
{
    MemoryManagerStatus_e status = MemoryManagerStatus_e::STATUS_ERROR;

    m_memory_interface->init();
    read_entry(MEMORY_HEADER_ENTRY);

    for(uint16_t i = 0; ((i < 2) && (status != MemoryManagerStatus_e::STATUTS_OK)); i++)
    {
        status = verify_memory();       
    }
    set_memory_verification(status);
}

void MemoryManager::loop()
{   
    bool written = false; 
    written = write_all(MemoryBlockWrite_e::WRITE_ON_CHANGE,MemoryManagerType_e::USER_DATA);
    if (written)
    {
        write_to_memory();
    }
}

uint16_t MemoryManager::add_block(MemoryManagerBlock &new_memory_block)
{
    m_memory_block_list.push_back(new_memory_block);
    m_memory_block_param_list.push_back(new_memory_block.bock_parameter());

    DBIF_LOG_DEBUG_0("New Block added - Blockadr = %i",new_memory_block);
    DBIF_LOG_DEBUG_0("Memory List Block Size =  %i",m_memory_block_list.size());

    return m_memory_block_list.size();
}

void MemoryManager::remove_block(MemoryManagerBlock &new_memory_block, bool delete_memory_data)
{
    int16_t pos = exists(new_memory_block);
    delete_entry(pos, delete_memory_data);
}

void MemoryManager::remove_block(uint16_t storage_idx, bool delete_memory_data)
{
    delete_entry(storage_idx, delete_memory_data);
}

void MemoryManager::shutdown(void)
{
    write_all(MemoryBlockWrite_e::WRITE_ALL_BLOCKS,MemoryManagerType_e::USER_DATA);  
}

MemoryManagerBlock *MemoryManager::get_block(uint16_t entry_idx)
{
    MemoryManagerBlock *ret = NULL;

    if (entry_idx < m_memory_block_list.size())
    {
        ret = &m_memory_block_list.at(entry_idx);
    }

    return ret;
}

void MemoryManager::delete_all(void)
{
    for(uint16_t i = 0; i < m_memory_block_list.size(); i++)
    {
        /* Overwirte data and delte all data entries */
        delete_entry(i,false);
    }

    /*Push everything into the memory....*/
    write_to_memory();
}


#if MEMORRY_CRC_ENABLED == true

uint32_t MemoryManager::calculate_crc(MemoryManagerType_e storageType)
{
    // uint8_t* user_data;
    // uint32_t length;
    // CRC32 crc;
    // crc.reset();
    // for(uint16_t i = 0; i < NUMBER_OF_EEPROM_DATA; i++)
    //{
    //     if(i != EEPROM_DATA_VALIDATION_IDX)
    //     {
    //         switch(storageType)
    //         {
    //             case INTERNAL_DATA:
    //                 user_data = storage[i].internal_data;
    //             break;
    //             case EEPROM_DATA:
    //                 user_data = storage[i].eeprom_data;
    //             break;
    //             case DEFAULT_DATA:
    //                 user_data = storage[i].internal_data;
    //             break;
    //             default:
    //                 user_data = nullptr;
    //             break;
    //         }
    //         length = storage[i].data_length;
    //         if(user_data != nullptr)
    //         {
    //             crc.update(user_data,length);
    //         }
    //     }
    // }
    //
    // return crc.finalize();
}

#endif