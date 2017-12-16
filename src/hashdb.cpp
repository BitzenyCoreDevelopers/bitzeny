#include "hashdb.h"

CHashDB *phashdb = nullptr;

CHashDB::CHashDB(size_t nCacheSize, bool fMemory, bool fWipe) : CDBWrapper(GetDataDir() / "hashes", nCacheSize, fMemory, fWipe) {
}

uint256 CHashDB::GetHash(const CBlockHeader &block) {
    uint256 hash;
    if (!this->Read(block, hash)) {
        hash = block.GetHash();
        this->Write(block, hash);
    }
    return hash;
}
