
#include "msort.h"
#include <iostream>
#include <vector>
#include <functional>
#include <cstdlib>

using namespace std;
template <typename T, typename Comp>
bool verifySorted(const std::vector<T>& vec, const Comp& c);

void genRandIntVector(std::vector<int>& vec, size_t numRandElems);

struct test_comp {
    bool operator()(const int random1, int random2) 
    {   
        return (random1 > random2); 
    }
};

int main()
{
    std::vector<int> test;
    genRandIntVector(test, 10);
    cout << "original list: ";
    for (int i = 0; i < 10; i++){
        cout << test[i] << " ";
    }
    cout << endl;
    test_comp fun;
    mergeSort(test, std::greater<int>());
     for (int i = 0; i < 10; i++){
        cout << test[i] << " ";
    }
    cout << endl;
    return 0;
}


template <typename T, typename Comp>
bool verifySorted(const std::vector<T>& vec, const Comp& c)
{
    if(2U > vec.size()) return true;
    for(size_t i = 0; i < vec.size()-1; i++) {
        if(c(vec[i+1], vec[i])) return false;
    }
    return true;
}

void genRandIntVector(std::vector<int>& vec, size_t numRandElems)
{
    vec.clear();
    vec.resize(numRandElems);
    srand(time(0));
    for(size_t i = 0; i < numRandElems; i++)
    {
        vec[i] = rand() % 10000;
    }
}
