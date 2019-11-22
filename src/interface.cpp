#include "interface.h"
#include "jni/com_bigbang_BigBangCore.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sodium.h>

#include "uint256.h"
#include "util.h"

static size_t serialize_join(size_t *size, void *thing, size_t size_thing, unsigned char *data)
{
    memcpy(data + *size, thing, size_thing);
    *size += size_thing;
    return *size;
}

static size_t deserialize_join(size_t *size, unsigned char *data, void *thing, size_t size_thing)
{
    memcpy(thing, data + *size, size_thing);
    *size += size_thing;

    return *size;
}

static size_t tx_serialize_without_sign(Transaction *tx, unsigned char *data)
{
    size_t size = 0;
    size_t size_thing = sizeof(tx->version);
    serialize_join(&size, &tx->version, size_thing, data);

    size_thing = sizeof(tx->type);
    serialize_join(&size, &tx->type, size_thing, data);

    size_thing = sizeof(tx->timestamp);
    serialize_join(&size, &tx->timestamp, size_thing, data);

    size_thing = sizeof(tx->lock_until);
    serialize_join(&size, &tx->lock_until, size_thing, data);

    size_thing = sizeof(tx->hash_anchor_bytes);
    serialize_join(&size, tx->hash_anchor_bytes, size_thing, data);

    size_thing = sizeof(tx->size0);
    serialize_join(&size, &tx->size0, size_thing, data);

    size_thing = (sizeof(unsigned char) * (32 + 1)) * tx->size0;
    serialize_join(&size, tx->input, size_thing, data);

    size_thing = sizeof(tx->prefix);
    serialize_join(&size, &tx->prefix, size_thing, data);

    size_thing = sizeof(tx->address_bytes);
    serialize_join(&size, tx->address_bytes, size_thing, data);

    size_thing = sizeof(tx->amount);
    serialize_join(&size, &tx->amount, size_thing, data);

    size_thing = sizeof(tx->tx_fee);
    serialize_join(&size, &tx->tx_fee, size_thing, data);

    size_thing = sizeof(tx->size1);
    serialize_join(&size, &tx->size1, size_thing, data);

    size_thing = tx->size1;
    serialize_join(&size, tx->vch_data, size_thing, data);

    return size;
}

static size_t tx_deserialize_without_sign(unsigned char *data, Transaction *tx)
{
    size_t size = 0;
    size_t size_thing = sizeof(tx->version);
    deserialize_join(&size, data, &tx->version, size_thing);

    size_thing = sizeof(tx->type);
    deserialize_join(&size, data, &tx->type, size_thing);

    size_thing = sizeof(tx->timestamp);
    deserialize_join(&size, data, &tx->timestamp, size_thing);

    size_thing = sizeof(tx->lock_until);
    deserialize_join(&size, data, &tx->lock_until, size_thing);

    size_thing = sizeof(tx->hash_anchor_bytes);
    deserialize_join(&size, data, &tx->hash_anchor_bytes, size_thing);

    size_thing = sizeof(tx->size0);
    deserialize_join(&size, data, &tx->size0, size_thing);

    size_thing = (sizeof(unsigned char) * (32 + 1)) * tx->size0;
    tx->input = (unsigned char *)malloc(size_thing);
    deserialize_join(&size, data, tx->input, size_thing);

    size_thing = sizeof(tx->prefix);
    deserialize_join(&size, data, &tx->prefix, size_thing);

    size_thing = sizeof(tx->address_bytes);
    deserialize_join(&size, data, &tx->address_bytes, size_thing);

    size_thing = sizeof(tx->amount);
    deserialize_join(&size, data, &tx->amount, size_thing);

    size_thing = sizeof(tx->tx_fee);
    deserialize_join(&size, data, &tx->tx_fee, size_thing);

    size_thing = sizeof(tx->size1);
    deserialize_join(&size, data, &tx->size1, size_thing);

    size_thing = tx->size1;
    tx->vch_data = (unsigned char *)malloc(tx->size1);
    deserialize_join(&size, data, tx->vch_data, size_thing);

    return size;
}

typedef struct _KeyPair
{
    uint256_t sk;
    uint256_t pk;
} KeyPair;

