## 1.4.2
 - Updated toolbox (new lightweight api)
 - Clean-up

## 1.4.1
 - Improved FixedData

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