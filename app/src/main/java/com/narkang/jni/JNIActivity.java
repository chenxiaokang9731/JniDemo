package com.narkang.jni;

import android.os.Bundle;
import android.support.v7.app.AppCompatActivity;
import android.widget.TextView;

import com.narkang.jni.util.JNIUtil;

/**
 *  参考的系列文章
 *  http://blog.csdn.net/column/details/17225.html
 */
public class JNIActivity extends AppCompatActivity {

    private TextView tv_jni;
    private TextView tv_jni_int;
    private TextView tv_jni_array;
    private TextView tv_ccalljava;

//    private JNIUtil jniUtil = new JNIUtil();

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_jni);

        tv_jni = (TextView) findViewById(R.id.tv_jni);
        tv_jni.setText(JNIUtil.JavaCallJNI());

        tv_jni_int = (TextView) findViewById(R.id.tv_jni_int);
        tv_jni_int.setText("10 + 20 = "+String.valueOf(JNIUtil.JavaCallJNISum(10, 20)));

        tv_jni_array = (TextView) findViewById(R.id.tv_jni_array);
        int[] intArray = {1, 2, 3};
        intArray = JNIUtil.JavaCallJNIArr(intArray);
        StringBuffer sb = new StringBuffer();
        for (int i: intArray) {
            sb.append(i).append(" ");
        }
        tv_jni_array.setText(sb.toString());

        tv_ccalljava = (TextView) findViewById(R.id.tv_ccalljava);
        JNIUtil.JNICallJavaBack();
        JNIUtil.JNICallJavaStaticBack();
    }
}
