# native-bip39 (with java bindings)
Bip39 mnemonic C++ implementation. Contains java bindings.


## Description
Logic almost completely taken from bitcoin-js library for generating mnemonic phrases.
[Site](https://iancoleman.io/bip39/) and [github](https://github.com/iancoleman/bip39)


## Features
* Generate random mnemonic ([PCG](http://www.pcg-random.org/) generator)
* Create root and extended bip* standard keys using some derivation path

## Examples
### C++
```c++
#include "minter/utils.h"
#include "minter/Bip39Mnemonic.h"
#include "minter/HDKeyEncoder.h"
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
target_link_libraries(bip39_core)
```

* Done!



## Documentation
TBD (see [src/main.cpp](https://github.com/edwardstock/native-bip39/blob/master/src/main.cpp) for simple examples)


