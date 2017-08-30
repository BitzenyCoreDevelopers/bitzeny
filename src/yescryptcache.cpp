#include "yescryptcache.h"

CBlockYescryptCache *yescryptCache = NULL;

CBlockYescryptCache::CBlockYescryptCache(size_t nCacheSize, bool fMemory, bool fWipe) : CLevelDBWrapper(GetDataDir() / "blocks" / "cache", nCacheSize, fMemory, fWipe) {
}

bool CBlockYescryptCache::WriteHash(const uint256 &hash, const uint256 &yescryptHash)
{
    return Write(std::make_pair('y', hash), yescryptHash);
}

bool CBlockYescryptCache::ReadHash(const uint256 &hash, uint256 &yescryptHash)
{
    return Read(std::make_pair('y', hash), yescryptHash);
}
