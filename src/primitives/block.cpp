// Copyright (c) 2009-2010 Satoshi Nakamoto
// Copyright (c) 2009-2016 The Bitcoin Core developers
// Copyright (c) 2017 The Placeholder Core developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include "primitives/block.h"

#include "hash.h"
#include "tinyformat.h"
#include "utilstrencodings.h"
#include "crypto/common.h"

static const uint32_t MAINNET_X16RV2ACTIVATIONTIME = 1885833276;
static const uint32_t MAINNET_X16RV3ACTIVATIONTIME = 1885833276;

uint256 CBlockHeader::GetHash() const
{
	if (nTime >= MAINNET_X16RV2ACTIVATIONTIME ) {
        return HashX16RV2(BEGIN(nVersion), END(nNonce), hashPrevBlock);
    }
	
	if (nTime >= MAINNET_X16RV3ACTIVATIONTIME ) {
        return HashX16RV3(BEGIN(nVersion), END(nNonce), hashPrevBlock);
    }
	
    return HashX16R(BEGIN(nVersion), END(nNonce), hashPrevBlock);
}

uint256 CBlockHeader::GetX16RV3Hash() const
{
    return HashX16RV3(BEGIN(nVersion), END(nNonce), hashPrevBlock);
}

std::string CBlock::ToString() const
{
    std::stringstream s;
    s << strprintf("CBlock(hash=%s, ver=0x%08x, hashPrevBlock=%s, hashMerkleRoot=%s, nTime=%u, nBits=%08x, nNonce=%u, vtx=%u)\n",
        GetHash().ToString(),
        nVersion,
        hashPrevBlock.ToString(),
        hashMerkleRoot.ToString(),
        nTime, nBits, nNonce,
        vtx.size());
    for (const auto& tx : vtx) {
        s << "  " << tx->ToString() << "\n";
    }
    return s.str();
}
