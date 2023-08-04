#include <iostream>
#include <stdlib.h>
#include <string>
#include <limits>
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

// The LRU Cache class
class LRUCache
{
private:
    Cache cache;

    // A helper function to check if cache has reached its full capacity
    inline bool isCacheFull()
    {
        return cache.index.size() == cache.capacity;
    };

    // A helper function to check if cache has no page in it
    inline bool isCacheEmpty()
    {
        return cache.index.size() == 0;
    };

    // A helper function to instantiate a new MemoryPage node
    MemoryPage *newMemoryPage(int key, string data)
    {
        MemoryPage *newPage = new MemoryPage(key, data);
        return newPage;
    };

    // A helper function to get more room on cache's memory by deleting the page at last position in cache's memory
    void makeARoom()
    {
        if (!isCacheFull())
            return;

        if (cache.memory.head == cache.memory.tail) // If cache's memory has just one page in it, updates the 'memory.head' pointer to NULL
            cache.memory.head = NULL;

        // Makes 'memory.tail' pointer to point to the memory page preceeding the one being removed
        MemoryPage *pageToBeRemoved = cache.memory.tail;
        cache.memory.tail = pageToBeRemoved->prev;
        if (cache.memory.tail != NULL)
            cache.memory.tail->next = NULL;

        cout << "Cache is full. Removing key " << pageToBeRemoved->key << " from cache before inserint a new one..."
             << endl;

        cache.index.erase(pageToBeRemoved->key);
        delete pageToBeRemoved;
    };

    void dump()
    {
        cout << "Cache's memory state: "
             << "[  ";

        MemoryPage *memoryPage = cache.memory.head;

        while (memoryPage != NULL)
        {
            cout << memoryPage->key << " => " << memoryPage->data << ",  ";
            memoryPage = memoryPage->next;
        }

        cout << "]" << endl;
    };

    void dumpIndex()
    {
        cout << "Cache's index state: "
             << "[  ";

        for (CacheIndex::iterator indexEntry = cache.index.begin();
             indexEntry != cache.index.end(); ++indexEntry)
            cout << indexEntry->first << " => " << indexEntry->second << ",  ";

        cout << "]" << endl;
    };

public:
    LRUCache()
    {
        build(0); // Invokes build() function with argument 0 so as to safely initialize cache's fundamental properties by the time of the class instantiation, leaving it at a ready state
    };

    // Creates a cache with N possible entries
    // build(0) emptys the cache
    void build(int n)
    {
        if (n > 0)
            cout << "Building a LRU Cache with capacity for " << n << " entries..." << endl;
        else
            cout << "Setting cache into a new empty state..." << endl;

        cache.capacity = n;

        for (CacheIndex::iterator indexEntry = cache.index.begin();
             indexEntry != cache.index.end(); ++indexEntry)
        {
            cache.index.erase(indexEntry->first);
            delete (indexEntry->second);
        }

        cache.memory.head = cache.memory.tail = NULL;
        dump();
        dumpIndex();
    };

    void destruct()
    {
        cout << "Destructing LRU Cache along with all data related to it..." << endl;
        build(0);
    };

    void add(int key, string data)
    {
        cout << "Adding value '" << data << "' to key: " << key << endl;

        CacheIndex::iterator indexEntry = cache.index.find(key);

        if (indexEntry != cache.index.end())
        {
            cout << "Key " << key << " has already an entry in the cache. Replacing it..." << endl;

            bool isHead = false;
            bool isTail = false;

            if (cache.memory.head == indexEntry->second)
            {
                isHead = true;

                cache.memory.head = indexEntry->second->next;

                if (cache.memory.head != NULL)
                {
                    cache.memory.head->prev = NULL;
                }
            }

            if (cache.memory.tail == indexEntry->second)
            {
                isTail = true;
                cache.memory.tail = indexEntry->second->prev;

                if (cache.memory.tail != NULL)
                {
                    cache.memory.tail->next = NULL;
                }
            }

            if (!isHead && !isTail)
            {
                indexEntry->second->prev->next = indexEntry->second->next;
                indexEntry->second->next->prev = indexEntry->second->prev;
            }

            delete (indexEntry->second);
        }
        else if (isCacheFull())
        {
            makeARoom();
        }

        MemoryPage *newPage = newMemoryPage(key, data);
        newPage->next = cache.memory.head;

        if (isCacheEmpty())
        {
            cache.memory.head = cache.memory.tail = newPage;
        }
        else
        {
            cache.memory.head->prev = newPage;
            cache.memory.head = newPage;
        }

        cache.index[key] = newPage;
        dump();
        dumpIndex();
    };

    string get(int key)
    {
        cout << "Reading value from key: " << key << endl;

        CacheIndex::iterator indexEntry = cache.index.find(key);

        if (indexEntry != cache.index.end())
        {
            MemoryPage *foundMemoryPage = indexEntry->second;
            if (foundMemoryPage != cache.memory.head)
            {
                foundMemoryPage->prev->next = foundMemoryPage->next;
                if (foundMemoryPage->next)
                    foundMemoryPage->next->prev = foundMemoryPage->prev;

                // If the requested page is at last position in cache's memory, then updates 'memory.tail' pointer as long as it is going to be moved to the first position now
                if (foundMemoryPage == cache.memory.tail)
                {
                    cache.memory.tail = foundMemoryPage->prev;
                    cache.memory.tail->next = NULL;
                }

                // Places the requested page before current first page in cache's memory
                foundMemoryPage->next = cache.memory.head;
                foundMemoryPage->prev = NULL;
                foundMemoryPage->next->prev = foundMemoryPage;

                // Updates 'memory.head' pointer to the requested page's address
                cache.memory.head = foundMemoryPage;

                dump();
                dumpIndex();
            }
            return foundMemoryPage->data;
        }
        else
        {
            cerr << "Cache page fault: Key " << key << " does not exist in cache!" << endl;
            throw std::invalid_argument("Unknown key!");
        }
    };
};

int main()
{
    
    LRUCache lruCache;
    while (true)
    {
        int choice;
        cout << "1->Build Cache" << endl;
        cout << "2->Add data to the cache" << endl;
        cout << "3->Get data to the cache" << endl;
        cout << "4->Delete the cache" << endl;
        cout << "5->Exit" << endl;
        cout << "Enter your choice: ";

        if (!(cin >> choice))
        {
            // Clear the error state and discard the invalid input
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Invalid choice! Please enter a valid number.\n"
                 << endl;
            continue;
        }
        int key;
        string data;
        switch (choice)
        {
        case 1:
            int n;
            cout << "Enter the capacity of the cache: ";
            while (true)
            {
                cin >> n;
                if (n > 0)
                    break;
                cout << "Invalid capacity! Please enter a positive integer." << endl;
            }
            lruCache.build(n);
            break;

        case 2:
            cout << "Enter the key: ";
            cin >> key;
            cout << "Enter the data: ";
            cin >> data;
            lruCache.add(key, data);
            break;

        case 3:
            cout << "Enter the key: ";
            cin >> key;
            try
            {
                cout << "Value '" << lruCache.get(key) << "' read." << endl;
            }
            catch (const std::invalid_argument &e)
            {
                cerr << "An error has occurred while reading data from cache: " << e.what() << endl;
            }
            break;

        case 4:
            lruCache.destruct();
            break;

        case 5:
            cout << "Exiting..." << endl;
            exit(0);
            break;

        default:
            cout << "Invalid choice!" << endl;
            break;
        }
    }
    return 0;
}