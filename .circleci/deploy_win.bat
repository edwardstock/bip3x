@echo off
set CONAN_LOCAL=1

for /f %%i in ('git rev-parse --short HEAD') do set REV=%%i
for /f %%i in ('type ..\version') do set VERS=%%i

@rem default config: enable c lib, disable jni
conan create . edwardstock/latest -s compiler.version=16 -s compiler.runtime=MTd -s compiler="Visual Studio" -s build_type=Debug --build=missing
conan create . edwardstock/latest -s compiler.version=16 -s compiler.runtime=MT -s compiler="Visual Studio" -s build_type=Release --build=missing
conan create . edwardstock/latest -s compiler.version=16 -s compiler.runtime=MDd -s compiler="Visual Studio" -s build_type=Debug --build=missing -o bip39:shared=True
conan create . edwardstock/latest -s compiler.version=16 -s compiler.runtime=MD -s compiler="Visual Studio" -s build_type=Release --build=missing -o bip39:shared=True

@rem no c lib
conan create . edwardstock/latest -s compiler.version=16 -s compiler.runtime=MTd -s compiler="Visual Studio" -s build_type=Debug --build=missing -o bip39:enableC=False
conan create . edwardstock/latest -s compiler.version=16 -s compiler.runtime=MT -s compiler="Visual Studio" -s build_type=Release --build=missing -o bip39:enableC=False
conan create . edwardstock/latest -s compiler.version=16 -s compiler.runtime=MDd -s compiler="Visual Studio" -s build_type=Debug --build=missing -o bip39:shared=True -o bip39:enableC=False
conan create . edwardstock/latest -s compiler.version=16 -s compiler.runtime=MD -s compiler="Visual Studio" -s build_type=Release --build=missing -o bip39:shared=True -o bip39:enableC=False

@rem enable jni
conan create . edwardstock/latest -s compiler.version=16 -s compiler.runtime=MTd -s compiler="Visual Studio" -s build_type=Debug --build=missing -o bip39:enableJNI=True
conan create . edwardstock/latest -s compiler.version=16 -s compiler.runtime=MT -s compiler="Visual Studio" -s build_type=Release --build=missing -o bip39:enableJNI=True
conan create . edwardstock/latest -s compiler.version=16 -s compiler.runtime=MDd -s compiler="Visual Studio" -s build_type=Debug --build=missing -o bip39:shared=True -o bip39:enableJNI=True
conan create . edwardstock/latest -s compiler.version=16 -s compiler.runtime=MD -s compiler="Visual Studio" -s build_type=Release --build=missing -o bip39:shared=True -o bip39:enableJNI=True

@rem no c lib, enable jni
conan create . edwardstock/latest -s compiler.version=16 -s compiler.runtime=MTd -s compiler="Visual Studio" -s build_type=Debug --build=missing -o bip39:enableC=False -o bip39:enableJNI=True
conan create . edwardstock/latest -s compiler.version=16 -s compiler.runtime=MT -s compiler="Visual Studio" -s build_type=Release --build=missing -o bip39:enableC=False -o bip39:enableJNI=True
conan create . edwardstock/latest -s compiler.version=16 -s compiler.runtime=MDd -s compiler="Visual Studio" -s build_type=Debug --build=missing -o bip39:shared=True -o bip39:enableC=False -o bip39:enableJNI=True
conan create . edwardstock/latest -s compiler.version=16 -s compiler.runtime=MD -s compiler="Visual Studio" -s build_type=Release --build=missing -o bip39:shared=True -o bip39:enableC=False -o bip39:enableJNI=True

rem conan upload bip39/%VERS%@edwardstock/latest --all -r=edwardstock