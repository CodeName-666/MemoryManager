/**
 * @file memory_manager_types.h
 * @brief This file defines types and structures used by the MemoryManager module.
 */

#ifndef MEMORY_MANAGER_TYPES_H
#define MEMORY_MANAGER_TYPES_H

#include "stdio.h"
#include "string.h"
#include "MemoryCrc/MemoryCrc_Types.h"

/**
 * @def MEMORY_HEADER_ENTRY
 * @brief Identifier for the memory header entry.
 */
#define MEMORY_HEADER_ENTRY 0u

/**
 * @enum MemoryDataCompare_e
 * @brief Enumerates the possible results of a memory data comparison.
 */
enum MemoryDataCompare_e
{
    MEMORY_DATA_NOT_EQUAL = 0x00, /**< Memory data is not equal. */
    MEMORY_DATA_EQUAL = 0x01      /**< Memory data is equal. */
};   


/**
 * @enum MemoryManagerStatus_e
 * @brief Enumerates the possible statuses of a MemoryManager block.
 */
enum MemoryManagerStatus_e
{
    STATUTS_OK = 0x33,     /**< MemoryManager block status: OK. */
    STATUS_NOT_OK = 0x22,  /**< MemoryManager block status: Not OK. */
    STATUS_UNINIT = 0x11,  /**< MemoryManager block status: Uninitialized. */
    STATUS_ERROR = 0x00    /**< MemoryManager block status: Error. */
};

/**
 * @enum MemoryManagerType_e
 * @brief Enumerates the types of data managed by a MemoryManager block.
 */
enum MemoryManagerType_e
{
    USER_DATA = 0x00,    /**< User data. */
    MEMORY_DATA,        /**< EEPROM data. */
    DEFAULT_DATA        /**< Default data. */
};

/**
 * @enum MemoryBlockWrite_e
 * @brief Enumerates the possible write modes for a MemoryManager block.
 */
enum MemoryBlockWrite_e
{
    WRITE_NOT_CONFIGURED = 0x00, /**< Write mode: Not configured. */
    WRITE_CYCLIC,               /**< Write mode: Cyclic (not implemented yet). */
    WRITE_ON_CHANGE,            /**< Write mode: On change. */
    WRITE_ON_SHUTDOWN,          /**< Write mode: On shutdown (not implemented yet). */
    WRITE_ALL_BLOCKS            /**< Write mode: All blocks. */
};

/**
 * @struct MemoryBlockParams_t
 * @brief Holds parameters for a MemoryManager block.
 */
struct MemoryBlockParams_t
{
    MemoryManagerStatus_e status; /**< Status of the MemoryManager block. */
    MemoryBlockWrite_e write_flag; /**< Write mode of the MemoryManager block. */

#if MEMORRY_CRC_ENABLED == true
    MemoryCrcType_e crc_type; /**< Type of CRC used (if enabled). */
#endif
    uint32_t cycle_time; /**< Cycle time for the block (if applicable). */
};

/**
 * @class MemoryManagerBlock
 * @brief Class representing a block of memory managed by a MemoryManager.
 *
 * This class holds pointers to user data, EEPROM data, default data, as well as
 * the length of the data, and a flag indicating whether the data has been initialized.
 * It also provides getter and setter methods for accessing and modifying the private member variables.
 */
class MemoryManagerBlock
{
public:
    /**
     * @brief Default constructor for MemoryManagerBlock.
     *
     * Initializes all member variables to default values.
     */
    MemoryManagerBlock() : m_user_data(nullptr), 
                           m_eeprom_data(nullptr),
                           m_default_data(nullptr), 
                           m_data_length(0),
                           m_block_params{MemoryManagerStatus_e::STATUS_UNINIT, MemoryBlockWrite_e::WRITE_NOT_CONFIGURED},
                           m_block_params_update(false)
    {
    }

    /**
     * @brief Constructor for MemoryManagerBlock.
     *
     * Initializes the member variables with the provided values.
     *
     * @param user_data Pointer to user data.
     * @param eeprom_data Pointer to EEPROM data.
     * @param data_length Length of data.
     * @param default_data Pointer to default data.
     */
    MemoryManagerBlock(uint8_t *user_data, 
                       uint8_t *eeprom_data, 
                       uint32_t data_length,
                       uint8_t *default_data = nullptr, 
                       MemoryBlockWrite_e write_flag = MemoryBlockWrite_e::WRITE_NOT_CONFIGURED) : 
                                               m_user_data(user_data),
                                               m_eeprom_data(eeprom_data),
                                               m_default_data(default_data),
                                               m_data_length(data_length),
                                               m_block_params{MemoryManagerStatus_e::STATUS_UNINIT, write_flag},
                                               m_block_params_update(false)
    {
    }

