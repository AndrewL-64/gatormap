#include "hashmap.h"
#include "treemap.h"
#include <chrono>
#include <iostream>
#include <random>
#include <iomanip>

typedef chrono::high_resolution_clock Clock;

void TestUnorderedMapInsert(UnorderedMap& umap, unsigned int x);
void TestOrderedMapInsert(OrderedMap& omap, unsigned int x, unsigned int& count);
void TestUnorderedMapSearch(UnorderedMap& umap, unsigned int x);
void TestOrderedMapSearch(OrderedMap& omap, unsigned int x, unsigned int count);
void TestUnorderedMapTraverse(UnorderedMap& umap, unsigned int x);
void TestOrderedMapTraverse(OrderedMap& omap, unsigned int x, unsigned int count);

int main() {
    UnorderedMap umap(100,0.8);
    UnorderedMap umap2(100,0.8);
    UnorderedMap umap3(100,0.8);
    
    OrderedMap omap;
    OrderedMap omap2;
    OrderedMap omap3;
    unsigned int count = 0, count2 = 0, count3 = 0;

    
    std::cout <<"Unordered Map:" << std::endl;
    std::cout <<"Insertion:" << std::endl;
    TestUnorderedMapInsert(umap, 100);
    TestUnorderedMapInsert(umap2, 10000);
    TestUnorderedMapInsert(umap3, 1000000);

    std::cout << "Traverse:" << std::endl;
    TestUnorderedMapTraverse(umap, 100);
    TestUnorderedMapTraverse(umap2, 10000);
    TestUnorderedMapTraverse(umap3, 1000000);

    std::cout << "Search:" << std::endl;
    TestUnorderedMapSearch(umap, 100);
    TestUnorderedMapSearch(umap2, 10000);
    TestUnorderedMapSearch(umap3, 1000000);

    std::cout <<"\nOrdered Map:" << std::endl;
    std::cout <<"Insertion:" << std::endl;
    TestOrderedMapInsert(omap, 100, count);
    TestOrderedMapInsert(omap2, 10000, count2);
    TestOrderedMapInsert(omap3, 1000000, count3);

    std::cout <<"Traverse:" << std::endl;
    TestOrderedMapTraverse(omap, 100, count);
    TestOrderedMapTraverse(omap2, 10000, count2);
    TestOrderedMapTraverse(omap3, 1000000, count3);

    std::cout <<"Search:" << std::endl;
    TestOrderedMapSearch(omap, 100, count);
    TestOrderedMapSearch(omap2, 10000, count2);
    TestOrderedMapSearch(omap3, 1000000, count3);

    

    std::cin.get(); //Added a pause to executable

    return 0;
}

void TestUnorderedMapInsert(UnorderedMap& umap, unsigned int x) {
    //Inserting x items
    auto t1 = Clock::now();
    for (unsigned int i = 0; i < x; i++) {
        string ID = "";
        while (ID.length() < 8) {
            ID += (char)(rand() % 10 + '0');
        }
        string name = "Test#" + to_string(i + 1);
        umap[ID] = name;
    }
    auto t2 = Clock::now();
    ostringstream output;
    output << "Duration for " << x << " insertions: " 
        << std::chrono::duration_cast<std::chrono::microseconds>(t2 - t1).count()
        << " µs";
    std::cout << std::left << std::setw(50) << output.str() << "Size of Table: "<< umap.size() << std::endl;
}

void TestOrderedMapInsert(OrderedMap& omap, unsigned int x, unsigned int& count) {
    //Inserting x items
    auto t1 = Clock::now();
    for (unsigned int i = 0; i < x; i++) {
        string ID = "";
        while (ID.length() < 8) {
            ID += (char)(rand() % 10 + '0');
        }
        string name = "Test#" + to_string(i + 1);
        bool inserted = omap.insert(ID, name);
        if (inserted)
            count++;
    }
    auto t2 = Clock::now();
    ostringstream output;
    output << "Duration for " << x << " insertions: " 
        << std::chrono::duration_cast<std::chrono::microseconds>(t2 - t1).count()
        << " µs";
    std::cout << std::left << std::setw(50) << output.str() << "Size of Table: "<< count << std::endl;
}

void TestUnorderedMapSearch(UnorderedMap& umap, unsigned int x) {
    //Searching x items
    auto t1 = Clock::now();
    for (unsigned int i = 0; i < x; i++) {
        string ID = "";
        while (ID.length() < 8) {
            ID += (char)(rand() % 10 + '0');
        }
        string result = umap[ID];
    }
    auto t2 = Clock::now();
    //Cleans up output
    ostringstream output;
    output << "Duration for " << x << " searches: " 
        << std::chrono::duration_cast<std::chrono::microseconds>(t2 - t1).count()
        << " µs";
    std::cout << std::left << std::setw(50) << output.str() << "Size of Table: "<< umap.size() << std::endl;
}

void TestOrderedMapSearch(OrderedMap& omap, unsigned int x, unsigned int count) {
    //Searching x items
    auto t1 = Clock::now();
    for (unsigned int i = 0; i < x; i++) {
        string ID = "";
        while (ID.length() < 8) {
            ID += (char)(rand() % 10 + '0');
        }
        string result = omap.search(ID);
    }
    auto t2 = Clock::now();
    ostringstream output;
    output << "Duration for " << x << " searches: " 
        << std::chrono::duration_cast<std::chrono::microseconds>(t2 - t1).count()
        << " µs";
    std::cout << std::left << std::setw(50) << output.str() << "Size of Table: "<< count << std::endl;
}

void TestUnorderedMapTraverse(UnorderedMap& umap, unsigned int x) {
    //Traversing x elements
    auto t1 = Clock::now();
    for (auto it : umap);
    auto t2 = Clock::now();
    ostringstream output;
    output << "Duration for traversing " << x << " items: " 
        << std::chrono::duration_cast<std::chrono::microseconds>(t2 - t1).count()
        << " µs";
    std::cout << std::left << std::setw(50) << output.str() << "Size of Table: "<< umap.size() << std::endl;
}

void TestOrderedMapTraverse(OrderedMap& omap, unsigned int x, unsigned int count) {
    auto t1 = Clock::now();
    omap.traverse();
    auto t2 = Clock::now();
    ostringstream output;
    output << "Duration for traversing " << x << " items: " 
        << std::chrono::duration_cast<std::chrono::microseconds>(t2 - t1).count()
        << " µs";
    std::cout << std::left << std::setw(50) << output.str() << "Size of Table: "<< count << std::endl;
}