void MakeKeyPair(char *pubkey, char *privkey)
{
    uint256_t pk;
    KeyPair keypair;

    crypto_sign_ed25519_keypair((uint8_t *)&pk, (uint8_t *)&keypair);

    uint256_get_hex(&(keypair.sk), privkey);
    uint256_get_hex(&(keypair.pk), pubkey);
}

static void GetPubKeyFromAddress(const char *address, uint256_t *pubkey)
{
    Base32Decode(std::string((char *)&(address[1])), (unsigned char *)pubkey);
}

static void GetAddressFromPubKey(uint256_t *pubkey, char *address)
{
    std::string strDataBase32;
    Base32Encode((unsigned char *)pubkey, strDataBase32);
    std::string addressStr = (std::string(1, '0' + 1) + strDataBase32);
    strcpy(address, addressStr.c_str());
}

void GetPubKeyAddress(const char *pubkey, char *address)
{
    uint256_t pubkey256;
    uint256_set_hex(&pubkey256, (char *)pubkey);

    //std::string addressStr = (std::string(1, '0' + 1) + ToHexString((unsigned char *)&pubkey256, 32));
    //strcpy(address, addressStr.c_str());

    GetAddressFromPubKey(&pubkey256, address);
}

BOOL IsValidAddress(const char *address)
{
    if (address == NULL || address[0] != '1' || strlen(address) != 57)
    {
        return 0;
    }
    else
    {
        return 1;
    }
}

void SignTransation(Transaction *tx, const char *secretHex)
{
    unsigned char data[4096];

    uint256_t hashAchor256;
    uint256_set_hex(&hashAchor256, tx->hash_anchor);
    memcpy(tx->hash_anchor_bytes, &hashAchor256, 32);

    uint256_t address256;
    GetPubKeyFromAddress(tx->address, &address256);
    memcpy(tx->address_bytes, &address256, 32);

    size_t size = tx_serialize_without_sign(tx, data);

    uint256_t secret256;
    uint256_set_hex(&secret256, (char *)secretHex);

    KeyPair key;
    uint256_t pk;
    crypto_sign_ed25519_seed_keypair((uint8_t *)&pk, (uint8_t *)&key, (uint8_t *)&secret256);

    unsigned char hash[32] = {0};
    unsigned char sign[64] = {0};
    crypto_generichash_blake2b(hash, sizeof(hash), data, size, NULL, 0);
    crypto_sign_ed25519_detached(sign, NULL, hash, sizeof(hash), (uint8_t *)&key);

    std::string hexStr = ToHexString(sign, 64);
    memset(tx->sign, 0, 1024);
    strcpy(tx->sign, hexStr.c_str());
    tx->size2 = 64;
}

void SerializeTxWithSign(Transaction *tx, char *dataHex)
{
    size_t size = 0;
    unsigned char data[1024 * 5];

    uint256_t hashAchor256;
    uint256_set_hex(&hashAchor256, tx->hash_anchor);
    memcpy(tx->hash_anchor_bytes, &hashAchor256, 32);

    uint256_t address256;
    GetPubKeyFromAddress(tx->address, &address256);
    memcpy(tx->address_bytes, &address256, 32);

    std::vector<unsigned char> signBytes = ParseHexString(tx->sign);

    tx->size2 = signBytes.size();
    tx->sign_bytes = (unsigned char *)malloc(tx->size2);
    memcpy(tx->sign_bytes, signBytes.data(), tx->size2);

    size = tx_serialize_without_sign(tx, data);

    size_t size_thing = sizeof(tx->size2);
    serialize_join(&size, &tx->size2, size_thing, data);

    size_thing = tx->size2;
    serialize_join(&size, tx->sign_bytes, size_thing, data);

    strcpy(dataHex, ToHexString(data, size).c_str());
}

void SerializeTxWithoutSign(Transaction *tx, char *dataHex)
{
    size_t size = 0;
    unsigned char data[1024 * 5];

    uint256_t hashAchor256;
    uint256_set_hex(&hashAchor256, tx->hash_anchor);
    memcpy(tx->hash_anchor_bytes, &hashAchor256, 32);

    uint256_t address256;
    GetPubKeyFromAddress(tx->address, &address256);
    memcpy(tx->address_bytes, &address256, 32);

    size = tx_serialize_without_sign(tx, data);

    strcpy(dataHex, ToHexString(data, size).append("00").c_str());
}

