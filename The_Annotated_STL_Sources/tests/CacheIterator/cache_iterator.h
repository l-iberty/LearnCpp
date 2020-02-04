#ifndef _CACHE_ITERATOR_
#define _CACHE_ITERATOR_
#define NOMINMAX
#include <Windows.h>
#include <WinInet.h>
#include <malloc.h>
#include <iterator>
#include <algorithm>

class cache_iterator :public std::iterator<
    std::input_iterator_tag, INTERNET_CACHE_ENTRY_INFOA> {
protected:
    HANDLE                          m_hCacheEntry;
    LPINTERNET_CACHE_ENTRY_INFOA    m_lpCacheEntryInfo;
    DWORD                           m_dwMaxBufSize;
    DWORD                           m_dwBufSize;
public:
    cache_iterator()
        : m_hCacheEntry(0), m_lpCacheEntryInfo(0), m_dwBufSize(), m_dwMaxBufSize() {}

    cache_iterator(LPCSTR lpszUrlSearchPattern)
        : m_hCacheEntry(0), m_lpCacheEntryInfo(0), m_dwMaxBufSize(1024), m_dwBufSize(1024)
    {
        allocate_buffer(m_dwBufSize);
        initialize(lpszUrlSearchPattern);
    }

    INTERNET_CACHE_ENTRY_INFOA& operator*() { return *m_lpCacheEntryInfo; }
    cache_iterator& operator++() {
        next();
        return *this;
    }
    cache_iterator operator++(int) {
        cache_iterator tmp = *this;
        ++*this;
        return tmp;
    }
    bool operator==(const cache_iterator& x) const { return m_hCacheEntry == x.m_hCacheEntry; }
    bool operator!=(const cache_iterator& x) const { return m_hCacheEntry != x.m_hCacheEntry; }

protected:
    void allocate_buffer(size_t size) {
        m_lpCacheEntryInfo = (LPINTERNET_CACHE_ENTRY_INFOA)malloc(size);
        if (m_lpCacheEntryInfo == NULL) throw;
    }
    void deallocate_buffer() {
        if (m_lpCacheEntryInfo != NULL) {
            free(m_lpCacheEntryInfo);
            m_lpCacheEntryInfo = NULL;
        }
    }
    void reallocate_buffer(size_t size) {
        deallocate_buffer();
        allocate_buffer(size);
    }
    void close_handle() {
        if (m_hCacheEntry != NULL) {
            FindCloseUrlCache(m_hCacheEntry);
            m_hCacheEntry = NULL;
        }
    }

    void initialize(LPCSTR lpszUrlSearchPattern) {
        m_hCacheEntry = FindFirstUrlCacheEntryA(
            lpszUrlSearchPattern, m_lpCacheEntryInfo, &m_dwBufSize);
        if (m_hCacheEntry == NULL) {
            if (ERROR_INSUFFICIENT_BUFFER == GetLastError()) {
                m_dwMaxBufSize = m_dwBufSize;
                reallocate_buffer(m_dwBufSize);
                m_hCacheEntry = FindFirstUrlCacheEntryA(
                    lpszUrlSearchPattern, m_lpCacheEntryInfo, &m_dwBufSize);
            } else {
                throw;
            }
        }
    }

    void next() {
        m_dwBufSize = std::max(m_dwBufSize, m_dwMaxBufSize);
        if (!FindNextUrlCacheEntryA(m_hCacheEntry, m_lpCacheEntryInfo, &m_dwBufSize)) {
            DWORD err = GetLastError();
            if (err == ERROR_INSUFFICIENT_BUFFER) {
                m_dwMaxBufSize = m_dwBufSize;
                reallocate_buffer(m_dwBufSize);
                FindNextUrlCacheEntryA(m_hCacheEntry, m_lpCacheEntryInfo, &m_dwBufSize);
            } else if (err == ERROR_NO_MORE_ITEMS) {
                deallocate_buffer();
                close_handle();
            } else {
                throw;
            }
        }
    }
};

#endif /* _CACHE_ITERATOR_ */
