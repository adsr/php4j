#include <sapi/embed/php_embed.h>
#include <ext/json/php_json.h>
#include <Zend/zend_smart_str.h>
#include <Zend/zend_smart_str_public.h>

#include "com_github_adsr_php4j_Php.h"
#include "com_github_adsr_php4j_Zval.h"

/** Convert a Zval Java object to a real zval */
static zval* obj2zval(JNIEnv *env, jobject obj) {
    jclass clazz;
    jfieldID field;
    jlong addr;

    clazz = (*env)->GetObjectClass(env, obj);
    field = (*env)->GetFieldID(env, clazz, "zvalAddr", "J");
    addr = (*env)->GetLongField(env, obj, field);

    return (zval*)addr;
}

/** Convert a real zval to a Zval Java object */
static jobject zval2obj(JNIEnv *env, zval *z) {
    jclass clazz;
    jmethodID ctor;
    jobject obj;
    jfieldID field;
    zval *zdup;

    clazz = (*env)->FindClass(env, "com/github/adsr/php4j/Zval");
    ctor = (*env)->GetMethodID(env, clazz, "<init>", "()V");
    obj = (*env)->NewObject(env, clazz, ctor);
    field = (*env)->GetFieldID(env, clazz, "zvalAddr", "J");

    zdup = (zval*)emalloc(sizeof(zval)); // freed in dispose()
    ZVAL_DUP(zdup, z);
    (*env)->SetLongField(env, obj, field, (jlong)zdup);

    return obj;
}

/*
 * Class:     com_github_adsr_php4j_Php
 * Method:    init
 * Signature: ()V
 */
JNIEXPORT void JNICALL Java_com_github_adsr_php4j_Php_init(JNIEnv *env, jclass cls) {
    php_embed_init(0, NULL);
}

/*
 * Class:     com_github_adsr_php4j_Php
 * Method:    shutdown
 * Signature: ()V
 */
JNIEXPORT void JNICALL Java_com_github_adsr_php4j_Php_shutdown(JNIEnv *env, jclass cls) {
    php_embed_shutdown();
}

/*
 * Class:     com_github_adsr_php4j_Php
 * Method:    execString
 * Signature: (Ljava/lang/String;)Lcom/github/adsr/php4j/Zval;
 */
JNIEXPORT jobject JNICALL Java_com_github_adsr_php4j_Php_execString(JNIEnv *env, jclass cls, jstring jcode) {
    zval retval;
    const char *code;
    char *result;
    jobject obj;

    code = (*env)->GetStringUTFChars(env, jcode, 0);
    zend_first_try {
        zend_eval_string_ex((char*)code, &retval, "php4j", 1);
    } zend_catch {
        ZVAL_NULL(&retval);
    } zend_end_try();
    (*env)->ReleaseStringUTFChars(env, jcode, code);

    obj = zval2obj(env, &retval);
    zval_ptr_dtor(&retval);

    return obj;
}

/*
 * Class:     com_github_adsr_php4j_Zval
 * Method:    getLong
 * Signature: ()J
 */
JNIEXPORT jlong JNICALL Java_com_github_adsr_php4j_Zval_getLong(JNIEnv *env, jobject obj) {
    zval *z;
    z = obj2zval(env, obj);
    return z ? zval_get_long(z) : 0;
}

/*
 * Class:     com_github_adsr_php4j_Zval
 * Method:    getDouble
 * Signature: ()D
 */
JNIEXPORT jdouble JNICALL Java_com_github_adsr_php4j_Zval_getDouble(JNIEnv *env, jobject obj) {
    zval *z;
    z = obj2zval(env, obj);
    return z ? zval_get_double(z) : 0.0;
}

/*
 * Class:     com_github_adsr_php4j_Zval
 * Method:    getBoolean
 * Signature: ()Z
 */
JNIEXPORT jboolean JNICALL Java_com_github_adsr_php4j_Zval_getBoolean(JNIEnv *env, jobject obj) {
    zval *z;
    z = obj2zval(env, obj);
    return z ? zval_is_true(z) : 0;
}

/*
 * Class:     com_github_adsr_php4j_Zval
 * Method:    getString
 * Signature: ()Ljava/lang/String;
 */
JNIEXPORT jstring JNICALL Java_com_github_adsr_php4j_Zval_getString(JNIEnv *env, jobject obj) {
    zval *z;
    zend_string *str;
    jstring jstr;

    z = obj2zval(env, obj);
    if (!z) {
        return NULL;
    }

    str = zval_get_string(z);
    jstr = (*env)->NewStringUTF(env, ZSTR_VAL(str));
    zend_string_release(str);

    return jstr;
}

