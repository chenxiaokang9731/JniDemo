#include <jni.h>

JNIEXPORT jstring JNICALL
Java_com_narkang_jni_util_JNIUtil_JavaCallJNI(JNIEnv *env, jclass type) {

    // TODO


    return (*env)->NewStringUTF(env, returnValue);
}

JNIEXPORT jint JNICALL
Java_com_narkang_jni_util_JNIUtil_JavaCallJNISum(JNIEnv *env, jclass type, jint num1, jint num2) {

    // TODO

}

JNIEXPORT jintArray JNICALL
Java_com_narkang_jni_util_JNIUtil_JavaCallJNIArr(JNIEnv *env, jclass type, jintArray arr_) {
    jint *arr = (*env)->GetIntArrayElements(env, arr_, NULL);

    // TODO

    (*env)->ReleaseIntArrayElements(env, arr_, arr, 0);
}

JNIEXPORT void JNICALL
Java_com_narkang_jni_util_JNIUtil_JNICallJavaBack(JNIEnv *env, jclass type) {

    // TODO

}

JNIEXPORT void JNICALL
Java_com_narkang_jni_util_JNIUtil_JNiCallJavaStaticBack(JNIEnv *env, jclass type) {

    // TODO

}