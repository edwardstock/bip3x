import os
from conans import ConanFile, CMake, tools


def get_version():
    with open(os.path.join(os.path.dirname(__file__), 'version'), 'r') as f:
        content = f.read()
        try:
            content = content.decode()
        except AttributeError:
            pass
        return content.strip()


class Bip39Conan(ConanFile):
    name = "bip39"
    version = get_version()
    license = "MIT"
    author = "Eduard Maximovich edward.vstock@gmail.com"
    url = "https://github.com/edwardstock/native-bip39"
    description = "Bip39 mnemonic C++ implementation. Contains java bindings."
    topics = ("bip39", "bip39-mnemonic", "bip44", "bip39-java")
    settings = "os", "compiler", "build_type", "arch"
    options = {
        "shared": [True, False],
        "enableJNI": [True, False],
        "enableCGO": [True, False],
    }
    default_options = {
        "shared": False,
        "enableJNI": False,
        "enableCGO": False,
    }
    exports = "version"
    exports_sources = (
        "cmake/*",
        "include/*",
        "tests/*",
        "src/*",
        "libs/*",
        "CMakeLists.txt",
        "conanfile.py",
        "LICENSE",
        "README.md",
    )
    generators = "cmake"
    default_user = "edwardstock"
    default_channel = "latest"

    build_requires = (
        "gtest/1.8.1@bincrafters/stable",
    )

    def source(self):
        if "CONAN_LOCAL" not in os.environ:
            self.run("rm -rf *")
            self.run("git clone --recursive https://github.com/edwardstock/native-bip39.git .")

    def build(self):
        cmake = CMake(self)
        opts = {
            'WITH_BIP39_TEST': 'Off',
            'CMAKE_BUILD_TYPE': 'Release',
            'ENABLE_GO': 'Off',
            'ENABLE_JNI': 'Off',
        }

        if self.options["shared"]:
            opts['ENABLE_SHARED'] = 'On'

        if self.options["enableJNI"]:
            opts['ENABLE_JNI'] = 'On'

        if self.options['enableCGO']:
            opts['ENABLE_GO'] = 'On'

        cmake.configure(defs=opts)
        cmake.build()

    def package(self):
        self.copy("*", dst="include", src="include", keep_path=True)
        self.copy("libbip39*.dylib", src="lib", dst="lib", keep_path=False)
        self.copy("libbip39*.so", src="lib", dst="lib", keep_path=False)
        self.copy("libbip39*.a", src="lib", dst="lib", keep_path=False)
        self.copy("libbip39*.dll", src="lib", dst="lib", keep_path=False)
        self.copy("libbip39*.lib", src="lib", dst="lib", keep_path=False)

    def package_info(self):
        self.cpp_info.libs = tools.collect_libs(self)
