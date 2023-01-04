// Copyright (c) 2020 The PIVX developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or https://www.opensource.org/licenses/mit-license.php.

#ifndef NEBULAPROJECT_SAPLING_TEST_FIXTURE_H
#define NEBULAPROJECT_SAPLING_TEST_FIXTURE_H

#include "test/test_nebulaproject.h"

/**
 * Testing setup that configures a complete environment for Sapling testing.
 */
struct SaplingTestingSetup : public TestingSetup {
    SaplingTestingSetup();
    ~SaplingTestingSetup();
};


#endif //NEBULAPROJECT_SAPLING_TEST_FIXTURE_H