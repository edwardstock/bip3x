# native-bip39 (with java bindings)
Bip39 mnemonic C++ implementation. Contains java bindings.


## Description
Logic almost completely taken from bitcoin-js library for generating mnemonic phrases.
[Site](https://iancoleman.io/bip39/) and [github](https://github.com/iancoleman/bip39)


## Usage

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
TBD (see `src/main.cpp` for simple examples)


