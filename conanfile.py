import os

from conan import ConanFile
from conan.tools.cmake import CMakeToolchain, CMakeDeps, CMake


def get_version():
    with open(os.path.join(os.path.dirname(__file__), 'version.info'), 'r') as f:
        content = f.read()
        try:
            content = content.decode()
        except AttributeError:
            pass
        return content.strip()


# noinspection PyCallingNonCallable
class Bip3xConan(ConanFile):
    name = "bip3x"
    version = get_version()
    license = "MIT"
    author = "Eduard Maximovich edward.vstock@gmail.com"
    url = "https://github.com/edwardstock/bip3x"
    description = "Bip39 mnemonic C++ implementation. Contains java and pure C bindings."
    topics = ("bip39", "bip39-mnemonic", "bip44", "bip39-java", "mnemonic-jni")
    settings = "os", "compiler", "build_type", "arch"
    options = {
        "shared": [True, False],
        "enable_jni": [True, False],
        "enable_c": [True, False],
        "with_openssl_rand": [True, False],
        "with_tests": [True, False]
    }
    default_options = {
        "shared": False,
        "with_openssl_rand": False,
        "enable_jni": False,
        "enable_c": False,
        "with_tests": False,
        "toolbox/*:shared": False,
    }
    exports = "version.info"
    exports_sources = (
        "cmake/*",
        "include/*",
        "cfg/*",
        "tests/*",
        "src/*",
        "libs/*",
        "CMakeLists.txt",
        "conanfile.py",
        "LICENSE",
        "README.md",
    )

    default_user = "edwardstock"
    default_channel = "latest"

    requires = (
        "toolbox/3.4.0@edwardstock/latest"
    )
    build_requires = (
        "gtest/1.13.0",
    )

    def generate(self):
        # This generates "conan_toolchain.cmake" in self.generators_folder
        tc = CMakeToolchain(self)
        tc.generate()

        # This generates "foo-config.cmake" and "bar-config.cmake" in self.generators_folder
        deps = CMakeDeps(self)
        deps.generate()

    def requirements(self):
        if self.options.with_openssl_rand:
            self.requires("openssl/3.1.1")
        if self.options.with_tests:
            self.requires("gtest/1.13.0")

    def build(self):
        cmake = CMake(self)
        opts = {
            'bip3x_BUILD_TESTS': 'Off',
            'bip3x_BUILD_C_BINDINGS': 'Off',
            'bip3x_BUILD_JNI_BINDINGS': 'Off',
            'bip3x_BUILD_SHARED_LIBS': 'Off',
            'bip3x_USE_OPENSSL_RANDOM': 'Off'
        }

        if self.options.shared:
            opts['bip3x_BUILD_SHARED_LIBS'] = 'On'

        if self.options.enable_jni:
            opts['bip3x_BUILD_JNI_BINDINGS'] = 'On'

        if self.options.enable_c:
            opts['bip3x_BUILD_C_BINDINGS'] = 'On'

        if self.options.with_openssl_rand:
            opts["bip3x_USE_OPENSSL_RANDOM"] = 'On'

        cmake.configure(variables=opts)
        cmake.build()

    def package(self):
        cmake = CMake(self)
        cmake.install()

    def package_info(self):
        self.cpp_info.includedirs = ['include']
        self.cpp_info.bindirs = ["bin"]
        self.cpp_info.libsdirs = ["lib", "lib/Debug", "lib/Release"]
        self.cpp_info.libs = ["bip3x"]
        if self.options.enable_c:
            self.cpp_info.libs.append("cbip3x")
        if self.options.enable_jni:
            self.cpp_info.libs.append("bip3x_jni")
