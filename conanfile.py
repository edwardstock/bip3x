import os

from conans import ConanFile, CMake


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
    url = "https://github.com/edwardstock/bip3x"
    description = "Bip39 mnemonic C++ implementation. Contains java and pure C bindings."
    topics = ("bip39", "bip39-mnemonic", "bip44", "bip39-java")
    settings = "os", "compiler", "build_type", "arch"
    options = {
        "shared": [True, False],
        "enableJNI": [True, False],
        "enableC": [True, False],
        "with_openssl_rand": [True, False]
    }
    default_options = {
        "shared": False,
        "with_openssl_rand": False,
        "enableJNI": False,
        "enableC": False,
        "toolbox:shared": False,
    }
    exports = "version"
    exports_sources = (
        "modules/*",
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
    generators = "cmake"
    default_user = "edwardstock"
    default_channel = "latest"

    requires = (
        "toolbox/3.2.3@edwardstock/latest"
    )
    build_requires = (
        "gtest/1.11.0",
    )

    def source(self):
        if "CONAN_LOCAL" not in os.environ:
            self.run("rm -rf *")
            self.run("git clone --recursive https://github.com/edwardstock/bip3x.git .")

    def configure(self):
        if self.settings.compiler == "Visual Studio":
            del self.settings.compiler.runtime

        if self.options.with_openssl_rand:
            self.requires.add("openssl/1.1.1k")

    def build(self):
        cmake = CMake(self)
        opts = {
            'ENABLE_TEST': 'Off',
            'CMAKE_BUILD_TYPE': 'Release',
            'ENABLE_BIP39_C': 'Off',
            'ENABLE_BIP39_JNI': 'Off',
            'ENABLE_SHARED': 'Off',
            'USE_OPENSSL_RANDOM': 'Off'
        }

        if self.options.shared:
            opts['ENABLE_SHARED'] = 'On'

        if self.options.enableJNI:
            opts['ENABLE_BIP39_JNI'] = 'On'

        if self.options.enableC:
            opts['ENABLE_BIP39_C'] = 'On'

        if self.options.with_openssl_rand:
            opts["USE_OPENSSL_RANDOM"] = 'On'

        opts['CMAKE_BUILD_TYPE'] = self.settings.get_safe("build_type")

        cmake.configure(defs=opts)
        if self.settings.compiler == "Visual Studio":
            cmake.build(args=['--config', self.settings.get_safe("build_type")])
        else:
            cmake.build()

    def package(self):
        self.copy("*", dst="include", src="include", keep_path=True)
        if self.options.enableC:
            self.copy("*.h", dst="include", src="src/bindings", keep_path=True)
            self.copy("*.hpp", dst="include", src="src/bindings", keep_path=True)

        dir_types = ['bin', 'lib', 'Debug', 'Release', 'RelWithDebInfo', 'MinSizeRel']
        file_types = ['lib', 'dll', 'dll.a', 'a', 'so', 'exp', 'pdb', 'ilk', 'dylib']

        for dirname in dir_types:
            for ftype in file_types:
                self.copy("*." + ftype, src=dirname, dst="lib", keep_path=False)

    def package_info(self):
        self.cpp_info.includedirs = ['include']
        self.cpp_info.libs = ['bip39']
        if self.options.enableC:
            self.cpp_info.libs.append('cbip39')

        if self.options.enableJNI:
            self.cpp_info.libs.append('bip39_jni')
