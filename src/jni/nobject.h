#pragma once

#include <bip3x/details/utils.h>
#include <jni.h>
#include <string>

namespace bip3x {

class nobject {
private:
    JNIEnv* env;
    jobject obj;
    jclass cls;
    bool instantiated;

public:
    nobject(JNIEnv* env, const jclass& cls);

    nobject(JNIEnv* env, const char* className);
    nobject(JNIEnv* env, const jobject& obj);
    nobject(JNIEnv* env, const jclass& cls, const jobject& obj);
    ~nobject();

    void setField(const char* fname, uint8_t value);
    void setField(const char* fname, int8_t value);
    void setField(const char* fname, uint16_t value);
    void setField(const char* fname, int16_t value);
    void setField(const char* fname, uint32_t value);
    void setField(const char* fname, int32_t value);
    void setField(const char* fname, uint64_t value);
    void setField(const char* fname, int64_t value);
    void setField(const char* fname, bool value);
    void setField(const char* fname, const std::string& value);
    void setField(const char* fname, const char* value);
    void setField(const char* fname, const jobject& value);

    template<size_t N>
    void setFieldUint8Array(const char* fname, const bip3x::bytes_array<N>& data) {
        auto sz = static_cast<jsize>(data.size());
        jbyteArray arr = env->NewByteArray(sz);
        env->SetByteArrayRegion(arr, 0, sz, reinterpret_cast<const jbyte*>(data.cdata()));

        env->SetObjectField(obj, env->GetFieldID(cls, fname, "[B"), arr);
    }

    void setFieldUint8Array(const char* fname, const bip3x::bytes_data& data);

    uint8_t getFieldUint8(const char* fname);
    int8_t getFieldInt8(const char* fname);
    uint16_t getFieldUint16(const char* fname);
    int16_t getFieldInt16(const char* fname);
    int32_t getFieldInt32(const char* fname);
    uint32_t getFieldUint32(const char* fname);
    int64_t getFieldInt64(const char* fname);
    uint64_t getFieldUint64(const char* fname);
    bool getFieldBool(const char* fname);

    template<typename T>
    T getFieldObject(const char* fname, const char* sig) {
        return (T) env->GetObjectField(obj, env->GetFieldID(cls, fname, sig));
    }

    template<size_t N>
    bip3x::bytes_array<N> getFieldUint8Array(const char* fname) {
        auto tmpArr = getFieldObject<jbyteArray>(fname, "[B" /*byte[]*/);
        return bip3x::bytes_array<N>(reinterpret_cast<uint8_t*>(env->GetByteArrayElements(tmpArr, 0)
        ));
    }

    uint32_t* getFieldUint32Array(const char* fname) {
        jintArray ar = getFieldObject<jintArray>(fname, "[I" /*int[]*/);
        return reinterpret_cast<uint32_t*>(env->GetIntArrayElements(ar, 0));
    }

    JNIEnv* getEnv() {
        return env;
    }

    jobject getObject() {
        return obj;
    }

    jclass& getClass() {
        return cls;
    }
};

} // namespace bip3x
