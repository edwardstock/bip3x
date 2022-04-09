# bip3x (with java bindings)
Bip39 mnemonic C++ implementation. Contains java bindings.

[![CircleCI](https://circleci.com/gh/edwardstock/bip3x.svg?style=svg)](https://circleci.com/gh/edwardstock/bip3x)    [ ![Download](https://img.shields.io/badge/Download-Artifactory:Latest-green) ](https://bintray.com/edwardstock/conan-public/bip39:edwardstock/)



## Description
Logic almost completely taken from bitcoin-js library for generating mnemonic phrases.
[Site](https://iancoleman.io/bip39/) and [github](https://github.com/iancoleman/bip39)


## Features
* Generate random mnemonic ([PCG](http://www.pcg-random.org/) generator)
* Create root and extended bip* standard keys using some derivation path

## Install using RPM package

Supported distributions:

- centos 7, 8
- fedora 32-34

Steps:

* Create `edwardstock.repo` file inside `/etc/yum.repos.d`
* Add below to this file

```ini
[edwardstock]
name=edwardstock
baseurl=https://edwardstock.jfrog.io/artifactory/rhel/[centos or fedora]/\$releasever/\$basearch
enabled=1
gpgcheck=0
gpgkey=https://edwardstock.jfrog.io/artifactory/rhel/[centos or fedora]/\$releasever/\$basearch/repodata/repomd.xml.key
repo_gpgcheck=1
```

* Update repository `yum -y update` or `dnf update`
* Install lib `yum install bip3x-devel`

## Install using DEB package

Supported distributions:

- debian: stretch, buster
- ubuntu: xenial, bionic, focal, groovy

```bash
echo "deb https://edwardstock.jfrog.io/artifactory/debian {distribution} main" | sudo tee -a /etc/apt/sources.list
curl -s https://edwardstock.jfrog.io/artifactory/api/gpg/key/public | sudo apt-key add -
apt update && apt install libbip3x-dev
```

<br/><br/>
After installing package from repository, you can use cmake-find module to find library:
```cmake
find_package(bip3x 2.2.1 REQUIRED)
target_link_libraries(MY_PROJECT bip3x::bip39)
```


## Build native JNI libs
* Install cmake (if not installed yet)
* Install Oracle JDK or OpenJDK (not tested yet)
* Set environment variable: JAVA_HOME=/path/to/jdk
* Build
```bash
mkdir build && cd build
cmake ../ -DCMAKE_BUILD_TYPE=Release -DENABLE_SHARED=On -DENABLE_BIP39_JNI=On -DENABLE_BIP39_C=Off -DENABLE_TEST=Off

make -j 4
```
* Pickup lib files:
  * libbip39.\[a | so | dylib\]
  * libbip39_jni.\[so | dylib\]
  
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
  * libbip39.dll
  * libbip39.dll.a
  * libbip39_jni.dll
  * libbip39_jni.dll.a
  

### Build under the Windows
**Requirements**
* CMake for Windows
* Visual Studio (at least community edition), tested only version 2019
* Visual Studio C++ compiler
* To build tests, you could install Conan OR just set configure-time flag `-DENABLE_CONAN=Off` and resolve submodules: `git submodule update --init --recursive`

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
#include <bip3x/Bip39Mnemonic.h>
#include <bip3x/HDKeyEncoder.h>
#include <iostream>

using namespace bip3x;

int main(int argc, char** argv) {
    // create random words
    Bip39Mnemonic::MnemonicResult encodedMnemonic = Bip39Mnemonic::generate();
    std::cout << encodedMnemonic.words << std::endl;
    
    // create mnemonic seed
    bytes_64 seed = HDKeyEncoder::makeBip39Seed(encodedMnemonic.words);
    
    // create root key from mnemonic seed
    HDKey bip32RootKey = HDKeyEncoder::makeBip32RootKey(seed);
    
    // and, finally derive keys
    // copy key to leave root key
    HDKey ethereumKey = bip32RootKey;
    // makeExtendedKey modifies source key
    HDKeyEncoder::makeExtendedKey(ethereumKey, "m/44'/60'/0'/0");

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
git submodule add https://github.com/edwardstock/bip3x.git bip39
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


