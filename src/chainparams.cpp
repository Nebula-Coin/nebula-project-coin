// Copyright (c) 2010 Satoshi Nakamoto
// Copyright (c) 2009-2015 The Bitcoin developers
// Copyright (c) 2014-2015 The Dash developers
// Copyright (c) 2015-2020 The PIVX developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include "chainparams.h"

#include "chainparamsseeds.h"
#include "consensus/merkle.h"
#include "tinyformat.h"
#include "utilstrencodings.h"

#include <assert.h>

static CBlock CreateGenesisBlock(const char* pszTimestamp, const CScript& genesisOutputScript, uint32_t nTime, uint32_t nNonce, uint32_t nBits, int32_t nVersion, const CAmount& genesisReward)
{
    CMutableTransaction txNew;
    txNew.nVersion = 1;
    txNew.vin.resize(1);
    txNew.vout.resize(1);
    txNew.vin[0].scriptSig = CScript() << 486604799 << CScriptNum(4) << std::vector<unsigned char>((const unsigned char*)pszTimestamp, (const unsigned char*)pszTimestamp + strlen(pszTimestamp));
    txNew.vout[0].nValue = genesisReward;
    txNew.vout[0].scriptPubKey = genesisOutputScript;

    CBlock genesis;
    genesis.vtx.push_back(std::make_shared<const CTransaction>(std::move(txNew)));
    genesis.hashPrevBlock.SetNull();
    genesis.nVersion = nVersion;
    genesis.nTime    = nTime;
    genesis.nBits    = nBits;
    genesis.nNonce   = nNonce;
    genesis.hashMerkleRoot = BlockMerkleRoot(genesis);
    return genesis;
}

void CChainParams::UpdateNetworkUpgradeParameters(Consensus::UpgradeIndex idx, int nActivationHeight)
{
    assert(IsRegTestNet()); // only available for regtest
    assert(idx > Consensus::BASE_NETWORK && idx < Consensus::MAX_NETWORK_UPGRADES);
    consensus.vUpgrades[idx].nActivationHeight = nActivationHeight;
}

/**
 * Build the genesis block. Note that the output of the genesis coinbase cannot
 * be spent as it did not originally exist in the database.
 *
 * CBlock(hash=00000ffd590b14, ver=1, hashPrevBlock=00000000000000, hashMerkleRoot=e0028e, nTime=1390095618, nBits=1e0ffff0, nNonce=28917698, vtx=1)
 *   CTransaction(hash=e0028e, ver=1, vin.size=1, vout.size=1, nLockTime=0)
 *     CTxIn(COutPoint(000000, -1), coinbase 04ffff001d01044c5957697265642030392f4a616e2f3230313420546865204772616e64204578706572696d656e7420476f6573204c6976653a204f76657273746f636b2e636f6d204973204e6f7720416363657074696e6720426974636f696e73)
 *     CTxOut(nValue=50.00000000, scriptPubKey=0xA9037BAC7050C479B121CF)
 *   vMerkleTree: e0028e
 */
static CBlock CreateGenesisBlock(uint32_t nTime, uint32_t nNonce, uint32_t nBits, int32_t nVersion, const CAmount& genesisReward)
{
    const char* pszTimestamp = "The Nebula Projects aims to solve problems with todays traditional markets and payment platforms.";
    const CScript genesisOutputScript = CScript() << ParseHex("04a2ed544a2a8aa4c763c6d0ecbafe4b44f92aae499d864a94a8564707fb3a3c9839f9be2b34ad248f36c751dda9282b63d00d1d090a91e3b14e2431eb527db4c8") << OP_CHECKSIG;
    return CreateGenesisBlock(pszTimestamp, genesisOutputScript, nTime, nNonce, nBits, nVersion, genesisReward);
}

/**
 * Main network
 */
/**
 * What makes a good checkpoint block?
 * + Is surrounded by blocks with reasonable timestamps
 *   (no blocks before with a timestamp after, none after with
 *    timestamp before)
 * + Contains no strange transactions
 */
static Checkpoints::MapCheckpoints mapCheckpoints = {
	{0, uint256S("0x001")},
};

