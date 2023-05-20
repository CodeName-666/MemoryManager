#ifndef MEMOR_MANGER_TYPES_H
#define MEMOR_MANGER_TYPES_H

#include "stdio.h"
#include "string.h"
#include "MemoryCrc/MemoryCrc_Types.h"

#define MEMORY_HEADER_ENTRY 0u

enum MemoryDataCompare_e
{
    MEMORY_DATA_NOT_EQUAL = 0x00,
    MEMORY_DATA_EQUAL = 0x01,
};   


enum MemoryManagerStatus_e
{
    STATUTS_OK    = 0x33,
    STATUS_NOT_OK = 0x22,
    STATUS_UNINIT = 0x11,
    STATUS_ERROR  = 0x00
};

enum MemoryManagerType_e
{
    USER_DATA = 0x00,
    MEMORY_DATA,
    DEFAULT_DATA
};

enum MemoryBlockWrite_e
{
    WRITE_NOT_CONFIGURED = 0x00,
    WRITE_CYCLIC, /*Not implemented yet*/
    WRITE_ON_CHANGE,
    WRITE_ON_SHUTDOWN, /*Not implemented yet*/
/*--- Special Key ------------------------------------*/
    WRITE_ALL_BLOCKS,

};

struct MemoryBlockParams_t
{
    MemoryManagerStatus_e status;
    MemoryBlockWrite_e write_flag;

#if MEMORRY_CRC_ENABLED == true
    MemoryCrcType_e crc_type;
#endif
    uint32_t cycle_time;
};

/**
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

    }*/

    /**
     * @brief Getter for m_user_data.
     *
     * @return Pointer to user data.
     */
    template <typename T = uint8_t>
    T *user_data() const { return reinterpret_cast<T *>(m_user_data); }

    /**
     * @brief Setter for m_user_data.
     *
     * @param user_data Pointer to user data.
     */
    template <typename T = uint8_t>
    void user_data(T *user_data) { m_user_data = reinterpret_cast<uint8_t *>(user_data); }

    /**
     * @brief Getter for m_eeprom_data.
     *
     * @return Pointer to EEPROM data.
     */
    template <typename T = uint8_t>
    T *eeprom_data() const { return reinterpret_cast<T *>(m_eeprom_data); }

    /**
     * @brief Setter for m_eeprom_data.
     *
     * @param eeprom_data Pointer to EEPROM data.
     */
    template <typename T = uint8_t>
    void eeprom_data(T *eeprom_data) { m_eeprom_data = eeprom_data; }

    /**
     * @brief Getter for m_default_data.
     *
     * @return Pointer to default data.
     */
    template <typename T = uint8_t>
    T *default_data() const { return reinterpret_cast<T *>(m_default_data); }

    /**
     * @brief Setter for m_default_data.
     *
     * @param default_data Pointer to default data.
     */
    template <typename T = uint8_t>
    void default_data(T *default_data) { m_default_data = reinterpret_cast<T *>(default_data); }

    /**
     * @brief Getter for m_data_length.
     *
     * @return Length of data.
     */
    uint32_t data_length() const { return m_data_length; }

    /**
     * @brief Setter for m_data_length.
     *
     * @param data_length Length of data.
     */
    void data_length(uint32_t data_length) { m_data_length = data_length; }

    /**
     * @brief Getter for m_init.
     *
     * @return MemoryManagerInit_e value indicating whether the data has been initialized.
     */
    MemoryManagerStatus_e status() const { return m_block_params.status; }

    /**
     * @brief Setter for m_init.
     *
     * @param status MemoryManagerInit_e value indicating whether the data has been initialized.
     */
    void status(MemoryManagerStatus_e status) { m_block_params_update = true;
                                                m_block_params.status = status; }

    MemoryBlockWrite_e write_flag(void) { return m_block_params.write_flag; }

    MemoryBlockParams_t *bock_parameter(void) { return &m_block_params; }

    void write_flag(MemoryBlockWrite_e write_flag) { m_block_params_update = true;}
    
    
    bool block_params_update(void) {return m_block_params_update;}

    void reset_block_params_update(void) {m_block_params_update = false;}

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

    inline bool operator==(const MemoryManagerBlock &other) { return compare(other) == 0; }

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

union MemoryManagerVersion_t
{
    struct Version_t
    {
        uint8_t patch;
        uint8_t minor;
        uint8_t major;
        uint8_t reserved;
    } str;

    uint32_t u32;
    
    inline bool operator==(const union MemoryManagerVersion_t &other) { return u32 == other.u32; }
    inline bool operator > ( const union MemoryManagerVersion_t &other) {return u32 > other.u32 ; }
    inline bool operator >= ( const union MemoryManagerVersion_t &other) {return u32 >= other.u32; }
    
    inline bool operator < ( const union MemoryManagerVersion_t &other) {return  u32 < other.u32; }
    inline bool operator <= ( const union MemoryManagerVersion_t &other) {return u32 <= other.u32; }
    
};

struct MemoryManagerHeader_t
{
    MemoryManagerVersion_t version;
    MemoryManagerStatus_e key;

#if MEMORRY_CRC_ENABLED == true
    uint32_t crc;
#endif
};

struct MemoryManagerConfig_t
{
    MemoryManagerVersion_t version;
    MemoryManagerBlock *storage;
    uint16_t number_of_storage;
};

#endif