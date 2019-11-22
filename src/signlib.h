// Copyright (c) 2019 The CBigbang developers
// Distributed under the MIT/X11 software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef BIGBANG_SIGNLIB_H
#define BIGBANG_SIGNLIB_H

#include <stdint.h>

typedef int BOOL;

#define PUBKEY_HEX_LEN (32 * 2 + 1)
#define PRIVKEY_HEX_LEN (32 * 2 + 1)
#define PUBKEY_ADDRESS_LEN (57 + 1)
#define UINT256_HEX_LEN 65;

typedef struct _Transaction
{
    uint16_t version;
    uint16_t type;
    uint32_t timestamp;
    uint32_t lock_until;
    char hash_anchor[64 + 1];            // hex string
    unsigned char hash_anchor_bytes[32]; // binary data (caller do not care about this field, you just care hex field)
    uint8_t size0;
    unsigned char input[1024 * 10];
    uint8_t prefix;
    char address[64 + 1];            // hex string
    unsigned char address_bytes[32]; // binary data (caller do not care about this field, you just care hex field)
    int64_t amount;
    int64_t tx_fee;
    uint8_t size1;
    unsigned char vch_data[1024 * 10];  // binary (caller do not care about this field, you just care hex field)
    uint8_t size2;                      // binary sign data size
    char sign[1024];                    // hex string
    unsigned char sign_bytes[1024 * 5]; // binary (caller do not care about this field, you just care hex field)
} Transaction;

#ifdef __cplusplus
extern "C"
{
#endif

    /**
 * @brief  MakeKeyPair
 * Generate public key and private key 
 *
 *
 * @author chenshaohan
 * @email  brainfvck@foxmail.com
 * @param  char *           pubkey  - public key hex string (output)
 * @param  char *           privkey - private key hex string (output)
 * @return none
 */
    void MakeKeyPair(char *pubkey, char *privkey);

    /**
 * @brief  GetPubKeyAddress
 * Get Address hex string from public key hex string 
 *
 *
 * @author chenshaohan
 * @email  brainfvck@foxmail.com
 * @param  const char *     pubkey  - public key hex string (input)
 * @param  char *           address - address hex string (output)
 * @return none
 */
    void GetPubKeyAddress(const char *pubkey, char *address);

    /**
 * @brief  IsValidAddress
 * validate Address hex string is valid 
 *
 *
 * @author chenshaohan
 * @email  brainfvck@foxmail.com
 * @param  const char *           address - address hex string (input)
 * @return 0 false 1 true
 */
    BOOL IsValidAddress(const char *address);

    /**
 * @brief  SignTransaction
 * Sign Transaction
 *
 *
 * @author chenshaohan
 * @email  brainfvck@foxmail.com
 * @param  Transaction *         tx - Transaction struct you need to sign (input | output, sign[64] field of Transaction is output)
 * @param  const char *          secretHex - secretKey hex string (input)
 * @return none
 */
    void SignTransation(Transaction *tx, const char *secretHex);

    /**
 * @brief  SerializeTxWithSign
 * Serialize Transaction
 *
 *
 * @author chenshaohan
 * @email  brainfvck@foxmail.com
 * @param  Transaction *         tx - signed Transaction struct you need to serialize (input)
 * @param  char *                dataHex - data hex string serialized (output)
 * @return none
 */
    void SerializeTxWithSign(Transaction *tx, char *dataHex);

    /**
 * @brief  SerializeTxWithoutSign
 * Serialize Transaction
 *
 *
 * @author chenshaohan
 * @email  brainfvck@foxmail.com
 * @param  Transaction *         tx - no signed Transaction struct you need to serialize (input)
 * @param  char *                dataHex - data hex string serialized (output)
 * @return none
 */
    void SerializeTxWithoutSign(Transaction *tx, char *dataHex);

    /**
 * @brief  DeserializeTxWithSign
 * Deserialize Transaction
 *
 *
 * @author chenshaohan
 * @email  brainfvck@foxmail.com
 * @param  const char *          dataHex -  signed stransaction hex data string you need to deserialized (input)
 * @param  Transaction *         tx - Transaction struct deserialized (output)
 * @return none
 */
    void DeserializeTxWithSign(const char *dataHex, Transaction *tx);

    /**
 * @brief  DeserializeTxWithoutSign
 * Deserialize Transaction
 *
 *
 * @author chenshaohan
 * @email  brainfvck@foxmail.com
 * @param  const char *          dataHex -  no signed stransaction hex data string you need to deserialized (input)
 * @param  Transaction *         tx - Transaction struct deserialized (output)
 * @return none
 */
    void DeserializeTxWithoutSign(const char *dataHex, Transaction *tx);

#ifdef __cplusplus
}
#endif

#endif