static const Checkpoints::CCheckpointData data = {
    &mapCheckpoints,
    1591401645, // * UNIX timestamp of last checkpoint block
    5607713,    // * total number of transactions between genesis and last checkpoint
                //   (the tx=... number in the UpdateTip debug.log lines)
    3000        // * estimated number of transactions per day after checkpoint
};

static Checkpoints::MapCheckpoints mapCheckpointsTestnet = {
    {0, uint256S("0x001")},
    //{    201, uint256S("6ae7d52092fd918c8ac8d9b1334400387d3057997e6e927a88e57186dc395231")},     // v5 activation (PoS/Sapling)
};

static const Checkpoints::CCheckpointData dataTestnet = {
    &mapCheckpointsTestnet,
    1454124731,
    0,
    3000};

static Checkpoints::MapCheckpoints mapCheckpointsRegtest = {{0, uint256S("0x001")}};
static const Checkpoints::CCheckpointData dataRegtest = {
    &mapCheckpointsRegtest,
    1454124731,
    0,
    100};

class CMainParams : public CChainParams
{
public:
    CMainParams()
    {
        strNetworkID = "main";

        genesis = CreateGenesisBlock(1672725004, 1367557, 0x1e0ffff0, 1, 250 * COIN);
        consensus.hashGenesisBlock = genesis.GetHash();

        assert(consensus.hashGenesisBlock == uint256S("0x00000784a1da73a185e3e3160c9515007690dd8b560b7aae7e8fdf8ddc0974ab"));
        assert(genesis.hashMerkleRoot == uint256S("0xcd20e2816c1b185a9b6f369d51011b1186ff5b98f6f5bf657cd6039bf289fc29"));

        consensus.fPowAllowMinDifficultyBlocks = true;
        consensus.powLimit   = ~UINT256_ZERO >> 20;   // NEBULAPROJECT starting difficulty is 1 / 2^12
        consensus.posLimitV1 = ~UINT256_ZERO >> 24;
        consensus.posLimitV2 = ~UINT256_ZERO >> 20;
        consensus.nBudgetCycleBlocks = 44640;       // approx. 1 every 30 days
        consensus.nBudgetFeeConfirmations = 6;      // Number of confirmations for the finalization fee
        consensus.nCoinbaseMaturity = 100;
        consensus.nFutureTimeDriftPoW = 7200;
        consensus.nFutureTimeDriftPoS = 180;
        consensus.nMasternodeCountDrift = 20;       // num of MN we allow the see-saw payments to be off by
        consensus.nMaxMoneyOut = 21000000 * COIN;
        consensus.nPoolMaxTransactions = 3;
        consensus.nProposalEstablishmentTime = 60 * 60 * 24;    // must be at least a day old to make it into a budget
        consensus.nStakeMinAge = 60 * 60;
        consensus.nStakeMinDepth = 100;
        consensus.nTargetTimespan = 40 * 60;
        consensus.nTargetTimespanV2 = 30 * 60;
        consensus.nTargetSpacing = 1 * 60;
        consensus.nTimeSlotLength = 15;
        consensus.nMaxProposalPayments = 6;

        // spork keys
        consensus.strSporkPubKey = "048b1d35ecf63595495865e4a454bc45a3e3877524b38ec5f4134892d682bb55a85fe534a1dbade0db7bf1d7d971cf3ccd9ebc015f167e57e3ae4b9ee626a32174";
        consensus.strSporkPubKeyOld = "040F129DE6546FE405995329A887329BED4321325B1A73B0A257423C05C1FCFE9E40EF0678AEF59036A22C42E61DFD29DF7EFB09F56CC73CADF64E05741880E3E7";
        consensus.nTime_EnforceNewSporkKey = 1608512400;    //!> December 21, 2020 01:00:00 AM GMT
        consensus.nTime_RejectOldSporkKey = 1614560400;     //!> March 1, 2021 01:00:00 AM GMT

        // height-based activations
        consensus.height_last_invalid_UTXO = 999999999;
        consensus.height_last_ZC_AccumCheckpoint = 999999999;
        consensus.height_last_ZC_WrappedSerials = 999999999;
        consensus.height_ZC_RecalcAccumulators = 999999999;

        // validation by-pass
        consensus.nNebulaprojectBadBlockTime = 1471401614;    // Skip nBit validation of Block 259201 per PR #915
        consensus.nNebulaprojectBadBlockBits = 0x1c056dac;    // Skip nBit validation of Block 259201 per PR #915

        // Zerocoin-related params
        consensus.ZC_Modulus = "25195908475657893494027183240048398571429282126204032027777137836043662020707595556264018525880784"
                "4069182906412495150821892985591491761845028084891200728449926873928072877767359714183472702618963750149718246911"
                "6507761337985909570009733045974880842840179742910064245869181719511874612151517265463228221686998754918242243363"
                "7259085141865462043576798423387184774447920739934236584823824281198163815010674810451660377306056201619676256133"
                "8441436038339044149526344321901146575444541784240209246165157233507787077498171257724679629263863563732899121548"
                "31438167899885040445364023527381951378636564391212010397122822120720357";
        consensus.ZC_MaxPublicSpendsPerTx = 637;    // Assume about 220 bytes each input
        consensus.ZC_MaxSpendsPerTx = 7;            // Assume about 20kb each input
        consensus.ZC_MinMintConfirmations = 20;
        consensus.ZC_MinMintFee = 1 * CENT;
        consensus.ZC_MinStakeDepth = 200;
        consensus.ZC_TimeStart = 1508214600;        // October 17, 2017 4:30:00 AM

        // Network upgrades
        consensus.vUpgrades[Consensus::BASE_NETWORK].nActivationHeight =
                Consensus::NetworkUpgrade::ALWAYS_ACTIVE;
        consensus.vUpgrades[Consensus::UPGRADE_TESTDUMMY].nActivationHeight =
                Consensus::NetworkUpgrade::NO_ACTIVATION_HEIGHT;
        consensus.vUpgrades[Consensus::UPGRADE_POS].nActivationHeight           = 1602;
        consensus.vUpgrades[Consensus::UPGRADE_POS_V2].nActivationHeight        = 1702;
        consensus.vUpgrades[Consensus::UPGRADE_ZC].nActivationHeight            = 999999999;
        consensus.vUpgrades[Consensus::UPGRADE_ZC_V2].nActivationHeight         = 999999999;
        consensus.vUpgrades[Consensus::UPGRADE_BIP65].nActivationHeight         = 1602;
        consensus.vUpgrades[Consensus::UPGRADE_ZC_PUBLIC].nActivationHeight     = 999999999;
        consensus.vUpgrades[Consensus::UPGRADE_V3_4].nActivationHeight          = 3000;
        consensus.vUpgrades[Consensus::UPGRADE_V4_0].nActivationHeight          = 3500;
        consensus.vUpgrades[Consensus::UPGRADE_V5_0].nActivationHeight          = 4000;

		/**
        consensus.vUpgrades[Consensus::UPGRADE_ZC].hashActivationBlock =
                uint256S("0x5b2482eca24caf2a46bb22e0545db7b7037282733faa3a42ec20542509999a64");
        consensus.vUpgrades[Consensus::UPGRADE_ZC_V2].hashActivationBlock =
                uint256S("0x37ea75fe1c9314171cff429a91b25b9f11331076d1c9de50ee4054d61877f8af");
        consensus.vUpgrades[Consensus::UPGRADE_BIP65].hashActivationBlock =
                uint256S("0x82629b7a9978f5c7ea3f70a12db92633a7d2e436711500db28b97efd48b1e527");
        consensus.vUpgrades[Consensus::UPGRADE_ZC_PUBLIC].hashActivationBlock =
                uint256S("0xe2448b76d88d37aba4194ffed1041b680d779919157ddf5cbf423373d7f8078e");
        consensus.vUpgrades[Consensus::UPGRADE_V3_4].hashActivationBlock =
                uint256S("0x0ef2556e40f3b9f6e02ce611b832e0bbfe7734a8ea751c7b555310ee49b61456");
        consensus.vUpgrades[Consensus::UPGRADE_V4_0].hashActivationBlock =
                uint256S("0x14e477e597d24549cac5e59d97d32155e6ec2861c1003b42d0566f9bf39b65d5");
		**/
        /**
         * The message start string is designed to be unlikely to occur in normal data.
         * The characters are rarely used upper ASCII, not valid as UTF-8, and produce
         * a large 4-byte int at any alignment.
         */
        pchMessageStart[0] = 0xda;
        pchMessageStart[1] = 0xba;
        pchMessageStart[2] = 0xdf;
        pchMessageStart[3] = 0xe7;
        nDefaultPort = 1818;

        // Note that of those with the service bits flag, most only support a subset of possible options
        vSeeds.emplace_back("nebulaproject.io", "seed1.nebulaproject.io", true);     // Primary DNS Seeder from Fuzzbawls
        vSeeds.emplace_back("nebulaproject.io", "seed2.nebulaproject.io", true);    // Secondary DNS Seeder from Fuzzbawls

        base58Prefixes[PUBKEY_ADDRESS] = std::vector<unsigned char>(1, 61);
        base58Prefixes[SCRIPT_ADDRESS] = std::vector<unsigned char>(1, 57);
        base58Prefixes[STAKING_ADDRESS] = std::vector<unsigned char>(1, 80);     // starting with 'S'
        base58Prefixes[SECRET_KEY] = std::vector<unsigned char>(1, 75);
        base58Prefixes[EXT_PUBLIC_KEY] = {0x33, 0x93, 0x71, 0x73};
        base58Prefixes[EXT_SECRET_KEY] = {0xab, 0xc9, 0x75, 0xbf};
        // BIP44 coin type is from https://github.com/satoshilabs/slips/blob/master/slip-0044.md
        base58Prefixes[EXT_COIN_TYPE] = {0x80, 0x00, 0x03, 0xa1};

        vFixedSeeds = std::vector<SeedSpec6>(pnSeed6_main, pnSeed6_main + ARRAYLEN(pnSeed6_main));

        // Sapling
        bech32HRPs[SAPLING_PAYMENT_ADDRESS]      = "ps";
        bech32HRPs[SAPLING_FULL_VIEWING_KEY]     = "pviews";
        bech32HRPs[SAPLING_INCOMING_VIEWING_KEY] = "nblaks";
        bech32HRPs[SAPLING_EXTENDED_SPEND_KEY]   = "p-secret-spending-key-main";
        bech32HRPs[SAPLING_EXTENDED_FVK]         = "pxviews";
    }

