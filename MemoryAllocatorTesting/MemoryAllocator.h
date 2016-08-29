// Made following tutorial at http://www.gamedev.net/page/resources/_/technical/general-programming/c-custom-memory-allocation-r3010
#pragma 
#include <stdint.h>
#include <cassert>




class MemoryAllocator
{
public:
    MemoryAllocator(size_t p_size, void* p_start)
        :m_size(p_size), m_start(p_start), m_numAllocations(0), m_usedMemory(0)
    {
        // Empty
    }

    virtual ~MemoryAllocator()
    {
        // Ensure we don't have any memory being leaked
        assert(m_numAllocations == 0 && m_usedMemory == 0);

        m_start = nullptr;
        m_size = 0;
    }

    // Allocates the specified number of memory. 
    virtual void* allocate(size_t p_size, uint8_t alignment = 4) = 0;
    // Deallocates a given pointer (I guess??)
    virtual void deallocate(void* p_pointer) = 0;

    /// Getters for member variables
    // Returns start pointer
    void* GetStart() const { return m_start; }
    // Returns size of allocated memory
    size_t GetSize() const { return m_size; }
    // Gets amount of used memory
    size_t GetUsedMemory() const { return m_usedMemory; }
    // Gets number of allocations made
    size_t GetNumAllocations() const { return m_numAllocations; 
    }

protected:
    // Pointer to the start of the allocated memory
    void* m_start;
    // Total size of allocated memory
    size_t m_size;

    // Amount of memory used (in bytes?)
    size_t m_usedMemory;
    // Number of total allocations made
    size_t m_numAllocations;
};

namespace MemoryAllocator
{
    // Template method which allocates whatever is needed using a desired allocator
    template<class T> T* AllocateNew(MemoryAllocator& p_allocator)
    {
        return new (p_allocator.allocate(sizeof(T), __alignof(T))) T;
    }

    // Allocates new memory base on an already existing object (copy)
    template<class T> T* AllocateNew(MemoryAllocator& p_allocator, const T& t)
    {
        return new (p_allocator.allocate(sizeof(T), __alignof(T))) T(t); // Confusing syntax
    }

    // Removes memory for given object
    template<class T> T* DeallocateDelete(MemoryAllocator& p_allocator, T& p_object)
    {
        // Call destructor of object
        p_object.~T();
        p_allocator.deallocate(&p_object);

    }

    // Allocates memory for an array of p_length number of elements
    template<classT> T* AllocateArray(MemoryAllocator& p_allocator, size_t p_length)
    {
        assert(p_length != 0); // Useless to allocate room for 0 objects

        uint8_t t_headerSize = sizeof(size_t) / sizeof(T);

        // Check if object is properly aligned. If not, increase header size
        if (sizeof(size_t) % sizeof(T) > 0)
            t_headerSize++;

        // Allocate extra space to store array length in the bytes before the array
        T* t_array = ((T*)allocator.allocate(sizeof(T)*(p_length + t_headerSize), __alignof(T))) + t_headerSize;
        // Store the length
        *(((size_t*)t_array) - 1) = p_length;
        // Store the values
        for (size_t i = 0; i < p_length; i++)
        {
            new (&t_array[i]) T; // This syntax is confusing...
        }

        return t_array;
    }

    // Deallocates specified array
    template<class T> void DeallocateArray(MemoryAllocator& p_allocator, T* p_array)
    {
        assert(p_array != nullptr);

        // Get length of array
        size_t t_arrayLength = *(((size_t*)t_arrayLength) - 1);

        // Deallucate all values
        for (size_t i = 0; i < t_arrayLength; i++)
        {
            p_array[i].~T();
        }

        // Deallocate header size
        uint8_t t_headerSize = sizeof(size_t) / sizeof(T);

        if (sizeof(size_t) % sizeof(T))
            t_headerSize += 1;

        p_allocator.deallocate(p_array - t_headerSize);
    }

}

