/**
 * @file MemoryManager.h
 * @brief This file contains the declaration of the MemoryManager class.
 */

#ifndef MEMORY_MANAGER_H
#define MEMORY_MANAGER_H

#include "MemoryManager_Types.h"
#include "Memory/Memory.h"
#include "MemoryList/MemoryList.h"

/**
 * @class MemoryManager
 * @brief This class provides an interface to store and manage data using a specified storage interface.
 */
class MemoryManager
{
public:
    /**
     * @brief Default constructor for the MemoryManager class.
     */
    MemoryManager();

    /**
     * @brief Constructor for the MemoryManager class.
     * @param interface A reference to an implementation of the IStorage interface.
     * @param config A reference to a MemoryManagerConfig_t struct that specifies the configuration for the storage interface.
     */
    MemoryManager(Memory &interface, MemoryManagerConfig_t &config);

    /**
     * @brief Adds a MemoryManagerConfig_t object to the storage configuration.
     * @param config A reference to a MemoryManagerConfig_t struct that specifies the configuration to add.
     */
    void add_config(MemoryManagerConfig_t &config);

    /**
     * @brief Adds an IStorage object to the storage interface.
     * @param interface A reference to an implementation of the IStorage interface to add.
     */
    void add_interface(Memory &interface);

    /**
     * @brief Initializes the storage interface.
     */
    void begin();

    /**
     * @brief Runs the main storage loop.
     */
    void loop();

    /**
     * @brief Adds a new storage entry to the data list.
     * @param new_memory_block A reference to the MemoryManagerBlock object to add_block.
     * @return An unsigned 16-bit integer indicating the index of the new storage entry in the data list.
     */
    uint16_t add_block(MemoryManagerBlock &new_memory_block);

    /**
     * @brief Removes a storage entry from the data list.
     * @param new_memory_block A reference to the MemoryManagerBlock object to remove_block.
     */
    void remove_block(MemoryManagerBlock &new_memory_block, bool delete_memory_data = true);

    /**
     * @brief Removes a storage entry from the data list.
     * @param storage_idx An unsigned 16-bit integer specifying the index of the storage entry to remove_block.
     */
    void remove_block(uint16_t storage_idx, bool delete_memory_data = true);

    /**
     * @brief Deletes all storage entries from the data list.
     */
    void delete_all(void);

    /**
     * @brief Retrieves a MemoryManagerBlock pointer based on the given entry index.
     * @param entry_idx An unsigned 16-bit integer specifying the index of the storage entry.
     * @return A pointer to the MemoryManagerBlock object.
     */
    MemoryManagerBlock *get_block(uint16_t entry_idx);

    /**
     * @brief Shuts down the MemoryManager and releases any resources.
     */
    void shutdown(void);

protected:
    /**
     * @brief Reads all storage entries in the data list.
     * @return A boolean value indicating the success of the operation.
     */
    bool read_all(void);

    /**
     * @brief Reads a specific storage entry from the data list.
     * @param entry_idx An unsigned 16-bit integer specifying the index of the storage entry.
     */
    void read_entry(uint16_t entry_idx);

    /**
     * @brief Reads the parameters of a specific storage entry from the data list.
     * @param entry_idx An unsigned 16-bit integer specifying the index of the storage entry.
     */
    void read_block_parameter(uint16_t entry_idx);

    /**
     * @brief Writes all storage entries to the storage interface.
     * @param write_flag A MemoryBlockWrite_e value specifying the write flag.
     * @param type A MemoryManagerType_e value specifying the data type.
     * @return A boolean value indicating the success of the operation.
     */
    bool write_all(MemoryBlockWrite_e write_flag = WRITE_ALL_BLOCKS, MemoryManagerType_e type = MemoryManagerType_e::USER_DATA);

    /**
     * @brief Writes the parameters of a specific storage entry to the storage interface.
     * @param entry_idx An unsigned 16-bit integer specifying the index of the storage entry.
     * @return A boolean value indicating the success of the operation.
     */
    bool write_block_parameter(uint16_t entry_idx);

    /**
     * @brief Writes a specific storage entry to the storage interface.
     * @param entry_idx An unsigned 16-bit integer specifying the index of the storage entry.
     * @param data_type A MemoryManagerType_e value specifying the data type.
     * @return A boolean value indicating the success of the operation.
     */
    bool write_entry(uint16_t entry_idx, MemoryManagerType_e data_type = MemoryManagerType_e::USER_DATA);