    const Checkpoints::CCheckpointData& Checkpoints() const
    {
        return data;
    }

};

/**
 * Testnet (v5)
 */
class CTestNetParams : public CChainParams
{
public:
    CTestNetParams()
    {
        strNetworkID = "test";

        genesis = CreateGenesisBlock(1672725009, 107973, 0x1e0ffff0, 1, 250 * COIN);
        consensus.hashGenesisBlock = genesis.GetHash();

        assert(consensus.hashGenesisBlock == uint256S("0x000008ed80350453d7443f94ead4f4a513466a22d9e52c25232ff21c4a8942df"));
        assert(genesis.hashMerkleRoot == uint256S("0xcd20e2816c1b185a9b6f369d51011b1186ff5b98f6f5bf657cd6039bf289fc29"));
		
        consensus.fPowAllowMinDifficultyBlocks = true;
        consensus.powLimit   = ~UINT256_ZERO >> 20;   // NEBULAPROJECT starting difficulty is 1 / 2^12
        consensus.posLimitV1 = ~UINT256_ZERO >> 24;
        consensus.posLimitV2 = ~UINT256_ZERO >> 20;
        consensus.nBudgetCycleBlocks = 144;         // approx 10 cycles per day
        consensus.nBudgetFeeConfirmations = 3;      // (only 8-blocks window for finalization on testnet)
        consensus.nCoinbaseMaturity = 15;
        consensus.nFutureTimeDriftPoW = 7200;
        consensus.nFutureTimeDriftPoS = 180;
        consensus.nMasternodeCountDrift = 20;       // num of MN we allow the see-saw payments to be off by
        consensus.nMaxMoneyOut = 21000000 * COIN;
        consensus.nPoolMaxTransactions = 3;
        consensus.nProposalEstablishmentTime = 60 * 5;  // at least 5 min old to make it into a budget
        consensus.nStakeMinAge = 60 * 60;
        consensus.nStakeMinDepth = 100;
        consensus.nTargetTimespan = 40 * 60;
        consensus.nTargetTimespanV2 = 30 * 60;
        consensus.nTargetSpacing = 1 * 60;
        consensus.nTimeSlotLength = 15;
        consensus.nMaxProposalPayments = 20;

        // spork keys
        consensus.strSporkPubKey = "04d852f3404130fc13e3c65106a066789b387f9619f0dc4b160d79451ce0e76065228ff3f9e418b4106cdda6a51d3561655436938404e27ee89de631d937217191";
        consensus.strSporkPubKeyOld = "04E88BB455E2A04E65FCC41D88CD367E9CCE1F5A409BE94D8C2B4B35D223DED9C8E2F4E061349BA3A38839282508066B6DC4DB72DD432AC4067991E6BF20176127";
        consensus.nTime_EnforceNewSporkKey = 1608512400;    //!> December 21, 2020 01:00:00 AM GMT
        consensus.nTime_RejectOldSporkKey = 1614560400;     //!> March 1, 2021 01:00:00 AM GMT

        // height based activations
        consensus.height_last_invalid_UTXO = -1;
        consensus.height_last_ZC_AccumCheckpoint = -1;
        consensus.height_last_ZC_WrappedSerials = -1;
        consensus.height_ZC_RecalcAccumulators = 999999999;

        // Zerocoin-related params
        consensus.ZC_Modulus = "25195908475657893494027183240048398571429282126204032027777137836043662020707595556264018525880784"
                "4069182906412495150821892985591491761845028084891200728449926873928072877767359714183472702618963750149718246911"
                "6507761337985909570009733045974880842840179742910064245869181719511874612151517265463228221686998754918242243363"
                "7259085141865462043576798423387184774447920739934236584823824281198163815010674810451660377306056201619676256133"
                "8441436038339044149526344321901146575444541784240209246165157233507787077498171257724679629263863563732899121548"
                "31438167899885040445364023527381951378636564391212010397122822120720357";
        consensus.ZC_MaxPublicSpendsPerTx = 637;    // Assume about 220 bytes each input
        consensus.ZC_MaxSpendsPerTx = 7;            // Assume about 20kb each input
        consensus.ZC_MinMintConfirmations = 20;
        consensus.ZC_MinMintFee = 1 * CENT;
        consensus.ZC_MinStakeDepth = 200;
        consensus.ZC_TimeStart = 1508214600;        // October 17, 2017 4:30:00 AM

        // Network upgrades
        consensus.vUpgrades[Consensus::BASE_NETWORK].nActivationHeight =
                Consensus::NetworkUpgrade::ALWAYS_ACTIVE;
        consensus.vUpgrades[Consensus::UPGRADE_TESTDUMMY].nActivationHeight =
                Consensus::NetworkUpgrade::NO_ACTIVATION_HEIGHT;
        consensus.vUpgrades[Consensus::UPGRADE_POS].nActivationHeight           = 201;
        consensus.vUpgrades[Consensus::UPGRADE_POS_V2].nActivationHeight        = 201;
        consensus.vUpgrades[Consensus::UPGRADE_ZC].nActivationHeight            = 201;
        consensus.vUpgrades[Consensus::UPGRADE_ZC_V2].nActivationHeight         = 201;
        consensus.vUpgrades[Consensus::UPGRADE_BIP65].nActivationHeight         = 201;
        consensus.vUpgrades[Consensus::UPGRADE_ZC_PUBLIC].nActivationHeight     = 201;
        consensus.vUpgrades[Consensus::UPGRADE_V3_4].nActivationHeight          = 201;
        consensus.vUpgrades[Consensus::UPGRADE_V4_0].nActivationHeight          = 201;
        consensus.vUpgrades[Consensus::UPGRADE_V5_0].nActivationHeight          = 201;

        /**
         * The message start string is designed to be unlikely to occur in normal data.
         * The characters are rarely used upper ASCII, not valid as UTF-8, and produce
         * a large 4-byte int at any alignment.
         */
        pchMessageStart[0] = 0xe0;
        pchMessageStart[1] = 0xb9;
        pchMessageStart[2] = 0xa6;
        pchMessageStart[3] = 0xd4;
        nDefaultPort = 1918;

        // nodes with support for servicebits filtering should be at the top
        vSeeds.emplace_back("nebulaproject.io", "nebulaproject-testnet.seed.nebulaproject.io", true);
        vSeeds.emplace_back("nebulaproject.io", "nebulaproject-testnet.seed2.nebulaproject.io", true);

        base58Prefixes[PUBKEY_ADDRESS] = std::vector<unsigned char>(1, 66); // Testnet nebulaproject addresses start with 'x' or 'y'
        base58Prefixes[SCRIPT_ADDRESS] = std::vector<unsigned char>(1, 37);  // Testnet nebulaproject script addresses start with '8' or '9'
        base58Prefixes[STAKING_ADDRESS] = std::vector<unsigned char>(1, 58);     // starting with 'W'
        base58Prefixes[SECRET_KEY] = std::vector<unsigned char>(1, 71);     // Testnet private keys start with '9' or 'c' (Bitcoin defaults)
        // Testnet nebulaproject BIP32 pubkeys start with 'DRKV'
        base58Prefixes[EXT_PUBLIC_KEY] = {0x7d, 0xb8, 0x5e, 0x7a};
        // Testnet nebulaproject BIP32 prvkeys start with 'DRKP'
        base58Prefixes[EXT_SECRET_KEY] = {0xba, 0x79, 0xbd, 0xa1};
        // Testnet nebulaproject BIP44 coin type is '1' (All coin's testnet default)
        base58Prefixes[EXT_COIN_TYPE] = {0x80, 0x00, 0x00, 0x01};

        vFixedSeeds = std::vector<SeedSpec6>(pnSeed6_test, pnSeed6_test + ARRAYLEN(pnSeed6_test));

        // Sapling
        bech32HRPs[SAPLING_PAYMENT_ADDRESS]      = "ptestsapling";
        bech32HRPs[SAPLING_FULL_VIEWING_KEY]     = "pviewtestsapling";
        bech32HRPs[SAPLING_INCOMING_VIEWING_KEY] = "nblaktestsapling";
        bech32HRPs[SAPLING_EXTENDED_SPEND_KEY]   = "p-secret-spending-key-test";
        bech32HRPs[SAPLING_EXTENDED_FVK]         = "pxviewtestsapling";
    }

