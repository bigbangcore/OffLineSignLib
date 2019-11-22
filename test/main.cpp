// Copyright (c) 2019 The CBigbang developers
// Distributed under the MIT/X11 software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include <stdio.h>
#include <string.h>
#include <time.h>
#include <malloc.h>
#include <string>
#include <algorithm>

#include "signlib.h"
#include "util.h"

const char *created_tx_hex_data =
    "010000004411d55d0000000069c07b268573a89eb2bf00a895d0ccd557b83af5490e15ca8d41dedc0000000001e9f3b5c8fe00417e1c06cd099af2417813b0163c5a701cc9bb4da8504710d55d00011915eb90d9f9d0d92233e33793d04c0a5f6384e65fdc2ea0ff098c5db28d910540420f000000000064000000000000000000";

const char *signed_tx_hex_data =
    "010000008d31d65d0000000069c07b268573a89eb2bf00a895d0ccd557b83af5490e15ca8d41dedc000000000191b5093377f21fc5a76435351504ce5eae7591380cc3502672fb23c2f230d65d00016f757a33cf3b4f83f2b37b2308090f949c6f3870d50ceb3e5aa59b3118c66d7240420f0000000000640000000000000000816f757a33cf3b4f83f2b37b2308090f949c6f3870d50ceb3e5aa59b3118c66d720100815a6d40702a7da0a810de9ba76091cf0f7df0b7b56b7a6ef280c9ff26c14fa178a313c5800bebda19cff9e745a346725838c9b5ecb388797bc04a21bca4a9077dc2140b805b6816ab2a35e692821b7904dcd8bbd52f14c7e5c095b1f20308";

const char *created_tx_hex_data1 =
    "010000005948d75d0000000069c07b268573a89eb2bf00a895d0ccd557b83af5490e15ca8d41dedc0000000002e563f10b18dc361305815da5b464ae6af0a39e5ef2dccf1a74e63b219781d65d00a43970696b5c1b39b0bf4bc0b68df5fb993213c367709a0b3cd9b42c8d31d65d000100815a6d40702a7da0a810de9ba76091cf0f7df0b7b56b7a6ef280c9ff26c14f40420f000000000064000000000000000000";

const char *signed_tx_hex_data1 =
    "010000005948d75d0000000069c07b268573a89eb2bf00a895d0ccd557b83af5490e15ca8d41dedc0000000002e563f10b18dc361305815da5b464ae6af0a39e5ef2dccf1a74e63b219781d65d00a43970696b5c1b39b0bf4bc0b68df5fb993213c367709a0b3cd9b42c8d31d65d000100815a6d40702a7da0a810de9ba76091cf0f7df0b7b56b7a6ef280c9ff26c14f40420f0000000000640000000000000000400d6c650009275f9fa4f64cbd4712e995f84f00f96d056b4610d983c8d0cbad8aefcf75dbfa3f9afaf4bd27e9062f96a2fbc8a98a4feb796bfde547dcb9836b0c";