void DeserializeTxWithSign(const char *dataHex, Transaction *tx)
{

    size_t size = 0;

    std::string hexStr(dataHex);
    auto data = ParseHexString(hexStr);

    size = tx_deserialize_without_sign(data.data(), tx);

    size_t size_thing = sizeof(tx->size2);
    deserialize_join(&size, data.data(), &tx->size2, size_thing);

    size_thing = tx->size2 * sizeof(unsigned char);
    tx->sign_bytes = (unsigned char *)malloc(size_thing);

    deserialize_join(&size, data.data(), tx->sign_bytes, size_thing);

    uint256_t hashAchor256;
    memcpy(&hashAchor256, tx->hash_anchor_bytes, 32);
    uint256_get_hex(&hashAchor256, tx->hash_anchor);

    uint256_t address256;
    memcpy(&address256, tx->address_bytes, 32);
    GetAddressFromPubKey(&address256, tx->address);

    strcpy(tx->sign, ToHexString(tx->sign_bytes, size_thing).c_str());
}

void DeserializeTxWithoutSign(const char *dataHex, Transaction *tx)
{
    size_t size = 0;

    std::string hexStr(dataHex);
    auto data = ParseHexString(hexStr);

    size = tx_deserialize_without_sign(data.data(), tx);

    uint256_t hashAchor256;
    memcpy(&hashAchor256, tx->hash_anchor_bytes, 32);
    uint256_get_hex(&hashAchor256, tx->hash_anchor);

    uint256_t address256;
    memcpy(&address256, tx->address_bytes, 32);
    GetAddressFromPubKey(&address256, tx->address);
}

/////////////////////  for JNI //////////////////////////

static void ConvertTransactionFromJava2C(JNIEnv *env, jobject jtx, Transaction *tx)
{
    jclass txCls = env->GetObjectClass(jtx);

    jfieldID versionField = env->GetFieldID(txCls, "version", "I");
    jfieldID typeField = env->GetFieldID(txCls, "type", "I");
    jfieldID timestampField = env->GetFieldID(txCls, "timestamp", "L");
    jfieldID lockUntilField = env->GetFieldID(txCls, "lockUntil", "L");
    jfieldID hashAnchorField = env->GetFieldID(txCls, "hashAnchor", "Ljava/lang/String;");
    jfieldID inputField = env->GetFieldID(txCls, "input", "[B");
    jfieldID prefixField = env->GetFieldID(txCls, "prefix", "S");
    jfieldID addressField = env->GetFieldID(txCls, "address", "Ljava/lang/String;");
    jfieldID amountField = env->GetFieldID(txCls, "amount", "L");
    jfieldID txFeeField = env->GetFieldID(txCls, "txFee", "L");
    jfieldID vchDataField = env->GetFieldID(txCls, "vchData", "[B");
    jfieldID signField = env->GetFieldID(txCls, "sign", "Ljava/lang/String;");

    env->DeleteLocalRef(txCls);

    tx->version = env->GetIntField(jtx, versionField);
    tx->type = env->GetIntField(jtx, typeField);
    tx->timestamp = env->GetLongField(jtx, timestampField);
    tx->lock_until = env->GetLongField(jtx, lockUntilField);
    tx->prefix = env->GetShortField(jtx, prefixField);
    tx->amount = env->GetLongField(jtx, amountField);
    tx->tx_fee = env->GetLongField(jtx, txFeeField);
    tx->size2 = 64;

    jstring hashAnchorStr = (jstring)env->GetObjectField(jtx, hashAnchorField);
    const char *hash_anchor = env->GetStringUTFChars(hashAnchorStr, JNI_FALSE);
    strcpy(tx->hash_anchor, hash_anchor);
    env->ReleaseStringUTFChars(hashAnchorStr, hash_anchor);
    env->DeleteLocalRef(hashAnchorStr);

    jstring addrStr = (jstring)env->GetObjectField(jtx, addressField);
    const char *address = env->GetStringUTFChars(addrStr, JNI_FALSE);
    strcpy(tx->address, address);
    env->ReleaseStringUTFChars(addrStr, address);
    env->DeleteLocalRef(addrStr);

    jstring signStr = (jstring)env->GetObjectField(jtx, signField);
    const char *sign = env->GetStringUTFChars(signStr, JNI_FALSE);
    strcpy(tx->sign, sign);
    env->ReleaseStringUTFChars(signStr, sign);
    env->DeleteLocalRef(signStr);

    jbyteArray inputData = (jbyteArray)env->GetObjectField(jtx, inputField);
    jbyte *pInput = env->GetByteArrayElements(inputData, JNI_FALSE);
    tx->input = (unsigned char *)pInput;
    jsize size0 = env->GetArrayLength(inputData);
    tx->size0 = size0;
    env->ReleaseByteArrayElements(inputData, pInput, 0);
    env->DeleteLocalRef(inputData);

    jbyteArray vchData = (jbyteArray)env->GetObjectField(jtx, vchDataField);
    jbyte *pVchData = env->GetByteArrayElements(vchData, JNI_FALSE);
    tx->vch_data = (unsigned char *)pVchData;
    jsize size1 = env->GetArrayLength(vchData);
    tx->size1 = size1;
    env->ReleaseByteArrayElements(vchData, pVchData, 0);
    env->DeleteLocalRef(vchData);
}

