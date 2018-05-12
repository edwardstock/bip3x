#include <jni.h>
#include "bip39.h"
/**
 * bip39. 2018
 * NativeBip39.cpp
 *
 * @author Eduard Maximovich <edward.vstock@gmail.com>
 * @link https://github.com/edwardstock
 */

jobjectArray Java_network_minter_mintercore_bip39_NativeBip39_getLanguages(JNIEnv *env, jclass) {

  char *langSrc[7];
  bip39_get_languages(langSrc);
  jsize len = 7; // we can't get length of array

  jobjectArray langArr = env->NewObjectArray(len, env->FindClass("java/lang/String"), NULL);
  for(int i = 0; i < len; i++) {
    env->SetObjectArrayElement(langArr, i, env->NewStringUTF(langSrc[i]));
  }

  return langArr;
}
