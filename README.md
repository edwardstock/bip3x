# native-bip39 (with java bindings)
Bip39 mnemonic C++ implementation. Contains java bindings.

[![CircleCI](https://circleci.com/gh/edwardstock/native-bip39.svg?style=svg)](https://circleci.com/gh/edwardstock/native-bip39)



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
cmake ../ -DCMAKE_BUILD_TYPE=Release

make
```
* Pickup lib files:
  * libbip39.[a | so | dylib] 
  * libbip39_jni.[so | dylib] 
  
### Cross-compile for Windows under mingw64 (MSVC didn't tested yet)
* Setup mingw64 for linux or macOS
* Setup Windows JDK (or just copy from windows machine to somewhere path)
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
  * libbip39.dll
  * libbip39.dll.a
  * libbip39_jni.dll
  * libbip39_jni.dll.a
  
Compiling under windows and mingw64 can be successfully, i didn't tested.


## Examples
### C++
```c++
#include <minter/utils.h>
#include <minter/Bip39Mnemonic.h>
#include <minter/HDKeyEncoder.h>
#include <iostream>

using namespace minter;

int main(int argc, char** argv) {
    // create random words
    Bip39Mnemonic::MnemonicResult encodedMnemonic = Bip39Mnemonic::generate();
    std::cout << encodedMnemonic.words << std::endl;
    
    // create mnemonic seed
    Data64 seed = HDKeyEncoder::makeBip39Seed(encodedMnemonic.words);
    
    // create root key from mnemonic seed
    HDKey bip32RootKey = HDKeyEncoder::makeBip32RootKey(seed);
    
    // and, finally derive keys
    HDKey ethereumKey = HDKeyEncoder::makeExtendedKey(bip32RootKey, "m/44'/60'/0'/0");

    // extended private key
    std::cout << ethereumKey.extPrivateKey.toString() << std::endl;
    
    // private key
    std::cout << ethereumKey.privateKey.toString() << std::endl;
    
    // extended public key
    std::cout << ethereumKey.extPublicKey.toString() << std::endl;
    
    // public key
    std::cout << ethereumKey.publicKey.toString() << std::endl;
    
    // et cetera..

    // the end.
    return 0;
}
```


### Java
tbd

### Android and java bindings

* Add this project as submodule (or just copy as sources) to your android module:
```bash
cd /path/to/project/app/src/main
mkdir cpp && cd cpp
git submodule add https://github.com/edwardstock/native-bip39.git bip39
```
 
* Create root cmake project in `/path/to/project/[moduleName]/src/main/cpp`
```cmake
project(moduleName-bindings)
cmake_minimum_required(VERSION 3.4)
    
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
import network.minter.core.bip39.NativeBip39

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
TBD (see [src/main.cpp](https://github.com/edwardstock/native-bip39/blob/master/src/main.cpp) for simple examples)


