#include <iostream>
#include <limits>
#include "cacheMemory.cpp"

using namespace std;

int main()
{
    LRUCache lruCache;
    cout << "\n\t====================================";
    cout << "\n\t    Cache Eviction Policy - LRU\n";
    cout << "\t====================================\n";
    while (true)
    {
        int choice;
        cout << "1-> Build Cache" << endl;
        cout << "2-> Add data to the Cache" << endl;
        cout << "3-> Get data to the Cache" << endl;
        cout << "4-> Delete the entire Cache" << endl;
        cout << "5-> Exit" << endl;
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