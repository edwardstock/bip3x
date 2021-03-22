@echo off
set CONAN_LOCAL=1

for /f %%i in ('type ..\version') do set VERS=%%i

@rem default config: enable c lib, disable jni
conan create . edwardstock/latest -s build_type=Debug --build=missing
conan create . edwardstock/latest -s build_type=Release --build=missing

@rem no c lib
conan create . edwardstock/latest -s build_type=Debug --build=missing -o enableC=False
conan create . edwardstock/latest -s build_type=Release --build=missing -o enableC=False
conan create . edwardstock/latest -s build_type=Debug --build=missing -o enableC=False -o shared=True
conan create . edwardstock/latest -s build_type=Release --build=missing -o enableC=False -o shared=True

@rem enable jni
conan create . edwardstock/latest -s build_type=Debug --build=missing -o enableJNI=True
conan create . edwardstock/latest -s build_type=Release --build=missing -o enableJNI=True
conan create . edwardstock/latest -s build_type=Debug --build=missing -o enableJNI=True -o shared=True
conan create . edwardstock/latest -s build_type=Release --build=missing -o enableJNI=True -o shared=True

@rem no c lib, enable jni
conan create . edwardstock/latest -s build_type=Debug --build=missing -o enableC=False -o enableJNI=True
conan create . edwardstock/latest -s build_type=Release --build=missing -o enableC=False -o enableJNI=True
conan create . edwardstock/latest -s build_type=Debug --build=missing -o enableC=False -o enableJNI=True -o shared=True
conan create . edwardstock/latest -s build_type=Release --build=missing -o enableC=False -o enableJNI=True -o shared=True

conan upload bip39/%VERS%@edwardstock/latest --all -r=edwardstock