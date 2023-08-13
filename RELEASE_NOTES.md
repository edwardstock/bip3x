## 3.0.0

- Upgraded Conan to version 2.0.
- Introduced Continuous Integration with GitHub Actions.
- Refactored the project structure to enhance readability and maintainability:
    - Moved all non-public sources to `src`.
    - Switched naming convention from CamelCase to snake_case.
- Updated example usage in `example/example.cpp`.
- Introduced several utility functions for basic operations:
    - `bip3x::ecdsa_recoverable_signature bip3x::sign_message(const bytes_data& message, const bytes_32& private_key)`:
      To sign a message.
    - `bip3x::get_eth_address(const bytes_32& private_key)`: To extract the Ethereum address.
  - `bytes_32 bip3x::mnemonic_to_private_key(const std::string& mnemonic_words, const derivation_path& derivation = "m/44'/0'/0'/0/0")`:
      To derive a private key from a mnemonic.
- Updated the JNI code and converted it to Kotlin for improved interoperability.
- Reverted the logic to return a new HDKey from `make_extended_key` function for consistency and added
  function `extend_key` which mutates original `hdkey`
- Fixed issues with building Windows DLLs.

## 2.3.0
- Updated openssl to 3.1.1
- Migrated to conan 2.0
- Added prefixes for cmake build variables `bip3x_*`
- Fixed install targets: pc files, cmake config files etc
- CI: github actions

## 2.2.2

- Added throw if hardening added to derivation path prefix
- Updated conan openssl from 1.1.1k to 3.0.5

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

- Changed behavior of making extended key - now `make_extended_key` passes `rootHdKey` by reference to make from it extended key, no more copies.
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
