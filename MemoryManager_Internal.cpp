#include "MemoryManager.h"
#include "MemoryManager_Debug.h"

bool MemoryManager::read_all(void)
{
    bool read_all_blocks_valid = true;
    MemoryManagerBlock *memory_block;

    for (uint16_t i = 1; i < m_memory_block_list.size(); i++)
    {
        read_block_parameter(i); 
        read_entry(i);            

        if(block_parameter_are_valid(i) == false)
        {
            read_all_blocks_valid = false;
            DBIF_LOG_DEBUG_0("Invalid Block Parameter_Nr: %i", i);
        }
    }
    return read_all_blocks_valid;
}

void MemoryManager::read_entry(uint16_t entry_idx)
{
    bool ret = false;
    uint16_t offset, memory_data_len;
    uint16_t read_bytes = 0;
    MemoryManagerBlock *memory_block;
    uint8_t *memory_data, *user_data;

    DBIF_LOG_INFO(".... Read Entry: %i ....", entry_idx);

    memory_block = &m_memory_block_list[entry_idx];
    memory_data_len = m_memory_block_list[entry_idx].data_length();

    memory_data = memory_block->get_data(MemoryManagerType_e::MEMORY_DATA);
    user_data   = memory_block->get_data(MemoryManagerType_e::USER_DATA);

    DBIF_LOG_DEBUG_0("User Data = %i, Memory Data = %i, Len= %i",user_data, memory_data, memory_data_len);

    offset = get_offest(entry_idx);
    read_bytes = m_memory_interface->read(memory_data, offset, memory_data_len);
    copy_data(memory_data, user_data, memory_data_len);  
}

void MemoryManager::read_block_parameter(uint16_t entry_idx)
{
    uint16_t offset, memory_data_len;
    MemoryManagerBlock *memory_block;
    MemoryBlockParams_t *memory_bock_params;

    memory_block       = &m_memory_block_list[entry_idx];
    memory_data_len    = m_memory_block_list[entry_idx].data_length();
    memory_bock_params = m_memory_block_list[entry_idx].bock_parameter();

    offset = get_offest(entry_idx);
    
    m_memory_interface->read(reinterpret_cast<uint8_t *>(memory_bock_params),
                             offset + memory_data_len,
                             sizeof(MemoryBlockParams_t));
    memory_block->reset_block_params_update();
}

uint16_t MemoryManager::get_offest(uint16_t idx)
{
    uint16_t offset = 0;
   
    if(idx > 0)
    {
        DBIF_LOG_DEBUG_0("Start Offset Calculation: ");
        DBIF_LOG_DEBUG_0("--------------------------");
        
        DBIF_LOG_DEBUG_0("Idx | LEN | SIZE  | SUM");

        for (uint16_t i = 0; i < idx; i++)
        {
            offset += m_memory_block_list.at(i).data_length() + sizeof(MemoryBlockParams_t);
            DBIF_LOG_DEBUG_0(" %i :  %i +  %i  = %i", idx, m_memory_block_list.at(i).data_length(), sizeof(MemoryBlockParams_t),  m_memory_block_list.at(i).data_length() + sizeof(MemoryBlockParams_t) );
        }
        
    }
    else
    {
        DBIF_LOG_DEBUG_0("Skip Offset Calculation: ");
    }
    DBIF_LOG_DEBUG_0("SUM %i: Offset = %i",idx, offset);
    return offset;
}

void MemoryManager::copy_data(uint8_t *source, uint8_t *destination, int16_t size)
{
    if(source != NULL && destination != NULL && size >0)
    {
        DBIF_LOG_DEBUG_0("Source: %i | Destination: %i | Length: %i",source,destination, size );
        memcpy(destination, source, size);
    }
    else 
    {
         DBIF_LOG_ERROR("Invalid Parameter: Source: %i | Destination: %i | Length: %i ", source,destination,size); 
    }
}



