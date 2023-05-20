/**

    @file MemoryManager.h
    @brief This file contains the declaration of the MemoryManager class.
*/

#ifndef MEMORY_MANAGER_H
#define MEMORY_MANAGER_H

#include "MemoryManager_Types.h"
#include "Memory/Memory.h"
#include "MemoryList/MemoryList.h"

/**
 *
 *  @class MemoryManager
 *
 *  @brief This class provides an interface to store and manage data using a specified storage interface.
 */
class MemoryManager
{
public:
    /**
     * @brief Default constructor for the MemoryManager class.
     */
    MemoryManager();

    /**
     *  @brief Constructor for the MemoryManager class.
     *  @param interface A reference to an implementation of the IStorage interface.
     *  @param config A reference to a MemoryManagerConfig_t struct that specifies the configuration for the storage interface.
     */
    MemoryManager(Memory &interface, MemoryManagerConfig_t &config);

    /**
     *  @brief Adds a MemoryManagerConfig_t object to the storage configuration.
     *  @param config A reference to a MemoryManagerConfig_t struct that specifies the configuration to add.
     */
    void add_config(MemoryManagerConfig_t &config);

    /**
     *  @brief Adds an IStorage object to the storage interface.
     *  @param interface A reference to an implementation of the IStorage interface to add.
     */
    void add_interface(Memory &interface);

    /**
     *  @brief Initializes the storage interface.
     */
    void begin();

    /**
     * @brief Runs the main storage loop.
     */
    void loop();

    /**
     *  @brief Adds a new storage entry to the data list.
     *  @param new_memory_block A reference to the MemoryManagerBlock object to add_block.
     *  @return An 16-bit unsigned integer indicating the index of the new storage entry in the data list.
     */
    uint16_t add_block(MemoryManagerBlock &new_memory_block);

    /**
     *  @brief Removes a storage entry from the data list.
     *  @param new_memory_block A reference to the MemoryManagerBlock object to remove_block.
     */
    void remove_block(MemoryManagerBlock &new_memory_block, bool delete_memory_data = true);
    
    /**
     *  @brief Removes a storage entry from the data list.
     *  @param storage_idx An unsigned 16-bit integer specifying the index of the storage entry to remove_block.
     */
    void remove_block(uint16_t storage_idx, bool delete_memory_data = true);

    void delete_all(void);

    MemoryManagerBlock *get_block(uint16_t entry_idx);

    void shutdown(void);

protected:
    /**
     * @brief Reads all storage entries in the data list.
     */
    bool read_all(void);

    void read_entry(uint16_t entry_idx);

    void read_block_parameter(uint16_t entry_idx);

    bool write_all(MemoryBlockWrite_e write_flag = WRITE_ALL_BLOCKS, 
                   MemoryManagerType_e type = MemoryManagerType_e::USER_DATA);

    bool write_block_parameter(uint16_t entry_idx);

    bool write_entry(uint16_t entry_idx, MemoryManagerType_e data_type = MemoryManagerType_e::USER_DATA);

    bool write_to_memory(void);

    void delete_entry(uint16_t entry_idx, bool delete_memory_data = true);

    bool block_parameter_are_valid(uint16_t entry_idx);

    MemoryManagerStatus_e verify_memory(void);

    void set_memory_verification(MemoryManagerStatus_e status);

    void update_memory_version(MemoryManagerVersion_t new_version);
    /**
     * @brief Calculate the byte offset value of a storage index
     *
     * @param idx MemoryManager index until the offset will be calculated
     * @return uint16_t Offest in bytes
     */
    uint16_t get_offest(uint16_t idx);

    /**
     * @brief Copy data
     *
     * @param source Data which should be copied
     * @param destination Target where the data will be copied
     * @param size Size of the copied data
     */
    void copy_data(uint8_t *source, uint8_t *destination, int16_t size);

    MemoryDataCompare_e compare_data(uint8_t *source_1, uint8_t *source_2, int16_t size);

    MemoryDataCompare_e compare_entry(uint16_t entry_idx,MemoryManagerType_e data_type = MemoryManagerType_e::USER_DATA);

    int16_t exists(MemoryManagerBlock &memory_block);

#if MEMORRY_CRC_ENABLED == true
    /**
     * @brief Caclulates the CRC32 checksum of the storage
     *
     * @param storageType Type which CRC32 should be calculated
     * @return uint32_t CRC Checksum
     */
    uint32_t calculate_crc(MemoryManagerType_e storageType);
#endif

    template <typename T = uint8_t>
    T *get_data(MemoryManagerBlock *memory_block, MemoryManagerType_e type)
    
    {
        T *ret = NULL;
        if (memory_block != NULL)
        {
            ret = memory_block->get_data<T>(type);
        }
        return ret;
    }

private:
    MemoryManagerBlockList m_memory_block_list; /*< The linked list that stores the storage data.*/
    Memory *m_memory_interface;                 /**< A pointer to the storage interface.*/
    MemoryManagerVersion_t m_current_version;
    MemoryManagerBlockParamList m_memory_block_param_list;
};

#endif /* MEMORY_MANAGER_H */