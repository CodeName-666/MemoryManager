MemoryManager

The MemoryManager class provides an interface to store and manage data using a specified storage interface. It allows you to add and remove storage entries, read and write data to the storage interface, and perform various memory management operations.
Table of Contents

    Usage
    Examples
    Methods
    Data Types
    Contributing
    License

Usage

To use the MemoryManager class in your project, follow these steps:

    Include the MemoryManager.h header file in your source file:

cpp

#include "MemoryManager.h"

    Create an instance of the MemoryManager class:

cpp

MemoryManager memoryManager;

    Configure the storage interface and memory settings by adding configurations and interfaces using the provided methods:

cpp

MemoryManagerConfig_t config;
// Set the configuration values
memoryManager.add_config(config);

Memory memoryInterface;
// Initialize the memory interface
memoryManager.add_interface(memoryInterface);

    Initialize the storage interface:

cpp

memoryManager.begin();

    Use the available methods to perform memory management operations, such as adding and removing storage entries, reading and writing data, and verifying memory integrity.

cpp

// Add a new storage entry
MemoryManagerBlock newMemoryBlock;
uint16_t entryIndex = memoryManager.add_block(newMemoryBlock);

// Read data from a storage entry
memoryManager.read_entry(entryIndex);

// Write data to the storage interface
memoryManager.write_all();

// Remove a storage entry
memoryManager.remove_block(entryIndex);

    When you're done using the MemoryManager instance, you can shut it down:

cpp

memoryManager.shutdown();

Examples

Here are a few examples that demonstrate the usage of the MemoryManager class:

    Adding a Storage Entry:

cpp

MemoryManager memoryManager;
MemoryManagerBlock newMemoryBlock;

// Set the necessary parameters of the new memory block

// Add the memory block to the storage
uint16_t entryIndex = memoryManager.add_block(newMemoryBlock);

    Reading Data from a Storage Entry:

cpp

MemoryManager memoryManager;
uint16_t entryIndex = 0; // Index of the storage entry to read

// Read the storage entry
memoryManager.read_entry(entryIndex);

    Writing Data to the Storage Interface:

cpp

MemoryManager memoryManager;

// Write all storage entries to the storage interface
memoryManager.write_all();

    Removing a Storage Entry:

cpp

MemoryManager memoryManager;
uint16_t entryIndex = 0; // Index of the storage entry to remove

// Remove the storage entry
memoryManager.remove_block(entryIndex);

Methods

Here are the available methods of the MemoryManager class:

    MemoryManager(): Default constructor for the MemoryManager class.
    MemoryManager(Memory& interface, MemoryManagerConfig_t& config): Constructor for the MemoryManager class with parameters for the storage interface and configuration.
    void add_config(MemoryManagerConfig_t& config): Adds a MemoryManagerConfig_t object to the storage configuration.
    void add_interface(Memory& interface): Adds an IStorage object to the storage interface.
    void begin(): Initializes the storage interface.
    void loop(): Runs the main storage loop.
    uint16_t add_block(MemoryManagerBlock& new_memory_block): Adds a new storage entry to the data list and returns its index.
    void remove_block(MemoryManagerBlock& memory_block): Removes a storage entry from the data list.
    void remove_block(uint16_t entry_index): Removes a storage entry from the data list based on its index.
    void read_entry(uint16_t entry_index): Reads data from a storage entry based on its index.
    void write_entry(uint16_t entry_index): Writes data to a storage entry based on its index.
    void write_all(): Writes all storage entries to the storage interface.
    void shutdown(): Shuts down the MemoryManager instance.

Data Types

The MemoryManager module defines several data types used in its implementation. Here are the descriptions of those data types:

    MemoryDataCompare_e: An enumeration representing the comparison result of memory data.
    MemoryManagerStatus_e: An enumeration representing the status of the MemoryManager instance.
    MemoryManagerType_e: An enumeration representing the type of memory data.
    MemoryBlockWrite_e: An enumeration representing the write behavior of a memory block.
    MemoryBlockParams_t: A structure holding the parameters of a memory block.
    MemoryManagerBlock: A class representing a block of memory managed by the MemoryManager.
    MemoryManagerVersion_t: A union representing the version of the MemoryManager.
    MemoryManagerHeader_t: A structure representing the header of the MemoryManager.
    MemoryManagerConfig_t: A structure representing the configuration of the MemoryManager.

Contributing

Contributions to the MemoryManager module are welcome! If you find any issues or want to extend its functionality, feel free to open a pull request on the GitHub repository.
License

The MemoryManager module is open-source and released under the MIT License. Feel free to use and modify the code according to your needs.