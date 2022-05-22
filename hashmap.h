#include "LinkedList.h"
//Using Programming 2 Linked List project code
#include <string>
#include <cstring>
#include <utility>
#include <math.h>
using namespace std;

unsigned int hashFunction(char const* key, int table_size);

class UnorderedMap {
    private:
        //Vector structure
        //Separate Chaining Collision Resolution
        //Each element will contain a pair of string IDs and names
        //Key = ID       Value = Name
        vector<LinkedList<pair<string,string>>*> umap;
        double LF;
        unsigned int numItems;
        int bucketCount;
    public:
    class Iterator {
            //Map object
            const UnorderedMap* mapPtr;
            //Node pointer to keep track of linked list position 
            LinkedList<pair<string,string>>::Node* itemPtr;
            //Keeping track of vector index in the overall container
            unsigned int vectorIndex;

            public:
                Iterator(LinkedList<pair<string,string>>::Node* ListNodePtr, unsigned int i, const UnorderedMap& obj);
                std::pair<string, string> operator*() const;
                Iterator& operator++();
                bool operator!=(Iterator const& rhs);
                Iterator& operator=(Iterator const& rhs);
                bool operator==(Iterator const& rhs);
        };
        UnorderedMap(unsigned int bucketCount, double loadFactor);
        ~UnorderedMap();
        Iterator begin() const;
        Iterator end() const;
        string& operator[](string const& key);
        void rehash();
        void remove(string const& key);
        unsigned int size();
        double loadFactor();
};

UnorderedMap::UnorderedMap(unsigned int bucketCount, double loadFactor) {
            this->bucketCount = bucketCount;
            umap.resize(bucketCount);
            numItems = 0;
            LF = loadFactor;
        }

UnorderedMap::~UnorderedMap() {
    for (int i = 0; i < bucketCount; i++) {
        if (umap[i] != nullptr) {
            delete umap[i];
        }
    }
}

unsigned int hashFunction(char const* key, int table_size) {
    unsigned int hashCode = 0;
    for (unsigned int i = 0; i < strlen(key); i++) {
        unsigned int b;
        if (i % 2 == 0) {
            b = (hashCode << 7) ^ int(key[i]) ^ (hashCode >> 3);
        }
        else {
            b = (hashCode << 11) ^ int(key[i]) ^ (hashCode >> 5);
            b = ~b;
        }
        hashCode = hashCode ^ b;
    }
    hashCode = hashCode & 0x7FFFFFFF; //214748647 
    return hashCode % table_size;
}

UnorderedMap::Iterator UnorderedMap::begin() const {
    unsigned int i;
    //Iterate through vector to check for first non-null linked list
    for (i = 0; i < bucketCount; i++) {
        if (umap[i] != nullptr) {
            //Invalid conversion from const LL node to LL node
            return Iterator(umap[i]->Head(), i, *this);
        }
    }
    //Otherwise, return nullptr and i = vector size
    return Iterator(nullptr, i, *this);
}

UnorderedMap::Iterator UnorderedMap::end() const {
    return Iterator(nullptr, bucketCount, *this);
}

string& UnorderedMap::operator[](string const& key) {
    //Convert key to hash function, get index
    unsigned int index = hashFunction(key.c_str(), bucketCount);
    //Check if list exists at index
    if (umap[index] != nullptr) {
        //if not, then iterate through list until node is found
        auto currentNode = umap[index]->Head();
        while (currentNode != nullptr) {
            if (currentNode->data.first == key)
                return currentNode->data.second;
            currentNode = currentNode->next;
        }
        //If key is not found, continue to creating a new value
    }
    else { //If list doesn't exist at index, create a list there
        umap[index] = new LinkedList<pair<string,string>>();
    }
    //Add the pair into the list. New item becomes new head, Rehash after insertion
    string s = "";
    pair<string,string> p = make_pair(key, s);
    umap[index]->AddHead(p);
    numItems++;
    rehash();
    index = hashFunction(key.c_str(), bucketCount);
    return (umap[index]->Head()->data.second);
}

