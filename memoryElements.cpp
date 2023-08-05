#include <iostream>
#include <map>

using namespace std;

// A single MemoryPage in the Memory area of the cache
typedef struct MemoryPage
{
    MemoryPage *prev, *next; // a pointer to the previous and next memory pages
    string data;             // the actual data which the memory page carries
    int key;                 // the key that identifies the data carried in the memory page

    MemoryPage(int key, string data)
    {
        this->data = data;
        this->key = key;
        prev = next = NULL;
    };
} MemoryPage;

// Memory Area of the cache
typedef struct Memory
{
    MemoryPage *head, *tail; // a pointer to the page at first and last position in memory
} Memory;

// A helper custom type to store cache's index as a associative key-value container
typedef map<int, MemoryPage *> CacheIndex;

// The cache itself
typedef struct Cache
{
    int capacity;     // how many memory pages cache is allowed to have
    Memory memory;    // the memory area used by the cache
    CacheIndex index; // a map for indexing entries in cache
} Cache;