bool MemoryManager::write_all(MemoryBlockWrite_e write_flag, MemoryManagerType_e type)
{
    bool write_success = false;
    MemoryManagerBlock *current_block;

    for (uint16_t i = 1; i < m_memory_block_list.size(); i++)
    {   
        current_block = &m_memory_block_list[i];
        
        if ((current_block->write_flag() == write_flag) ||
            (write_flag == MemoryBlockWrite_e::WRITE_ALL_BLOCKS))
        {
           // DBIF_LOG_DEBUG_0("Entry %i: Request to write", i);
            if(compare_entry(i,type)== MemoryDataCompare_e::MEMORY_DATA_NOT_EQUAL)
            {       
                DBIF_LOG_DEBUG_0("Entry %i: Differenzes found", i);
                write_success = write_entry(i, type);
                if(write_success)
                {
                    DBIF_LOG_DEBUG_0("Update Block Parameter for Entry  %i",i);
                    write_success = write_block_parameter(i);
                }
            }
        }
    }
    return write_success;
}

bool MemoryManager::write_entry(uint16_t entry_idx, MemoryManagerType_e data_type)
{
    uint16_t offset, data_len;
    MemoryManagerBlock *memory_block;
    uint8_t *memory_data, *write_data;
    uint16_t bytes_written = 0;

    memory_block = &m_memory_block_list[entry_idx];
    memory_data  = memory_block->get_data(MemoryManagerType_e::MEMORY_DATA);
    write_data   = memory_block->get_data(data_type);
    data_len     = memory_block->data_length();

    memory_block->status(MemoryManagerStatus_e::STATUS_NOT_OK);
    
    if(memory_data != NULL && write_data != NULL)
    {
        copy_data(write_data, memory_data, data_len);
        offset = get_offest(entry_idx);
        bytes_written = m_memory_interface->write(memory_data, offset, data_len);
        if(bytes_written)
        {
            DBIF_LOG_DEBUG_0("Entry %i: Successfully written to Memory");
            memory_block->status(MemoryManagerStatus_e::STATUTS_OK);
        }
        else 
        {
             DBIF_LOG_ERROR("Entry %i: Error during write progress to Memory");
        }
    }
    else
    { 
        DBIF_LOG_ERROR("Entry %i: Memory_Data = %i | Write_Data = %i", entry_idx, memory_data, write_data);
    }
        
    return (bytes_written > 0 ? true : false);
}

bool MemoryManager::write_block_parameter(uint16_t entry_idx)
{
    bool update_parameter = false;
    uint16_t offset, data_len;
    MemoryManagerBlock *memory_block;
    MemoryBlockParams_t *parameter;
    uint8_t *memory_data, *write_data;

    memory_block = &m_memory_block_list[entry_idx];

    if (memory_block->block_params_update())
    {
        offset = get_offest(entry_idx) + memory_block->data_length();
        parameter = m_memory_block_list[entry_idx].bock_parameter();

        m_memory_interface->write(reinterpret_cast<uint8_t *>(parameter),
                                  offset,
                                  sizeof(MemoryBlockParams_t));
        update_parameter = true;
    }
    
    return update_parameter;
}


MemoryDataCompare_e MemoryManager::compare_data(uint8_t *source_1, uint8_t *source_2, int16_t size)
{
    MemoryDataCompare_e ret = MemoryDataCompare_e::MEMORY_DATA_NOT_EQUAL;
    
    if(source_1 != NULL && source_2 != NULL && size > 0)
    {
       
        if (memcmp(source_1, source_2, size) == 0) // MEMORY IS EQUAL
        {   
            ret = MemoryDataCompare_e::MEMORY_DATA_EQUAL;
        }
    }
    else 
    {
        DBIF_LOG_ERROR("Invalid Parameter: \r\n \tSource_1: %i,\r\n \t Source_2: %i,\r\n \t Length: %i ", source_1,source_2,size); 
    }

    return ret;
}

int16_t MemoryManager::exists(MemoryManagerBlock &memory_block)
{
    bool found = false;
    uint16_t idx = -1;
    for (uint16_t i = 0; (i < m_memory_block_list.size()) && (found == false); i++)
    {
        if (m_memory_block_list[i] == memory_block)
        {
            idx = i;
            found = true;
        }
    }
    return idx;
}

void MemoryManager::delete_entry(uint16_t entry_idx, bool delete_memory_data)
{
    MemoryManagerBlock block;
    if(entry_idx < m_memory_block_list.size())
    {
        block = m_memory_block_list.at(entry_idx);
        memset(block.user_data(), 0x00, block.data_length());
        write_entry(entry_idx, USER_DATA);

        if (delete_memory_data)
        {
            write_to_memory();
        }

        m_memory_block_list.erase(m_memory_block_list.begin() + entry_idx);
        m_memory_block_param_list.erase(m_memory_block_param_list.begin() + entry_idx);
    }
    else
    {
        DBIF_LOG_ERROR("Invalid Index");
    }
}