    /**
     * @brief Writes data to the storage interface.
     * @return A boolean value indicating the success of the operation.
     */
    bool write_to_memory(void);

    /**
     * @brief Deletes a specific storage entry from the data list.
     * @param entry_idx An unsigned 16-bit integer specifying the index of the storage entry.
     * @param delete_memory_data A boolean value indicating whether to delete the memory data associated with the entry.
     */
    void delete_entry(uint16_t entry_idx, bool delete_memory_data = true);

    /**
     * @brief Checks if the block parameters of a specific storage entry are valid.
     * @param entry_idx An unsigned 16-bit integer specifying the index of the storage entry.
     * @return A boolean value indicating whether the block parameters are valid.
     */
    bool block_parameter_are_valid(uint16_t entry_idx);

    /**
     * @brief Verifies the integrity of the storage.
     * @return A MemoryManagerStatus_e value indicating the status of the memory verification.
     */
    MemoryManagerStatus_e verify_memory(void);

    /**
     * @brief Sets the memory verification status.
     * @param status A MemoryManagerStatus_e value specifying the memory verification status.
     */
    void set_memory_verification(MemoryManagerStatus_e status);

    /**
     * @brief Updates the memory version.
     * @param new_version A MemoryManagerVersion_t value specifying the new memory version.
     */
    void update_memory_version(MemoryManagerVersion_t new_version);

    /**
     * @brief Calculates the byte offset value of a storage index.
     * @param idx An unsigned 16-bit integer specifying the MemoryManager index.
     * @return An unsigned 16-bit integer indicating the offset in bytes.
     */
    uint16_t get_offest(uint16_t idx);

    /**
     * @brief Copies data from source to destination.
     * @param source The source data to be copied.
     * @param destination The target where the data will be copied.
     * @param size The size of the data to be copied.
     */
    void copy_data(uint8_t *source, uint8_t *destination, int16_t size);

    /**
     * @brief Compares two blocks of data.
     * @param source_1 The first block of data to compare.
     * @param source_2 The second block of data to compare.
     * @param size The size of the data blocks to compare.
     * @return A MemoryDataCompare_e value indicating the result of the data comparison.
     */
    MemoryDataCompare_e compare_data(uint8_t *source_1, uint8_t *source_2, int16_t size);

    /**
     * @brief Compares a specific storage entry with data of a given type.
     * @param entry_idx An unsigned 16-bit integer specifying the index of the storage entry.
     * @param data_type A MemoryManagerType_e value specifying the data type.
     * @return A MemoryDataCompare_e value indicating the result of the data comparison.
     */
    MemoryDataCompare_e compare_entry(uint16_t entry_idx, MemoryManagerType_e data_type = MemoryManagerType_e::USER_DATA);

    /**
     * @brief Checks if a memory block exists in the data list.
     * @param memory_block A reference to the MemoryManagerBlock object to check.
     * @return An integer value indicating the index of the memory block (-1 if not found).
     */
    int16_t exists(MemoryManagerBlock &memory_block);

#if MEMORRY_CRC_ENABLED == true
    /**
     * @brief Calculates the CRC32 checksum of the storage.
     * @param storageType The type of storage for which the CRC32 should be calculated.
     * @return An unsigned 32-bit integer representing the CRC checksum.
     */
    uint32_t calculate_crc(MemoryManagerType_e storageType);
#endif

    /**
     * @brief Retrieves the data pointer of a specific memory block based on the given data type.
     * @tparam T The data type of the memory block.
     * @param memory_block A pointer to the MemoryManagerBlock object.
     * @param type A MemoryManagerType_e value specifying the data type.
     * @return A pointer to the data of the specified type.
     */
    template <typename T = uint8_t>
    T *get_data(MemoryManagerBlock *memory_block, MemoryManagerType_e type)
    {
        T *ret = nullptr;
        if (memory_block != nullptr)
        {
            ret = memory_block->get_data<T>(type);
        }
        return ret;
    }

private:
    MemoryManagerBlockList m_memory_block_list; /**< The linked list that stores the storage data. */
    Memory *m_memory_interface;                 /**< A pointer to the storage interface. */
    MemoryManagerVersion_t m_current_version;
    MemoryManagerBlockParamList m_memory_block_param_list;
};

#endif /* MEMORY_MANAGER_H */
