package com.narkang.jni.util;

import android.util.Log;

/**
 * Created by Administrator on 2017/9/25.
 */
public class JNIUtil {

    public static final String TAG = JNIUtil.class.getName();

    static {
        System.loadLibrary("jni-utils");
    }

    public native static String JavaCallJNI();

    public native static int JavaCallJNISum(int num1, int num2);

    public native static int[] JavaCallJNIArr(int[] arr);

    public native static void JNICallJavaBack();

    public void JNICallJava(String str){
        Log.i(TAG, str);
    }

    public native static void JNICallJavaStaticBack();

    public static void JNICallJavaStatic(String str){
        Log.i(TAG, str);
    }
}
