## 2.2.1

- Using gtest 1.11.0

## 2.2.0

- Updated toolbox to latest version
- Added ability to use OpenSSL random generator (conan option: with_openssl_rand, cmake: -DUSE_OPENSSL_RANDOM)

## 2.1.1

- Fixed mnemonic generation for MinGW

## 2.1.0

- Removed submodules as dependencies (except gtest) as it leads to version incompatibility
- CMake install config and targets
- Packaging config
- Adopted for old GCC < 5

## 2.0.1

- Updated toolbox to 3.1.1

## 2.0.0

- Breaking: Removed minter:Data* types, now using toolbox bytes buffer
- Breaking: Unified headers location, now all locations starts with bip3x and bip3x/crypto.
- Breaking: Renamed minter namespace to bip3x. The same for JNI.

## 1.4.3

- Updates for Windows

## 1.4.2

- Updated toolbox (new lightweight api)
- Clean-up

## 1.4.1

- Improved bytes_array

## 1.4.0

- Changed behavior of making extended key - now `makeExtendedKey` passes `rootHdKey` by reference to make from it extended key, no more copies.
  The same behavior for c-library.
- PVS-Studio analyzing
- A little cleanup
- More safely clean data
## 1.2.5

- Fixed conanfile.py

## 1.2.4

- Fixed definitions generation

## 1.2.3

- Fixed creating shared DLLs on windows

## 1.2.1

- Correct packaging MSVC built libraries

## 1.2.0

- Renamed and standartized c-headers (prefix c)
- Fixed github release package script

## 1.1.0

- Renamed build-time options for convenient usage with "subdirectory" approach, see first lines of CMakeLists.txt
- Renamed `bip39_go` target to `cbip39`