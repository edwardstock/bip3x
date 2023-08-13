# bip3x (with java bindings)
Bip39 mnemonic C++ implementation. Contains java bindings.

![build status](https://github.com/edwardstock/bip3x/actions/workflows/build_conan.yml/badge.svg)


## Description
Logic almost completely taken from bitcoin-js library for generating mnemonic phrases.
[Site](https://iancoleman.io/bip39/) and [github](https://github.com/iancoleman/bip39)


## Features
* Generate random mnemonic ([PCG](http://www.pcg-random.org/) generator)
* Create root and extended bip* standard keys using derivation path
* Get ETH-like address from private key (requires exactly private key, because it requires to get uncompressed public key)
* Simple message signing with private key (verify method is not implemented yet)

## Build native JNI libs
* Install cmake (if not installed yet)
* Install Oracle JDK or OpenJDK (not tested yet)
* Set environment variable: `JAVA_HOME=/path/to/jdk` OR cmake prefix path like this: `-DCMAKE_PREFIX_PATH="/Users/JohnDoe/.sdkman/candidates/java/17.0.6-amzn"`
* Build
```bash
mkdir build && cd build
cmake ../ -DCMAKE_BUILD_TYPE=Release -Dbip3x_BUILD_SHARED_LIBS=On -Dbip3x_BUILD_JNI_BINDINGS=On -Dbip3x_BUILD_C_BINDINGS=Off -Dbip3x_BUILD_TESTS=Off

make -j 4
```
* Pickup lib files:
  * libbip3x.\[a | so | dylib\]
  * libbip3x_jni.\[so | dylib\]
  
### Cross-compile for Windows under mingw64
* Setup mingw64 for linux or macOS
* Setup Windows JDK (or just copy from windows machine to somewhere path)
* **IMPORTANT:** using c++ (mt19937, and PCGRand not works properly too) random generator is not secure.
  Use `-Dbip3x_USE_OPENSSL_RANDOM=On` while configuring to use OpenSSL random generator.
* Build
```bash
mkdir build && cd build
cmake ../ -DCMAKE_BUILD_TYPE=Release \
-DCMAKE_TOOLCHAIN_FILE=../cmake/mingw-w64.cmake \
-DCMAKE_SYSROOT=/path/to/mingw \
-DCROSS_ARCH=[choose i686 or x86_64] \
-DJAVA_HOME=/path/to/extracted/windows/jdk

make
```
* Pickup lib files:
  * libbip3x.dll
  * libbip3x.dll.a
  * libbip3x_jni.dll
  * libbip3x_jni.dll.a


## Examples
[see exmaple.cpp file](example/example.cpp)


### Java
tbd

### Android and java bindings (todo: separate android library with included java bindings)

* Add this project as submodule (or just copy as sources) to your android module:
```bash
cd /path/to/project/app/src/main
mkdir cpp && cd cpp
git submodule add https://github.com/edwardstock/bip3x.git bip39
```
 
* Create root cmake project in `/path/to/project/[moduleName]/src/main/cpp`
```cmake
project(moduleName-cbip3x)
cmake_minimum_required(VERSION 3.24)
    
# include bip39
include_directories(bip39)
    
# add library
add_subdirectory(bip39)
```
 
* Configure [**gradle**](https://gradle.org/)

```groovy
android {
    ...
    sourceSets {
        main {
            jni.srcDirs = ['src/main/cpp/bip39/src/java']
            java.srcDirs = ['src/main/cpp/bip39/src/java']
        }
    }
    
    externalNativeBuild {
        cmake {
            path 'src/main/cpp/CMakeLists.txt'
        }
    }
    ...
}
 ```
 
* Initialize it

```java
import com.edwardstock.bip3x.BIP3X

class Main {
    public static int main(String[] args) {
        BIP3X.init();

        if (!BIP3X.isEnabled()) {
            throw new RuntimeException(BIP3X.getError());
        }
    }
}
```

* Done!


### Add to CMake project

* Just add it
```cmake
# include bip39
include_directories(bip39)
    
# add library
add_subdirectory(bip39)
```

 and 
```cmake
target_link_libraries(bip39)
```

* Done!