    /**
     * @brief Templated constructor for MemoryManagerBlock.
     *
     * Initializes the member variables with the provided templated values.
     *
     * @tparam T Type of data.
     * @param user_data Pointer to user data.
     * @param eeprom_data Pointer to EEPROM data.
     * @param data_length Length of data.
     * @param default_data Pointer to default data.
     */

    /*template<class T = uint8_t> explicit MemoryManagerBlock(T* user_data, T* eeprom_data, uint32_t data_length,
                       T* default_data = nullptr) : m_user_data(reinterpret_cast<uint8_t*>(user_data)),
                                                 m_eeprom_data(reinterpret_cast<uint8_t*>(eeprom_data)),
                                                 m_default_data(reinterpret_cast<uint8_t*>(default_data)),
                                                 m_data_length(data_length), m_init(false)
    {

    /**
     * @brief Getter for user data.
     *
     * @tparam T Type of user data.
     * @return Pointer to user data.
     */
    template <typename T = uint8_t>
    T *user_data() const { return reinterpret_cast<T *>(m_user_data); }

    /**
     * @brief Setter for user data.
     *
     * @tparam T Type of user data.
     * @param user_data Pointer to user data.
     */
    template <typename T = uint8_t>
    void user_data(T *user_data) { m_user_data = reinterpret_cast<uint8_t *>(user_data); }

    /**
     * @brief Getter for EEPROM data.
     *
     * @tparam T Type of EEPROM data.
     * @return Pointer to EEPROM data.
     */
    template <typename T = uint8_t>
    T *eeprom_data() const { return reinterpret_cast<T *>(m_eeprom_data); }

    /**
     * @brief Setter for EEPROM data.
     *
     * @tparam T Type of EEPROM data.
     * @param eeprom_data Pointer to EEPROM data.
     */
    template <typename T = uint8_t>
    void eeprom_data(T *eeprom_data) { m_eeprom_data = eeprom_data; }

    /**
     * @brief Getter for default data.
     *
     * @tparam T Type of default data.
     * @return Pointer to default data.
     */
    template <typename T = uint8_t>
    T *default_data() const { return reinterpret_cast<T *>(m_default_data); }

    /**
     * @brief Setter for default data.
     *
     * @tparam T Type of default data.
     * @param default_data Pointer to default data.
     */
    template <typename T = uint8_t>
    void default_data(T *default_data) { m_default_data = reinterpret_cast<T *>(default_data); }

    /**
     * @brief Getter for data length.
     *
     * @return Length of data.
     */
    uint32_t data_length() const { return m_data_length; }

    /**
     * @brief Setter for data length.
     *
     * @param data_length Length of data.
     */
    void data_length(uint32_t data_length) { m_data_length = data_length; }

    /**
     * @brief Getter for the status of the MemoryManager block.
     *
     * @return Status of the MemoryManager block.
     */
    MemoryManagerStatus_e status() const { return m_block_params.status; }

    /**
     * @brief Setter for the status of the MemoryManager block.
     *
     * @param status Status of the MemoryManager block.
     */
    void status(MemoryManagerStatus_e status) { m_block_params_update = true;
                                                m_block_params.status = status; }
	
	/**
	 *
	 */
	 MemoryBlockWrite_e write_flag(void) { return m_block_params.write_flag; }
    /**
     * @brief Setter for the write flag of the MemoryManager block.
     *
     * @param write_flag Write flag of the MemoryManager block.
     */
	void write_flag(MemoryBlockWrite_e write_flag) { m_block_params_update = true;}

    /**
     * @brief Getter for the block parameters.
     *
     * @return Pointer to the block parameters.
     */
    MemoryBlockParams_t *bock_parameter(void) { return &m_block_params; }

    /**
     * @brief Getter for the block parameters update flag.
     *
     * @return Block parameters update flag.
     */
     bool block_params_update(void) {return m_block_params_update;}

    /**
     * @brief Resets the block parameters update flag.
     */
	void reset_block_params_update(void) {m_block_params_update = false;}

    /**
     * @brief Get the data of the specified type.
     *
     * @tparam T Type of data.
     * @param type MemoryManagerType_e value indicating the type of data to retrieve.
     * @return Pointer to the data of the specified type.
     */
    template <typename T = uint8_t>
    T* get_data(MemoryManagerType_e type)
    {
         T *ret = NULL;
   
        switch (type)
        {
            case MemoryManagerType_e::USER_DATA:
                ret = user_data<T>();
                break;
            case MemoryManagerType_e::MEMORY_DATA:
                ret = eeprom_data<T>();
                break;
            case MemoryManagerType_e::DEFAULT_DATA:
                ret = default_data<T>();
                break;
            default:
                ret = NULL;
                break;
        }
        return ret;
    }
	