/*
 * Class:     com_github_adsr_php4j_Zval
 * Method:    getArray
 * Signature: ()[Lcom/github/adsr/php4j/Zval;
 */
JNIEXPORT jobjectArray JNICALL Java_com_github_adsr_php4j_Zval_getArray(JNIEnv *env, jobject obj) {
    zval *z;
    zval zcopy;
    jobjectArray array;
    jclass clazz;
    HashTable *array_ht;
    zend_long array_len;
    zend_string *array_key;
    jsize array_i;
    zval *array_val;

    z = obj2zval(env, obj);
    if (!z) {
        return NULL;
    }

    ZVAL_COPY(&zcopy, z);
    convert_to_array(&zcopy);

    clazz = (*env)->FindClass(env, "com/github/adsr/php4j/Zval");

    array_ht = Z_ARRVAL(zcopy);
    array_len = zend_array_count(array_ht);
    array = (*env)->NewObjectArray(env, array_len, clazz, NULL);
    array_i = 0;
    ZEND_HASH_FOREACH_VAL(array_ht, array_val) {
        (*env)->SetObjectArrayElement(env, array, array_i, zval2obj(env, array_val));
        array_i += 1;
    } ZEND_HASH_FOREACH_END();
    zval_ptr_dtor(&zcopy);

    return array;
}

/*
 * Class:     com_github_adsr_php4j_Zval
 * Method:    getHash
 * Signature: ()Ljava/util/Map;
 */
JNIEXPORT jobject JNICALL Java_com_github_adsr_php4j_Zval_getHash(JNIEnv *env, jobject obj) {
    zval *z;
    zval zcopy;
    jobject map;
    jmethodID ctor;
    jmethodID put;
    jclass clazz;
    HashTable *array_ht;
    zend_string *array_key;
    zend_long array_i;
    zval *array_val;

    z = obj2zval(env, obj);
    if (!z) {
        return NULL;
    }

    ZVAL_COPY(&zcopy, z);
    convert_to_array(&zcopy);

    clazz = (*env)->FindClass(env, "java/util/HashMap");
    ctor = (*env)->GetMethodID(env, clazz, "<init>", "()V");
    put = (*env)->GetMethodID(env, clazz, "put", "(Ljava/lang/Object;Ljava/lang/Object;)Ljava/lang/Object;");
    map = (*env)->NewObject(env, clazz, ctor);

    array_ht = Z_ARRVAL(zcopy);
    ZEND_HASH_FOREACH_KEY_VAL(array_ht, array_i, array_key, array_val) {
        (*env)->CallObjectMethod(env, map, put,
            (*env)->NewStringUTF(env, ZSTR_VAL(array_key)),
            zval2obj(env, array_val)
        );
    } ZEND_HASH_FOREACH_END();
    zval_ptr_dtor(&zcopy);

    return map;
}

/*
 * Class:     com_github_adsr_php4j_Zval
 * Method:    getJson
 * Signature: ()Ljava/lang/String;
 */
JNIEXPORT jstring JNICALL Java_com_github_adsr_php4j_Zval_getJson(JNIEnv *env, jobject obj) {
    char *json;
    jstring jstr;
    smart_str buf = {0};
    zval *z;

    z = obj2zval(env, obj);
    if (!z) {
        return NULL;
    }

    php_json_encode(&buf, z, 0);
    smart_str_0(&buf);
    jstr = (*env)->NewStringUTF(env, ZSTR_VAL(buf.s));
    smart_str_free(&buf);

    return jstr;
}

/*
 * Class:     com_github_adsr_php4j_Zval
 * Method:    dispose
 * Signature: ()V
 */
JNIEXPORT void JNICALL Java_com_github_adsr_php4j_Zval_dispose(JNIEnv *env, jobject obj) {
    zval *z;
    jclass clazz;
    jfieldID field;
    z = obj2zval(env, obj);
    if (!z) {
        return;
    }
    clazz = (*env)->GetObjectClass(env, obj);
    field = (*env)->GetFieldID(env, clazz, "zvalAddr", "J");
    (*env)->SetLongField(env, obj, field, (jlong)0);
    zval_ptr_dtor(z);
    efree(z); // allocated in zval2obj()
}
