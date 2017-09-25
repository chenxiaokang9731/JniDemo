Jni开发我虽然现在开发中用得少，但是还是得有必要看下的，个人平时对这快也是比较有兴趣，再加上现在在看OpenCV，更加有必要了解下这方面的东西了。

现在在网上这方面的资料虽然多，但是都是比较老的资料了，比如：命名行集成，Elicpse集成，android.mk文件集成。一般都要配置很多东西，用起来都感觉不顺手。而现在官网新出的比较流行的是使用`CMake`进行Jni开发了，所以这篇文章打算结合`CMake`来记录下Jni开发，以及自己使用的过程。

1.想了解其它的方式关于Jni开发的，可以看[这里](http://blog.csdn.net/mabeijianxi/article/details/68525164)
2.关于`CMake`，可以看下[官方文档](https://developer.android.google.cn/studio/projects/add-native-code.html)，里面对于它的使用方法，都有详细介绍。
3.想深入理解的，可以看[这里](http://wiki.jikexueyuan.com/project/deep-android-v1/jni.html)

------------------------------------

###1.)CMake编写

AS2.2以上只要创建项目时，勾选上对C++的支持，就会生成一个支持C++的项目，剩下的我们添加cpp文件时，只要在CMake里面添加，就能调用C++的代码。

但是我还是决定创建一个原生的项目，在原生项目上添加CMake的规则文件，这样记得会更清楚些，至少我们知道在原生的基础上如何用CMake来添加对C++的支持。

< 1 > 添加cpp文件

![这里写图片描述](http://img.blog.csdn.net/20170925144719216?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcXFfMTgyNDIzOTE=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

< 2 > CMakeLists.txt代码（纯粹为了复制）

```
#设置 CMake编译C/C++文件的最小版本

cmake_minimum_required(VERSION 3.4.1)

#设置C/C++资源的配置信息
#三个参数的配置信息
#jni-utils                        表示C/C++文件的名称
#SHARED                           设置库的一种模式  SHARED / STATIC
#src/main/cpp/jni-utils.cpp       C/C++文件的具体路径
add_library(
            jni-utils
            SHARED
            src/main/cpp/jni-utils.cpp
)

# CMake包含了系统库的文件,指定你需要的系统文件
find_library(
            log-lib
            log
)

# 把Cmake指定的系统库文件，链接到C/C++文件中
target_link_libraries(
            jni-utils
            ${log-lib}
)
```

< 3 >Gradle中添加Cmake支持

```
android {
    defaultConfig {
        externalNativeBuild {
            cmake {
                //fexceptions  启用对 C++ 异常处理的支持
                //-frtti 支持 RTTI
                cppFlags "-fexceptions", "-frtti"
                abiFilters "armeabi", "armeabi-v7a"
            }
        }
    }
    //JNI配置 引入cmake的配置文件
    externalNativeBuild {
        cmake {
            path "CMakeLists.txt"
        }
    }
    buildTypes {
        release {
           
        }
    }
}

dependencies {
  
}
```

-----------------------

### 2.)Java调用C++

java调用native首先需要，加载add_library中定义好的c++名称

```
static {
    System.loadLibrary("jni-utils");
}
```

编写调用代码需要加`native`关键字

```
public native static String JavaCallJNI();
```

c++对应的实现

```
/**
 *  java调用jni
 */
JNIEXPORT jstring JNICALL
Java_com_narkang_jni_util_JNIUtil_JavaCallJNI(JNIEnv *env, jclass type) {
    const char* returnValue = "来自C++的问候";
    return env->NewStringUTF(returnValue);
}
```

c++处理int和int[]

java的调用：

```
public native static int JavaCallJNISum(int num1, int num2);

public native static int[] JavaCallJNIArr(int[] arr);
```

c++的处理：

```
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

```

### 3.）Jni回调Java

< 1 > Jni回调Java非静态方法

java的调用：

```
public native static void JNICallJavaBack();

public void JNICallJava(String str){
    Log.i(TAG, str);
}
```

Jni主要还是要通过反射来会掉java方法，其中设计到方法签名，可以看[这里](http://blog.csdn.net/gl_mine_csdn/article/details/77869445)

c++的处理：
```
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
```

< 2 > Jni回调Java静态方法

Java的调用：
```
public native static void JNICallJavaStaticBack();

public static void JNICallJavaStatic(String str){
    Log.i(TAG, str);
}
```

c++的处理:

```
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
```

注意静态方法回调，不需要实例化对象，可以直接通过反射调用。

-----------------------

### 4.）运行结果

手机运行结果

![这里写图片描述](http://img.blog.csdn.net/20170925153656962?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcXFfMTgyNDIzOTE=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

控制台运行结果：

![这里写图片描述](http://img.blog.csdn.net/20170925153717505?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcXFfMTgyNDIzOTE=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

------------------------

### 5.）参考

1.http://blog.csdn.net/column/details/17225.html
2.https://developer.android.google.cn/studio/projects/add-native-code.html
3.http://blog.csdn.net/huachao1001/article/details/53906237
