//
// Created by Administrator on 2017/9/25.
//
#include <jni.h>
#include <stdio.h>

extern "C"{
    JNIEXPORT jstring JNICALL
    Java_com_narkang_jni_util_JNIUtil_JavaCallJNI(JNIEnv *env, jclass type);
    JNIEXPORT jint JNICALL
    Java_com_narkang_jni_util_JNIUtil_JavaCallJNISum(JNIEnv *env, jclass type, jint num1, jint num2);
    JNIEXPORT jintArray JNICALL
    Java_com_narkang_jni_util_JNIUtil_JavaCallJNIArr(JNIEnv *env, jclass type, jintArray arr_);
    JNIEXPORT void JNICALL
    Java_com_narkang_jni_util_JNIUtil_JNICallJavaBack(JNIEnv *env, jclass type);
    JNIEXPORT void JNICALL
    Java_com_narkang_jni_util_JNIUtil_JNICallJavaStaticBack(JNIEnv *env, jclass type);
}

/**
 *  java调用jni
 */
JNIEXPORT jstring JNICALL
Java_com_narkang_jni_util_JNIUtil_JavaCallJNI(JNIEnv *env, jclass type) {
    const char* returnValue = "来自C++的问候";
    return env->NewStringUTF(returnValue);
}

/**
 *  java处理jni的int
 */
JNIEXPORT jint JNICALL
Java_com_narkang_jni_util_JNIUtil_JavaCallJNISum(JNIEnv *env, jclass type, jint num1, jint num2) {
    return num1 + num2;
}

/**
 * java处理jni的数组
 */
JNIEXPORT jintArray JNICALL
Java_com_narkang_jni_util_JNIUtil_JavaCallJNIArr(JNIEnv *env, jclass type, jintArray arr_) {
    //1.获取arr数组的元素
    jint *arr = env->GetIntArrayElements(arr_, NULL);
    //2.获取arr数组的长度
    jint arrsize = env -> GetArrayLength(arr_);
    //3.遍历数组
    for (int i = 0; i < arrsize; ++i) {
        *(arr + i) += 10;
    }
    //4.释放内存
    env->ReleaseIntArrayElements(arr_, arr, 0);
    //5.返回值
    return arr_;
}

/**
 * jni回调java非静态方法
 */
JNIEXPORT void JNICALL
Java_com_narkang_jni_util_JNIUtil_JNICallJavaBack(JNIEnv *env, jclass type) {
    //1.得到类的字节码
    jclass cls = env->FindClass("com/narkang/jni/util/JNIUtil");
    //2.获取方法id
    //cls类的字节码 name java方法名 sig java方法签名
    jmethodID  mid = env->GetMethodID(cls, "JNICallJava", "(Ljava/lang/String;)V");
    //3.实例化该类
    jobject jobject = env->AllocObject(cls);
    //4.设置java层参数的值
    jstring str = env->NewStringUTF("来自JNI的回调 JNICallJava");
    //5.设置java层的方法
    env->CallVoidMethod(jobject, mid, str);
}

/**
 * jni回调java静态方法
 */
JNIEXPORT void JNICALL
Java_com_narkang_jni_util_JNIUtil_JNICallJavaStaticBack(JNIEnv *env, jclass type) {
    //1.得到类的字节码
    jclass cls = env->FindClass("com/narkang/jni/util/JNIUtil");
    //2.获取方法id
    //cls类的字节码 name java方法名 sig java方法签名
    jmethodID  mid = env->GetStaticMethodID(cls, "JNICallJavaStatic", "(Ljava/lang/String;)V");
    //3.设置java层参数的值
    jstring str = env->NewStringUTF("来自JNI的静态方法回调 JNICallJavaStatic");
    //4.设置java层的方法
    env->CallStaticVoidMethod(cls, mid, str);
}