static jobject ConvertTransactionFromC2Java(JNIEnv *env, Transaction *tx)
{
    jclass txCls = env->FindClass("com/bigbang/Transaction");

    jmethodID constructor = env->GetMethodID(txCls, "<init>", "()V");

    jobject jtx = env->NewObject(txCls, constructor);

    jfieldID versionField = env->GetFieldID(txCls, "version", "I");
    jfieldID typeField = env->GetFieldID(txCls, "type", "I");
    jfieldID timestampField = env->GetFieldID(txCls, "timestamp", "L");
    jfieldID lockUntilField = env->GetFieldID(txCls, "lockUntil", "L");
    jfieldID hashAnchorField = env->GetFieldID(txCls, "hashAnchor", "Ljava/lang/String;");
    jfieldID inputField = env->GetFieldID(txCls, "input", "[B");
    jfieldID prefixField = env->GetFieldID(txCls, "prefix", "S");
    jfieldID addressField = env->GetFieldID(txCls, "address", "Ljava/lang/String;");
    jfieldID amountField = env->GetFieldID(txCls, "amount", "L");
    jfieldID txFeeField = env->GetFieldID(txCls, "txFee", "L");
    jfieldID vchDataField = env->GetFieldID(txCls, "vchData", "[B");
    jfieldID signField = env->GetFieldID(txCls, "sign", "Ljava/lang/String;");

    env->DeleteLocalRef(txCls);

    env->SetIntField(jtx, versionField, tx->version);
    env->SetIntField(jtx, typeField, tx->type);
    env->SetLongField(jtx, timestampField, tx->timestamp);
    env->SetLongField(jtx, lockUntilField, tx->lock_until);
    env->SetShortField(jtx, prefixField, tx->prefix);
    env->SetLongField(jtx, amountField, tx->amount);
    env->SetLongField(jtx, txFeeField, tx->tx_fee);

    jstring jHashAnchor = env->NewStringUTF(tx->hash_anchor);
    env->SetObjectField(jtx, hashAnchorField, jHashAnchor);
    env->DeleteLocalRef(jHashAnchor);

    jstring jAddress = env->NewStringUTF(tx->address);
    env->SetObjectField(jtx, addressField, jAddress);
    env->DeleteLocalRef(jAddress);

    jstring jSign = env->NewStringUTF(tx->sign);
    env->SetObjectField(jtx, signField, jSign);
    env->DeleteLocalRef(jSign);

    jbyteArray jInput = env->NewByteArray(tx->size0);
    env->SetByteArrayRegion(jInput, 0, tx->size0, (jbyte *)tx->input);
    env->SetObjectField(jtx, inputField, jInput);
    env->DeleteLocalRef(jInput);

    jbyteArray jVchData = env->NewByteArray(tx->size1);
    env->SetByteArrayRegion(jVchData, 0, tx->size1, (jbyte *)tx->vch_data);
    env->SetObjectField(jtx, vchDataField, jVchData);
    env->DeleteLocalRef(jVchData);

    return jtx;
}