    /**
     * @brief Overloaded equality operator for comparing MemoryManagerBlock objects.
     *
     * @param other The other MemoryManagerBlock object to compare to.
     * @return True if the objects are equal, false otherwise.
     */
    inline bool operator==(const MemoryManagerBlock &other) { return compare(other) == 0; }

    /**
     * @brief Overloaded inequality operator for comparing MemoryManagerBlock objects.
     *
     * @param other The other MemoryManagerBlock object to compare to.
     * @return True if the objects are not equal, false otherwise.
     */
    inline bool operator!=(const MemoryManagerBlock &other) { return compare(other) != 0; }
private:
    uint16_t compare(const MemoryManagerBlock &other)
    {
        int16_t ret = 0;
        ret = memcmp(m_user_data, other.user_data(), m_data_length);
        ret += memcmp(m_eeprom_data, other.eeprom_data(), m_data_length);
        ret += memcmp(m_default_data, other.default_data(), m_data_length);
        return ret;
    }
private:
    uint8_t *m_user_data;               /**< Pointer to user data. */
    uint8_t *m_eeprom_data;             /**< Pointer to EEPROM data. */
    uint8_t *m_default_data;            /**< Pointer to default data. */
    uint32_t m_data_length;             /**< Length of data. */
    MemoryBlockParams_t m_block_params; /**< Boolean value indicating whether the data has been initialized. */
    bool m_block_params_update;
};

/**
 * @brief Union representing the version of a MemoryManager.
 */
union MemoryManagerVersion_t
{
    struct Version_t
    {
        uint8_t patch;    /**< Patch version number. */
        uint8_t minor;    /**< Minor version number. */
        uint8_t major;    /**< Major version number. */
        uint8_t reserved; /**< Reserved byte. */
    } str;

    uint32_t u32; /**< Version as a 32-bit unsigned integer. */
    
    /**
     * @brief Overloaded equality operator for comparing MemoryManagerVersion_t objects.
     *
     * @param other The other MemoryManagerVersion_t object to compare to.
     * @return True if the objects are equal, false otherwise.
     */
    bool operator==(const MemoryManagerVersion_t &other) const { return u32 == other.u32; }

    /**
     * @brief Overloaded greater than operator for comparing MemoryManagerVersion_t objects.
     *
     * @param other The other MemoryManagerVersion_t object to compare to.
     * @return True if this object is greater than the other object, false otherwise.
     */
    bool operator>(const MemoryManagerVersion_t &other) const {return u32 > other.u32 ; }

    /**
     * @brief Overloaded greater than or equal to operator for comparing MemoryManagerVersion_t objects.
     *
     * @param other The other MemoryManagerVersion_t object to compare to.
     * @return True if this object is greater than or equal to the other object, false otherwise.
     */
    bool operator>=(const MemoryManagerVersion_t &other) const {return u32 >= other.u32; }

    /**
     * @brief Overloaded less than operator for comparing MemoryManagerVersion_t objects.
     *
     * @param other The other MemoryManagerVersion_t object to compare to.
     * @return True if this object is less than the other object, false otherwise.
     */
    bool operator<(const MemoryManagerVersion_t &other) const {return  u32 < other.u32; }

    /**
     * @brief Overloaded less than or equal to operator for comparing MemoryManagerVersion_t objects.
     *
     * @param other The other MemoryManagerVersion_t object to compare to.
     * @return True if this object is less than or equal to the other object, false otherwise.
     */
    bool operator<=(const MemoryManagerVersion_t &other) const {return u32 <= other.u32; }
};


/**
 * @brief Struct representing the header of a MemoryManager.
 */
struct MemoryManagerHeader_t
{
    MemoryManagerVersion_t version; /**< Version of the MemoryManager. */
    MemoryManagerStatus_e key;     /**< Status key of the MemoryManager. */

#if MEMORRY_CRC_ENABLED == true
    uint32_t crc; /**< CRC value of the MemoryManager. */
#endif
};

/**
 * @brief Struct representing the configuration of a MemoryManager.
 */
struct MemoryManagerConfig_t
{
    MemoryManagerVersion_t version;     /**< Version of the MemoryManager. */
    MemoryManagerBlock *storage;        /**< Pointer to the storage of MemoryManagerBlocks. */
    uint16_t number_of_storage;         /**< Number of MemoryManagerBlocks in the storage. */
};

#endif