#include <iostream>
#include <functional>
#include <algorithm>
#include "cache_iterator.h"
#pragma comment(lib, "wininet")

struct display_cache_entry {
    void operator()(const INTERNET_CACHE_ENTRY_INFOA& cei) const {
        std::cout << cei.lpszSourceUrlName << std::endl;
    }
};

int main()
{
    cache_iterator cacheIter(NULL), cacheEnd;
    std::for_each(cacheIter, cacheEnd, display_cache_entry());
}