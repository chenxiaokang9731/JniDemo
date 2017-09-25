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