    const Checkpoints::CCheckpointData& Checkpoints() const
    {
        return dataTestnet;
    }
};

/**
 * Regression test
 */
class CRegTestParams : public CChainParams
{
public:
    CRegTestParams()
    {
        strNetworkID = "regtest";

        genesis = CreateGenesisBlock(1672725014, 1017993, 0x1e0ffff0, 1, 250 * COIN);
        consensus.hashGenesisBlock = genesis.GetHash();

        assert(consensus.hashGenesisBlock == uint256S("0x000000503d43320b06fcbe20d8314cc871b20496df24a317a7028920cbd9c4df"));
        assert(genesis.hashMerkleRoot == uint256S("0xcd20e2816c1b185a9b6f369d51011b1186ff5b98f6f5bf657cd6039bf289fc29"));

        consensus.fPowAllowMinDifficultyBlocks = true;
        consensus.powLimit   = ~UINT256_ZERO >> 20;   // NEBULAPROJECT starting difficulty is 1 / 2^12
        consensus.posLimitV1 = ~UINT256_ZERO >> 24;
        consensus.posLimitV2 = ~UINT256_ZERO >> 20;
        consensus.nBudgetCycleBlocks = 144;         // approx 10 cycles per day
        consensus.nBudgetFeeConfirmations = 3;      // (only 8-blocks window for finalization on regtest)
        consensus.nCoinbaseMaturity = 100;
        consensus.nFutureTimeDriftPoW = 7200;
        consensus.nFutureTimeDriftPoS = 180;
        consensus.nMasternodeCountDrift = 4;        // num of MN we allow the see-saw payments to be off by
        consensus.nMaxMoneyOut = 43199500 * COIN;
        consensus.nPoolMaxTransactions = 2;
        consensus.nProposalEstablishmentTime = 60 * 5;  // at least 5 min old to make it into a budget
        consensus.nStakeMinAge = 0;
        consensus.nStakeMinDepth = 2;
        consensus.nTargetTimespan = 40 * 60;
        consensus.nTargetTimespanV2 = 30 * 60;
        consensus.nTargetSpacing = 1 * 60;
        consensus.nTimeSlotLength = 15;
        consensus.nMaxProposalPayments = 20;

        /* Spork Key for RegTest:
        WIF private key: 932HEevBSujW2ud7RfB1YF91AFygbBRQj3de3LyaCRqNzKKgWXi
        private key hex: bd4960dcbd9e7f2223f24e7164ecb6f1fe96fc3a416f5d3a830ba5720c84b8ca
        Address: yCvUVd72w7xpimf981m114FSFbmAmne7j9
        */
        consensus.strSporkPubKey = "04e43206636ec87586aa804d4a5603a21910473618c3df26a0933f7ba6ebaa4cdf5fe8140103a7de2ecf4a2f801d23bf1d4ace5e9d286985608e9ccc44417d1d30";
        consensus.strSporkPubKeyOld = "";
        consensus.nTime_EnforceNewSporkKey = 0;
        consensus.nTime_RejectOldSporkKey = 0;

        // height based activations
        consensus.height_last_invalid_UTXO = -1;
        consensus.height_last_ZC_AccumCheckpoint = 310;     // no checkpoints on regtest
        consensus.height_last_ZC_WrappedSerials = -1;
        consensus.height_ZC_RecalcAccumulators = 999999999;

        // Zerocoin-related params
        consensus.ZC_Modulus = "25195908475657893494027183240048398571429282126204032027777137836043662020707595556264018525880784"
                "4069182906412495150821892985591491761845028084891200728449926873928072877767359714183472702618963750149718246911"
                "6507761337985909570009733045974880842840179742910064245869181719511874612151517265463228221686998754918242243363"
                "7259085141865462043576798423387184774447920739934236584823824281198163815010674810451660377306056201619676256133"
                "8441436038339044149526344321901146575444541784240209246165157233507787077498171257724679629263863563732899121548"
                "31438167899885040445364023527381951378636564391212010397122822120720357";
        consensus.ZC_MaxPublicSpendsPerTx = 637;    // Assume about 220 bytes each input
        consensus.ZC_MaxSpendsPerTx = 7;            // Assume about 20kb each input
        consensus.ZC_MinMintConfirmations = 10;
        consensus.ZC_MinMintFee = 1 * CENT;
        consensus.ZC_MinStakeDepth = 10;
        consensus.ZC_TimeStart = 0;                 // not implemented on regtest

        // Network upgrades
        consensus.vUpgrades[Consensus::BASE_NETWORK].nActivationHeight =
                Consensus::NetworkUpgrade::ALWAYS_ACTIVE;
        consensus.vUpgrades[Consensus::UPGRADE_TESTDUMMY].nActivationHeight =
                Consensus::NetworkUpgrade::NO_ACTIVATION_HEIGHT;
        consensus.vUpgrades[Consensus::UPGRADE_POS].nActivationHeight           = 251;
        consensus.vUpgrades[Consensus::UPGRADE_POS_V2].nActivationHeight        = 251;
        consensus.vUpgrades[Consensus::UPGRADE_ZC].nActivationHeight            = 300;
        consensus.vUpgrades[Consensus::UPGRADE_ZC_V2].nActivationHeight         = 300;
        consensus.vUpgrades[Consensus::UPGRADE_BIP65].nActivationHeight         =
                Consensus::NetworkUpgrade::ALWAYS_ACTIVE;
        consensus.vUpgrades[Consensus::UPGRADE_ZC_PUBLIC].nActivationHeight     = 400;
        consensus.vUpgrades[Consensus::UPGRADE_V3_4].nActivationHeight          = 251;
        consensus.vUpgrades[Consensus::UPGRADE_V4_0].nActivationHeight          =
                Consensus::NetworkUpgrade::ALWAYS_ACTIVE;
        consensus.vUpgrades[Consensus::UPGRADE_V5_0].nActivationHeight          = 300;

        /**
         * The message start string is designed to be unlikely to occur in normal data.
         * The characters are rarely used upper ASCII, not valid as UTF-8, and produce
         * a large 4-byte int at any alignment.
         */
        pchMessageStart[0] = 0xae;
        pchMessageStart[1] = 0xd4;
        pchMessageStart[2] = 0xbb;
        pchMessageStart[3] = 0xd8;
        nDefaultPort = 2018;

        base58Prefixes[PUBKEY_ADDRESS] = std::vector<unsigned char>(1, 55); // Testnet nebulaproject addresses start with 'x' or 'y'
        base58Prefixes[SCRIPT_ADDRESS] = std::vector<unsigned char>(1, 68);  // Testnet nebulaproject script addresses start with '8' or '9'
        base58Prefixes[STAKING_ADDRESS] = std::vector<unsigned char>(1, 36);     // starting with 'W'
        base58Prefixes[SECRET_KEY] = std::vector<unsigned char>(1, 23);     // Testnet private keys start with '9' or 'c' (Bitcoin defaults)
        // Testnet nebulaproject BIP32 pubkeys start with 'DRKV'
        base58Prefixes[EXT_PUBLIC_KEY] = {0x83, 0x7f, 0x09, 0x1d};
        // Testnet nebulaproject BIP32 prvkeys start with 'DRKP'
        base58Prefixes[EXT_SECRET_KEY] = {0x93, 0xae, 0xa3, 0x2d};
        // Testnet nebulaproject BIP44 coin type is '1' (All coin's testnet default)
        base58Prefixes[EXT_COIN_TYPE] = {0x80, 0x00, 0x00, 0x01};

        // Sapling
        bech32HRPs[SAPLING_PAYMENT_ADDRESS]      = "ptestsapling";
        bech32HRPs[SAPLING_FULL_VIEWING_KEY]     = "pviewtestsapling";
        bech32HRPs[SAPLING_INCOMING_VIEWING_KEY] = "nblaktestsapling";
        bech32HRPs[SAPLING_EXTENDED_SPEND_KEY]   = "p-secret-spending-key-test";
        bech32HRPs[SAPLING_EXTENDED_FVK]         = "pxviewtestsapling";
    }

    const Checkpoints::CCheckpointData& Checkpoints() const
    {
        return dataRegtest;
    }
};

static std::unique_ptr<CChainParams> globalChainParams;

const CChainParams &Params()
{
    assert(globalChainParams);
    return *globalChainParams;
}

std::unique_ptr<CChainParams> CreateChainParams(const std::string& chain)
{
    if (chain == CBaseChainParams::MAIN)
        return std::unique_ptr<CChainParams>(new CMainParams());
    else if (chain == CBaseChainParams::TESTNET)
        return std::unique_ptr<CChainParams>(new CTestNetParams());
    else if (chain == CBaseChainParams::REGTEST)
        return std::unique_ptr<CChainParams>(new CRegTestParams());
    throw std::runtime_error(strprintf("%s: Unknown chain %s.", __func__, chain));
}

void SelectParams(const std::string& network)
{
    SelectBaseParams(network);
    globalChainParams = CreateChainParams(network);
}

void UpdateNetworkUpgradeParameters(Consensus::UpgradeIndex idx, int nActivationHeight)
{
    globalChainParams->UpdateNetworkUpgradeParameters(idx, nActivationHeight);
}
