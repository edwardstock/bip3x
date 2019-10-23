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
        "enableC": [True, False],
    }
    default_options = {
        "shared": False,
        "enableJNI": False,
        "enableC": True,
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
            'ENABLE_BIP39_TESTS': 'Off',
            'CMAKE_BUILD_TYPE': 'Release',
            'ENABLE_BIP39_C': 'On',
            'ENABLE_BIP39_JNI': 'Off',
            'ENABLE_BIP39_SHARED': 'Off'
        }

        if self.options["shared"]:
            opts['ENABLE_BIP39_SHARED'] = 'On'

        if self.options["enableJNI"]:
            opts['ENABLE_BIP39_JNI'] = 'On'

        if self.options['enableC']:
            opts['ENABLE_BIP39_C'] = 'On'

        cmake.configure(defs=opts)
        if self.settings.compiler == "Visual Studio":
            cmake.build(args=['--config', self.settings.get_safe("build_type")])
        else:
            cmake.build()

    def package(self):
        if self.options.enableC:
            self.copy("*.h", dst="include", src="src/bindings", keep_path=True)
            self.copy("*.hpp", dst="include", src="src/bindings", keep_path=True)

        self.copy("*", dst="include", src="include", keep_path=True)
        self.copy("*.dylib", src="lib", dst="lib", keep_path=False)
        self.copy("*.so", src="lib", dst="lib", keep_path=False)
        self.copy("*.a", src="lib", dst="lib", keep_path=False)
        self.copy("*.lib", src="lib", dst="lib", keep_path=False)
        self.copy("*.lib", src="Release", dst="lib", keep_path=False)
        self.copy("*.lib", src="Debug", dst="lib", keep_path=False)
        self.copy("*.dll", src="lib", dst="lib", keep_path=False)
        self.copy("*.dll", src="Release", dst="lib", keep_path=False)
        self.copy("*.dll", src="Debug", dst="lib", keep_path=False)
        self.copy("*.dll.a", src="lib", dst="lib", keep_path=False)
        self.copy("*.dll.a", src="Release", dst="lib", keep_path=False)
        self.copy("*.dll.a", src="Debig", dst="lib", keep_path=False)
        self.copy("*.exp", src="lib", dst="lib", keep_path=False)
        self.copy("*.exp", src="Release", dst="lib", keep_path=False)
        self.copy("*.exp", src="Debug", dst="lib", keep_path=False)
        self.copy("*.ilk", src="lib", dst="lib", keep_path=False)
        self.copy("*.ilk", src="Release", dst="lib", keep_path=False)
        self.copy("*.ilk", src="Debug", dst="lib", keep_path=False)
        self.copy("*.pdb", src="lib", dst="lib", keep_path=False)
        self.copy("*.pdb", src="Release", dst="lib", keep_path=False)
        self.copy("*.pdb", src="Debug", dst="lib", keep_path=False)


def package_info(self):
    self.cpp_info.libs = tools.collect_libs(self)