int main()
{
    char pubkey[PUBKEY_HEX_LEN] = {0};
    char privkey[PRIVKEY_HEX_LEN] = {0};

    MakeKeyPair(pubkey, privkey);

    if (strlen(pubkey) != PUBKEY_HEX_LEN - 1 || strlen(pubkey) != strlen(privkey))
    {
        fprintf(stderr, "MakeKeyPair Test ERROR!\n");
        fprintf(stderr, "pkey: %s\nskey: %s\n", pubkey, privkey);
        return -1;
    }

    char pkeyaddr[PUBKEY_ADDRESS_LEN] = {0};
    GetPubKeyAddress("e8e3770e774d5ad84a8ea65ed08cc7c5c30b42e045623604d5c5c6be95afb4f9", pkeyaddr);

    if (strcmp(pkeyaddr, "1z6taz5dyrv2xa11pc92y0ggbrf2wf36gbtk8wjprb96qe3kqwfm3ayc1") != 0)
    {
        fprintf(stderr, "GetPubKeyAddress Test ERROR!\n");
        fprintf(stderr, "PubKeyAddress: %s\n", pkeyaddr);
        return -1;
    }

    if (!IsValidAddress(pkeyaddr))
    {
        fprintf(stderr, "IsValidAddress Test ERROR!\n");
        fprintf(stderr, "Error Address: %s\n", pkeyaddr);
        return -1;
    }

    printf("----------------------- no signed tx test ----------------------\n");

    Transaction tx_desrialized = {0};

    DeserializeTxWithoutSign(created_tx_hex_data, &tx_desrialized);

    printf("tx version is: %u\n", tx_desrialized.version);

    printf("tx type is: %u\n", tx_desrialized.type);

    printf("tx timestamp: %u\n", tx_desrialized.timestamp);

    printf("lock util: %u\n", tx_desrialized.lock_until);

    printf("tx hash anchor: %s\n", tx_desrialized.hash_anchor);

    printf("tx address: %s\n", tx_desrialized.address);

    printf("tx size0: %u\n", tx_desrialized.size0);

    printf("tx size1: %u\n", tx_desrialized.size1);

    printf("tx prefix: %u\n", tx_desrialized.prefix);

    printf("tx amount: %lu\n", tx_desrialized.amount);

    printf("tx fee: %lu\n", tx_desrialized.tx_fee);

    printf("tx size2: %u\n", tx_desrialized.size2);

    printf("tx sign: %s\n", tx_desrialized.sign);

    char hexxx[1024 * 5] = {0};
    SerializeTxWithoutSign(&tx_desrialized, hexxx);

    if (strcmp(hexxx, created_tx_hex_data) != 0)
    {
        fprintf(stderr, "SerializeTxWithoutSign TEST error\n");
        fprintf(stderr, "SerializeTxWithoutSign hex: %s\n", hexxx);
        return -1;
    }

    printf("------------------- signed tx test-------------------\n");

    Transaction tx_signed_deserialized = {0};
    DeserializeTxWithSign(signed_tx_hex_data, &tx_signed_deserialized);

    printf("tx version is: %u\n", tx_signed_deserialized.version);

    printf("tx type is: %u\n", tx_signed_deserialized.type);

    printf("tx timestamp: %u\n", tx_signed_deserialized.timestamp);

    printf("lock util: %u\n", tx_signed_deserialized.lock_until);

    printf("tx hash anchor: %s\n", tx_signed_deserialized.hash_anchor);

    printf("tx address: %s\n", tx_signed_deserialized.address);

    printf("tx size0: %u\n", tx_signed_deserialized.size0);

    printf("tx size1: %u\n", tx_signed_deserialized.size1);

    printf("tx prefix: %u\n", tx_signed_deserialized.prefix);

    printf("tx amount: %lu\n", tx_signed_deserialized.amount);

    printf("tx fee: %lu\n", tx_signed_deserialized.tx_fee);

    printf("tx size2: %u\n", tx_signed_deserialized.size2);

    printf("tx sign: %s\n", tx_signed_deserialized.sign);

    char hexSignedTx[1024 * 5] = {0};
    SerializeTxWithSign(&tx_signed_deserialized, hexSignedTx);

    if (strcmp(hexSignedTx, signed_tx_hex_data) != 0)
    {
        fprintf(stderr, "SerializeTxWithSign TEST error!\n");
        fprintf(stderr, "SerializeTxWithSign Hex: %s\n", hexSignedTx);
        return -1;
    }

    printf("SerializeTxWithSign Hex: %s\n", hexSignedTx);

    Transaction tx_desrialized1 = {0};
    DeserializeTxWithoutSign(created_tx_hex_data1, &tx_desrialized1);

    SignTransation(&tx_desrialized1, "3a7a45f05643fa2e7eeb11da2e2c66e43ddf4f7535dccbb3e6c07fb39201b1df");

    char signedDataHex[1024 * 5] = {0};
    SerializeTxWithSign(&tx_desrialized1, signedDataHex);

    if (strcmp(signedDataHex, signed_tx_hex_data1) != 0)
    {
        fprintf(stderr, "SerializeTxWithSign TEST error!\n");
        fprintf(stderr, "SerializeTxWithSign Hex: %s\n", signedDataHex);
        return -1;
    }

    printf("SerializeTxWithSign Hex: %s\n", signedDataHex);

    return 0;
}
