//
// MinterWallet. 2018
// Eduard Maximovich <edward.vstock@gmail.com>
//

/**
 * 
 * 
 */

#include <string>
#include "nobject.h"

// CTRS
minter::nobject::nobject(JNIEnv *env, const jclass &cls) :
    env(env),
    cls(cls),
    obj(env->AllocObject(cls)),
    instantiated(true) { }

minter::nobject::nobject(JNIEnv *env, const char *className) :
    env(env),
    cls(env->FindClass(className)),
    instantiated(true) {
    obj = env->AllocObject(cls);
}

minter::nobject::nobject(JNIEnv *env, const jobject &obj) :
    env(env),
    obj(obj),
    cls(env->GetObjectClass(obj)),
    instantiated(false) { }

minter::nobject::nobject(JNIEnv *env, const jclass &cls, const jobject &obj) :
    env(env),
    cls(cls),
    obj(obj),
    instantiated(false) { }

minter::nobject::~nobject() {
    if (instantiated) {
//        env->DeleteLocalRef(obj);
    }
}

// API
void minter::nobject::setField(const char *fname, uint8_t value) {
    jfieldID fid = env->GetFieldID(cls, fname, "B");
    env->SetByteField(obj, fid, (jbyte) value);
}
void minter::nobject::setField(const char *fname, int8_t value) {
    jfieldID fid = env->GetFieldID(cls, fname, "B");
    env->SetByteField(obj, fid, (jbyte) value);
}
void minter::nobject::setField(const char *fname, uint16_t value) {
    jfieldID fid = env->GetFieldID(cls, fname, "S");
    env->SetShortField(obj, fid, (jshort) value);
}
void minter::nobject::setField(const char *fname, int16_t value) {
    jfieldID fid = env->GetFieldID(cls, fname, "S");
    env->SetShortField(obj, fid, (jshort) value);
}
void minter::nobject::setField(const char *fname, uint32_t value) {
    jfieldID fid = env->GetFieldID(cls, fname, "I");
    env->SetIntField(obj, fid, (jint) value);
}
void minter::nobject::setField(const char *fname, int32_t value) {
    jfieldID fid = env->GetFieldID(cls, fname, "I");
    env->SetIntField(obj, fid, value);
}
void minter::nobject::setField(const char *fname, uint64_t value) {
    jfieldID fid = env->GetFieldID(cls, fname, "J");
    env->SetLongField(obj, fid, (jlong) value);
}
void minter::nobject::setField(const char *fname, int64_t value) {
    jfieldID fid = env->GetFieldID(cls, fname, "J");
    env->SetLongField(obj, fid, value);
}
void minter::nobject::setField(const char *fname, bool value) {
    jfieldID fid = env->GetFieldID(cls, fname, "Z");
    env->SetLongField(obj, fid, (jboolean) value);
}
void minter::nobject::setField(const char *fname, const std::string &value) {
    jfieldID fid = env->GetFieldID(cls, fname, "Ljava/lang/String;");
    const char *s = value.c_str();
    jstring v = env->NewStringUTF(s);
    env->SetObjectField(obj, fid, v);
    env->ReleaseStringUTFChars(v, s);
}
void minter::nobject::setField(const char *fname, const char *value) {
    jfieldID fid = env->GetFieldID(cls, fname, "Ljava/lang/String;");
    jstring v = env->NewStringUTF(value);
    env->SetObjectField(obj, fid, v);
    env->ReleaseStringUTFChars(v, value);
}
void minter::nobject::setField(const char *fname, const jobject &value) {
    jfieldID fid = env->GetFieldID(cls, fname, "java/lang/Object");
    env->SetObjectField(obj, fid, value);
}

void minter::nobject::setFieldUint8Array(const char *fname, const minter::Data &data) {
    jsize sz = static_cast<jsize>(data.size());
    jbyteArray arr = env->NewByteArray(sz);
    env->SetByteArrayRegion(arr, 0, sz, reinterpret_cast<const jbyte *>(data.cdata()));
    env->SetObjectField(obj, env->GetFieldID(cls, fname, "[B"), arr);
}
uint8_t minter::nobject::getFieldUint8(const char *fname) {
    return (uint8_t) env->GetByteField(obj, env->GetFieldID(cls, fname, "B"));
}
int8_t minter::nobject::getFieldInt8(const char *fname) {
    return env->GetByteField(obj, env->GetFieldID(cls, fname, "B"));
}
uint16_t minter::nobject::getFieldUint16(const char *fname) {
    return static_cast<uint16_t>(env->GetShortField(obj, env->GetFieldID(cls, fname, "S")));
}
int16_t minter::nobject::getFieldInt16(const char *fname) {
    return env->GetShortField(obj, env->GetFieldID(cls, fname, "S"));
}
int32_t minter::nobject::getFieldInt32(const char *fname) {
    return env->GetIntField(obj, env->GetFieldID(cls, fname, "I"));
}
uint32_t minter::nobject::getFieldUint32(const char *fname) {
    return (uint32_t) env->GetIntField(obj, env->GetFieldID(cls, fname, "I"));
}
int64_t minter::nobject::getFieldInt64(const char *fname) {
    return env->GetIntField(obj, env->GetFieldID(cls, fname, "J"));
}
uint64_t minter::nobject::getFieldUint64(const char *fname) {
    return (uint64_t) env->GetLongField(obj, env->GetFieldID(cls, fname, "J"));
}
bool minter::nobject::getFieldBool(const char *fname) {
    return (bool) env->GetBooleanField(obj, env->GetFieldID(cls, fname, "Z"));
}


