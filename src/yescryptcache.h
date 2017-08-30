#ifndef YESCRYPTCACHE_H
#define YESCRYPTCACHE_H

#include "uint256.h"
#include "leveldbwrapper.h"

class CBlockYescryptCache : public CLevelDBWrapper
{
public:
    CBlockYescryptCache(size_t nCacheSize, bool fMemory = false, bool fWipe = false);
private:
    CBlockYescryptCache(const CBlockYescryptCache&);
    void operator=(const CBlockYescryptCache&);
public:
    bool WriteHash(const uint256 &hash, const uint256 &yescryptHash);
    bool ReadHash(const uint256 &hash, uint256 &yescryptHash);
};

#endif // YESCRYPTCACHE_H