void UnorderedMap::rehash() {
    //Comparing doubles: Sourced from https://stackoverflow.com/questions/17333/what-is-the-most-effective-way-for-float-and-double-comparison
    bool equalDouble= fabs((loadFactor() - LF)) < (__DBL_EPSILON__* fabs(loadFactor() + LF));
    if (equalDouble) {
        int n = numItems;
        //Create a new vector double the size of the new one
        vector<LinkedList<pair<string,string>>*> newVector;
        newVector.resize(bucketCount * 2);
        auto it = this->begin();
        while (it != this->end()) {
            //Insert item at new vector
            int i = hashFunction((*it).first.c_str(), bucketCount * 2);
            if (newVector[i] == nullptr) {
                newVector[i] = new LinkedList<pair<string,string>>();
            }
            newVector[i]->AddHead((*it));
            //Delete item at old vector
            auto old = it;
            ++it;
            remove((*old).first);
        }
        umap.clear();
        umap = newVector;
        bucketCount *= 2;
        numItems = n;
    }
}

void UnorderedMap::remove(string const& key) {
    unsigned int index = hashFunction(key.c_str(), bucketCount);
    auto currentNode = umap[index]->Head();
    while (currentNode != nullptr) {
        if (currentNode->data.first == key) {
            umap[index]->Remove(currentNode->data);
            numItems--;
            break;
        }
        currentNode = currentNode->next;
    }
    if (umap[index]->Head() == nullptr)
        umap[index] = nullptr;
}

unsigned int UnorderedMap::size() {
    return numItems;
}

double UnorderedMap::loadFactor() {
    return ((double)numItems)/bucketCount;
}

UnorderedMap::Iterator::Iterator(LinkedList<pair<string,string>>::Node* ListNodePtr, unsigned int i, const UnorderedMap& obj)
{
    vectorIndex = i;
    itemPtr = ListNodePtr;
    mapPtr = &obj;
}

std::pair<string, string> UnorderedMap::Iterator::operator*() const {
    return itemPtr->data;
}

UnorderedMap::Iterator& UnorderedMap::Iterator::operator++() {
    if (itemPtr->next != nullptr)
        itemPtr = itemPtr->next;
    else 
    {
        //Increment index to next non-null vector entry
        vectorIndex++;
        while (mapPtr->umap[vectorIndex] == nullptr && vectorIndex < mapPtr->bucketCount)
            vectorIndex++;
        //Condition to check for last value 
        if (vectorIndex >= mapPtr->bucketCount)
            itemPtr = nullptr;
        else
            itemPtr = mapPtr->umap[vectorIndex]->Head();
    }  
    return *this;
}

bool UnorderedMap::Iterator::operator!=(Iterator const& rhs) 
{
    if (itemPtr != rhs.itemPtr && vectorIndex != rhs.vectorIndex) {
        return true;
    }
    return false;
}

UnorderedMap::Iterator& UnorderedMap::Iterator::operator=(Iterator const& rhs) 
{
    itemPtr = rhs.itemPtr;
    vectorIndex = rhs.vectorIndex;
    return *this;
}

bool UnorderedMap::Iterator::operator==(Iterator const& rhs) {
if (itemPtr == rhs.itemPtr && vectorIndex == rhs.vectorIndex)
    return true;
return false;
}

/**
int main()
{
    int lines = 0, buckets = 0;
    double maxLoadFactor = 0.0;
    std::string command = "", ufid = "", name = "", key = "";
    std::cin >> lines >> buckets >> maxLoadFactor;
    UnorderedMap myMap = UnorderedMap(buckets, maxLoadFactor);
    while(lines--)
    {
        std::cin >> command;
        if(command == "hash")
        {
            std::cin >> key;
            const char* convertedKey = key.c_str();
            std::cout << hashFunction(convertedKey, buckets) << "\n";
        }
        else if(command == "insert") 
        {
            std::cin >> ufid >> name;
            myMap[ufid] = name;
        }
        else if(command == "size") 
        {
            std::cout << myMap.size() <<"\n";
        }
        else if(command == "load") 
        {
            std::cout << std::fixed << std::setprecision(2) << myMap.loadFactor() <<"\n";
        }
        else if(command == "search")
        {
            std::cin >> ufid;
            std::cout << myMap[ufid] << "\n";
        }
        else if(command == "traverse")
        {
            for (UnorderedMap::Iterator iter = myMap.begin(); iter != myMap.end(); ++iter) 
            {
                std::cout << (*iter).first << " " << (*iter).second << "\n";
            }

             /**This should also work
                for (auto tableEntry: myMap) 
                {
                    std::cout << tableEntry.first << " " << tableEntry.second << "\n";
                }
            **\/
        }
        else if(command == "remove")
        {
            std::cin >> ufid;
            myMap.remove(ufid);
        }  
    }

    return 0;
}
**/