bool MemoryManager::block_parameter_are_valid(uint16_t entry_idx)
{   
    bool ret = true;
    
    //MemoryBlockParams_t *parameter = m_memory_block_list[entry_idx].bock_parameter();
    //if (parameter->status == MemoryManagerStatus_e::STATUTS_OK)
    //{
    //    DBIF_LOG_DEBUG_0("Block Parameter Valid Entry: %i",entry_idx);
    //    ret = true;
    //} 
    //else
    //{
    //    DBIF_LOG_DEBUG_0("Block Parameter Invalid Entry: %i",entry_idx);
    //}
    return ret;
}


bool MemoryManager::write_to_memory(void)
{
    return m_memory_interface->commit();
}


MemoryManagerStatus_e MemoryManager::verify_memory(void)
{
    MemoryManagerStatus_e ret = MemoryManagerStatus_e::STATUS_ERROR;
    bool blocks_valid = false;
    // Version compare to check whether new Version available

    MemoryManagerVersion_t memory_version = get_block(MEMORY_HEADER_ENTRY)->user_data<MemoryManagerHeader_t>()->version;
    
    DBIF_LOG_DEBUG_0("m_current_version <= memory_version = %i",m_current_version <= get_block(MEMORY_HEADER_ENTRY)->user_data<MemoryManagerHeader_t>()->version);
    DBIF_LOG_DEBUG_1("Current Version: %i.%i.%i",m_current_version.str.major,m_current_version.str.minor,m_current_version.str.patch);
    DBIF_LOG_DEBUG_1("Memory  Version: %i.%i.%i",memory_version.str.major,memory_version.str.minor,memory_version.str.patch);


    if (m_current_version <= memory_version)
    {
        
        DBIF_LOG_DEBUG_0("Memory Version up to date");
        
        blocks_valid = read_all();
        
        if(blocks_valid)
        {
            DBIF_LOG_DEBUG_0("Memory Init Successful");            
            ret = MemoryManagerStatus_e::STATUTS_OK;
        }
        else
        {
            DBIF_LOG_ERROR("Memory Init ERROR");        
            ret = MemoryManagerStatus_e::STATUS_ERROR;
        }
    }
    else
    {
        DBIF_LOG_DEBUG_0("Memory Version update needed, Write Default Data");
        
        // Update of MEMORY. Version differs.
        blocks_valid = write_all(WRITE_ALL_BLOCKS, DEFAULT_DATA);

        DBIF_LOG_DEBUG_1("Update Status = %i",blocks_valid);

        if(blocks_valid)
        {
            
            update_memory_version(m_current_version);
            blocks_valid = write_entry(MEMORY_HEADER_ENTRY);
            DBIF_LOG_DEBUG_1("WRITE Entry Status: %i", blocks_valid);

            blocks_valid = write_to_memory();
            DBIF_LOG_DEBUG_1("WRITE To Memory Status: %i", blocks_valid);

            if(blocks_valid)
            {
                ret = MemoryManagerStatus_e::STATUS_UNINIT;
            }
            else 
            {
                DBIF_LOG_ERROR("Memory Version Update Failed");
            }
        }
        else 
        {
            DBIF_LOG_ERROR("Memory Init Write Default Data ERROR");
            ret = MemoryManagerStatus_e::STATUS_NOT_OK;
        }
    }
    return ret;
}


void MemoryManager::set_memory_verification(MemoryManagerStatus_e status)
{

}


void MemoryManager::update_memory_version(MemoryManagerVersion_t new_version)
{
    MemoryManagerBlock* header_block = get_block(MEMORY_HEADER_ENTRY);
    MemoryManagerHeader_t *header = header_block->get_data<MemoryManagerHeader_t>(MemoryManagerType_e::USER_DATA);
    header->version.u32 = new_version.u32;
}

MemoryDataCompare_e MemoryManager::compare_entry(uint16_t entry_idx,MemoryManagerType_e data_type)
{
    MemoryManagerBlock block;
    MemoryDataCompare_e ret = MemoryDataCompare_e::MEMORY_DATA_NOT_EQUAL;
    if(entry_idx < m_memory_block_list.size())
    {
        block = m_memory_block_list[entry_idx];
        ret = compare_data(block.eeprom_data(),block.get_data(data_type),block.data_length());
        
    }
    return ret;
}