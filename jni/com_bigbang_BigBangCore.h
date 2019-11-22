/* DO NOT EDIT THIS FILE - it is machine generated */
#include <jni.h>
/* Header for class com_bigbang_BigBangCore */

#ifndef _Included_com_bigbang_BigBangCore
#define _Included_com_bigbang_BigBangCore
#ifdef __cplusplus
extern "C"
{
#endif
  /*
 * Class:     com_bigbang_BigBangCore
 * Method:    makeKeyPair
 * Signature: ()[Ljava/lang/String;
 */
  JNIEXPORT jobjectArray JNICALL Java_com_bigbang_BigBangCore_makeKeyPair(JNIEnv *, jobject);

  /*
 * Class:     com_bigbang_BigBangCore
 * Method:    getPubKeyAddress
 * Signature: (Ljava/lang/String;)Ljava/lang/String;
 */
  JNIEXPORT jstring JNICALL Java_com_bigbang_BigBangCore_getPubKeyAddress(JNIEnv *, jobject, jstring);

  /*
 * Class:     com_bigbang_BigBangCore
 * Method:    isValidAddress
 * Signature: (Ljava/lang/String;)Z
 */
  JNIEXPORT jboolean JNICALL Java_com_bigbang_BigBangCore_isValidAddress(JNIEnv *, jobject, jstring);

  /*
 * Class:     com_bigbang_BigBangCore
 * Method:    signTransaction
 * Signature: (Lcom/bigbang/Transaction;Ljava/lang/String;)V
 */
  JNIEXPORT void JNICALL Java_com_bigbang_BigBangCore_signTransaction(JNIEnv *, jobject, jobject, jstring);

  /*
 * Class:     com_bigbang_BigBangCore
 * Method:    serializeTxWithoutSign
 * Signature: (Lcom/bigbang/Transaction;)Ljava/lang/String;
 */
  JNIEXPORT jstring JNICALL Java_com_bigbang_BigBangCore_serializeTxWithoutSign(JNIEnv *, jobject, jobject);

  /*
 * Class:     com_bigbang_BigBangCore
 * Method:    serializeTxWithSign
 * Signature: (Lcom/bigbang/Transaction;)Ljava/lang/String;
 */
  JNIEXPORT jstring JNICALL Java_com_bigbang_BigBangCore_serializeTxWithSign(JNIEnv *, jobject, jobject);

  /*
 * Class:     com_bigbang_BigBangCore
 * Method:    deserializeTxWithSign
 * Signature: (Ljava/lang/String;)Lcom/bigbang/Transaction;
 */
  JNIEXPORT jobject JNICALL Java_com_bigbang_BigBangCore_deserializeTxWithSign(JNIEnv *, jobject, jstring);

  /*
 * Class:     com_bigbang_BigBangCore
 * Method:    deserializeTxWithoutSign
 * Signature: (Ljava/lang/String;)Lcom/bigbang/Transaction;
 */
  JNIEXPORT jobject JNICALL Java_com_bigbang_BigBangCore_deserializeTxWithoutSign(JNIEnv *, jobject, jstring);

#ifdef __cplusplus
}
#endif
#endif
