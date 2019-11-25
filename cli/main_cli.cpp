// Copyright (c) 2019 The CBigbang developers
// Distributed under the MIT/X11 software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include <stdio.h>
#include <string.h>
#include <time.h>
#include <string>
#include <algorithm>

#include "signlib.h"
#include "util.h"

static void PrintTx(Transaction *tx)
{
    printf("tx version is: %u\n", tx->version);

    printf("tx type is: %u\n", tx->type);

    printf("tx timestamp: %u\n", tx->timestamp);

    printf("lock util: %u\n", tx->lock_until);

    printf("tx hash anchor: %s\n", tx->hash_anchor);

    printf("tx address: %s\n", tx->address);

    printf("tx input size: %u\n", tx->size0);

    printf("tx vchData size1: %u\n", tx->size1);

    printf("tx prefix: %u\n", tx->prefix);

    printf("tx amount: %lu\n", tx->amount);

    printf("tx fee: %lu\n", tx->tx_fee);

    printf("tx signed size: %u\n", tx->size2);

    printf("tx sign: %s\n", tx->sign);
}

static void help()
{
    printf("sign-cli deserializetxwithoutsign <tx hex data>\n");
    printf("sign-cli deserializetxwithsign <tx hex data>\n");
}

int main(int argc, char *argv[])
{
    if (argc == 2 && strcmp(argv[1], "-help") == 0)
    {
        help();
        return 0;
    }

    if (argc == 3 && strcmp(argv[1], "deserializetxwithoutsign") == 0 && strlen(argv[2]) != 0)
    {
        Transaction tx_desrialized = {0};
        DeserializeTxWithoutSign(argv[2], &tx_desrialized);
        PrintTx(&tx_desrialized);
    }

    if (argc == 3 && strcmp(argv[1], "deserializetxwithsign") == 0 && strlen(argv[2]) != 0)
    {
        Transaction tx_desrialized = {0};
        DeserializeTxWithSign(argv[2], &tx_desrialized);
        PrintTx(&tx_desrialized);
    }

    return 0;
}