#include <sapi/embed/php_embed.h>
// #include <ext/standard/info.h>
#include <ext/json/php_json.h>
#include <Zend/zend_smart_str.h>
#include <Zend/zend_smart_str_public.h>

#include "com_github_adsr_php4j_Php.h"

/** Convert zval to jstring */
static jstring zval2jstring(JNIEnv *env, zval *z) {
    char *json;
    jstring jstr;
    smart_str buf = {0};

    php_json_encode(&buf, z, 0);
    smart_str_0(&buf);
    jstr = (*env)->NewStringUTF(env, ZSTR_VAL(buf.s));
    smart_str_free(&buf);
    zval_ptr_dtor(z);

    return jstr;
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
 * Signature: (Ljava/lang/String;)Ljava/lang/String;
 */
JNIEXPORT jstring JNICALL Java_com_github_adsr_php4j_Php_execString(JNIEnv *env, jclass cls, jstring jcode) {
    zval retval;
    const char *code;
    char *result;

    code = (*env)->GetStringUTFChars(env, jcode, 0);
    zend_first_try {
        zend_eval_string_ex((char*)code, &retval, "php4j", 1);
    } zend_catch {
        ZVAL_NULL(&retval);
    } zend_end_try();
    (*env)->ReleaseStringUTFChars(env, jcode, code);

    return zval2jstring(env, &retval);
}