jobjectArray JNICALL Java_com_bigbang_BigBangCore_makeKeyPair(JNIEnv *env, jobject jobj)
{
    char pubkey[PUBKEY_HEX_LEN] = {0};
    char privkey[PRIVKEY_HEX_LEN] = {0};

    MakeKeyPair(pubkey, privkey);

    jclass strCls = env->FindClass("java/lang/String");
    jobjectArray ret = env->NewObjectArray(2, strCls, NULL);
    env->DeleteLocalRef(strCls);

    jstring jPubKey = env->NewStringUTF(pubkey);
    env->SetObjectArrayElement(ret, 0, jPubKey);
    env->DeleteLocalRef(jPubKey);

    jstring jPrivKey = env->NewStringUTF(privkey);
    env->SetObjectArrayElement(ret, 1, jPrivKey);
    env->DeleteLocalRef(jPrivKey);

    return ret;
}

jstring JNICALL Java_com_bigbang_BigBangCore_getPubKeyAddress(JNIEnv *env, jobject jobj, jstring pubkey)
{
    const char *nativePubKey = env->GetStringUTFChars(pubkey, JNI_FALSE);

    char pkeyaddr[PUBKEY_ADDRESS_LEN] = {0};
    GetPubKeyAddress(nativePubKey, pkeyaddr);

    env->ReleaseStringUTFChars(pubkey, nativePubKey);

    return env->NewStringUTF(pkeyaddr);
}

jboolean JNICALL Java_com_bigbang_BigBangCore_isValidAddress(JNIEnv *env, jobject jobj, jstring address)
{
    const char *nativeAddress = env->GetStringUTFChars(address, JNI_FALSE);

    jboolean isValid = IsValidAddress(nativeAddress) ? JNI_TRUE : JNI_FALSE;

    env->ReleaseStringUTFChars(address, nativeAddress);

    return isValid;
}

void JNICALL Java_com_bigbang_BigBangCore_signTransaction(JNIEnv *env, jobject jobj, jobject txObj, jstring secretKey)
{
    const char *nativeSecretKey = env->GetStringUTFChars(secretKey, 0);

    Transaction tx;
    ConvertTransactionFromJava2C(env, txObj, &tx);
    SignTransation(&tx, nativeSecretKey);
    ConvertTransactionFromC2Java(env, &tx);

    env->ReleaseStringUTFChars(secretKey, nativeSecretKey);
}

jstring JNICALL Java_com_bigbang_BigBangCore_serializeTxWithoutSign(JNIEnv *env, jobject jObj, jobject txObj)
{
    Transaction tx;
    ConvertTransactionFromJava2C(env, txObj, &tx);
    char dataHex[1024 * 5] = {0};
    SerializeTxWithoutSign(&tx, dataHex);
    jstring jDataHex = env->NewStringUTF(dataHex);
    return jDataHex;
}

jstring JNICALL Java_com_bigbang_BigBangCore_serializeTxWithSign(JNIEnv *env, jobject jObj, jobject txObj)
{
    Transaction tx;
    ConvertTransactionFromJava2C(env, txObj, &tx);
    char dataHex[1024 * 5] = {0};
    SerializeTxWithSign(&tx, dataHex);
    jstring jDataHex = env->NewStringUTF(dataHex);
    free(tx.sign_bytes);
    return jDataHex;
}

jobject JNICALL Java_com_bigbang_BigBangCore_deserializeTxWithSign(JNIEnv *env, jobject jObj, jstring dataHex)
{
    const char *nativeDataHex = env->GetStringUTFChars(dataHex, JNI_FALSE);
    Transaction tx;
    DeserializeTxWithSign(nativeDataHex, &tx);
    jobject jtx = ConvertTransactionFromC2Java(env, &tx);
    free(tx.input);
    free(tx.vch_data);
    free(tx.sign_bytes);
    return jtx;
}

jobject JNICALL Java_com_bigbang_BigBangCore_deserializeTxWithoutSign(JNIEnv *env, jobject jObj, jstring dataHex)
{
    const char *nativeDataHex = env->GetStringUTFChars(dataHex, JNI_FALSE);
    Transaction tx;
    DeserializeTxWithoutSign(nativeDataHex, &tx);
    jobject jtx = ConvertTransactionFromC2Java(env, &tx);
    free(tx.input);
    free(tx.vch_data);
    return jtx;
}
