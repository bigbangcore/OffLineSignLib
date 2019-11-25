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

int main(int argc, char *argv[])
{
    if (argc == 2 && strcmp(argv[1], "-help"))
    {
        printf("sign-cli deserializetxwithoutsign <tx hex data>\n");
        return 0;
    }
    return 0;
}