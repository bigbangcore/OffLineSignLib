// Copyright (c) 2019 The CBigbang developers
// Distributed under the MIT/X11 software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef UINT256_H
#define UINT256_H

#include <stdint.h>

typedef struct uint256_t
{
    uint32_t pn[8];
} uint256_t;

int uint256_set_hex(uint256_t *uint, char *psz);
void uint256_get_hex(uint256_t *data, char *psz);
int uint256_compare(uint256_t *data1, uint256_t *data2);

#endif