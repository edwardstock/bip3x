# bip3x (with java bindings)
Bip39 mnemonic C++ implementation. Contains java bindings.

![build status](https://github.com/edwardstock/bip3x/actions/workflows/build_conan.yml/badge.svg)


## Description
Logic almost completely taken from bitcoin-js library for generating mnemonic phrases.
[Site](https://iancoleman.io/bip39/) and [github](https://github.com/iancoleman/bip39)


## Features
* Generate random mnemonic ([PCG](http://www.pcg-random.org/) generator)
* Create root and extended bip* standard keys using some derivation path

## Build native JNI libs
* Install cmake (if not installed yet)
* Install Oracle JDK or OpenJDK (not tested yet)
* Set environment variable: JAVA_HOME=/path/to/jdk
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
  Use `-DUSE_OPENSSL_RANDOM=On` while configuring to use OpenSSL random generator.
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
  

### Build under the Windows
**Requirements**
* CMake for Windows
* Visual Studio (at least community edition), tested only version 2019
* Visual Studio C++ compiler
* To build tests, you could install Conan (`-DCMAKE_PROJECT_TOP_LEVEL_INCLUDES=modules/conan_provider.cmake`) OR using out-of-box fetchcontent

Step-by-step:
```bash
cmake .. -DCMAKE_BUILD_TYPE=Debug -DCMAKE_INSTALL_PREFIX="D:\path\you\want\to\install\libs"
cmake --build .
cmake --build . --target install
```

Then pick-up lib files and headers from **D:\path\you\want\to\install\libs**



## Examples
### C++
```c++
#include <bip3x/utils.h>
#include <bip3x/bip3x_mnemonic.h>
#include <bip3x/HDKeyEncoder.h>
#include <iostream>

using namespace bip3x;

int main(int argc, char** argv) {
    // create random words
    bip3x_mnemonic::MnemonicResult encodedMnemonic = bip3x_mnemonic::generate();
    std::cout << encodedMnemonic.words << std::endl;
    
    // create mnemonic seed
    bytes_64 seed = HDKeyEncoder::make_bip39_seed(encodedMnemonic.words);
    
    // create root key from mnemonic seed
    HDKey bip32RootKey = HDKeyEncoder::makeBip32RootKey(seed);
    
    // and, finally derive keys
    // copy key to leave root key
    HDKey ethereumKey = bip32RootKey;
    // make_extended_key modifies source key
    HDKeyEncoder::make_extended_key(ethereumKey, "m/44'/60'/0'/0");

    // extended private key
    std::cout << ethereumKey.ext_private_key.toString() << std::endl;
    
    // private key
    std::cout << ethereumKey.private_key.toString() << std::endl;
    
    // extended public key
    std::cout << ethereumKey.ext_public_key.toString() << std::endl;
    
    // public key
    std::cout << ethereumKey.public_key.toString() << std::endl;
    
    // et cetera..

    // the end.
    return 0;
}
```


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
import com.edwardstock.bip3x.NativeBip39

class Main {
    public static int main(String[] args) {
        NativeBip39.init();
        
        if(!NativeBip39.isEnabled()) {
            throw new RuntimeException(NativeBip39.getError());
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



## Documentation
TBD (see [src/main.cpp](https://github.com/edwardstock/bip3x/blob/master/src/main.cpp) for simple examples)
