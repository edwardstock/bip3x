package com.edwardstock.bip3x

import java.nio.ByteBuffer
import java.nio.ByteOrder


class BIP3X private constructor(
    var isEnabled: Boolean
) {
    companion object {
        const val ENTROPY_LEN_128 = 16
        const val ENTROPY_LEN_160 = 20
        const val ENTROPY_LEN_192 = 24
        const val ENTROPY_LEN_224 = 28
        const val ENTROPY_LEN_256 = 32
        const val ENTROPY_LEN_288 = 36
        const val ENTROPY_LEN_320 = 40
        const val LANG_DEFAULT = "en"
        const val MR_OK = 0
        const val MR_UNSUPPORTED_ENTROPY = 1
        const val MR_UNKNOWN_ERROR = 2

        val LIB_NAMES = arrayOf("bip3x", "bip3x_jni")
        val LIB_FILES = arrayOf("libbip3x_jni.so", "libbip3x.so")
        private var INSTANCE: BIP3X? = null
        var error: Throwable? = null
            private set
        private val nativeBuffer = ThreadLocal<ByteBuffer?>()

        @JvmStatic
        fun init() {
            if (INSTANCE == null) {
                val enabled: Boolean = try {
                    for (soname in LIB_NAMES) {
                        System.loadLibrary(soname)
                    }
                    true
                } catch (e: UnsatisfiedLinkError) {
                    System.err.println(
                        String.format(
                            "Unable to load bip3x library in %s: %s",
                            System.getProperty("java.library.path"),
                            e.message
                        )
                    )
                    error = e
                    false
                }
                INSTANCE = BIP3X(enabled)
            }
        }

        @JvmStatic
        val languages: Array<String>
            get() = bip3xGetLanguages()

        @JvmStatic
        fun getWordsFromLanguage(lang: String): Array<String> {
            require(lang.isNotEmpty()) { "Language required" }
            return bip3xGetWordsFromLanguage(lang)
        }

        /**
         * Generates random mnemonic with PCGRandom for english language and entropy: 16 bytes
         * @return
         */
        @JvmOverloads
        fun generate(language: String = LANG_DEFAULT, entropy: Int = ENTROPY_LEN_128): MnemonicResult {
            return bip3xGenerate(language, entropy)
        }

        @JvmStatic
        @JvmOverloads
        fun encodeBytes(
            input: ByteArray,
            language: String = LANG_DEFAULT,
            entropy: Int = ENTROPY_LEN_128
        ): MnemonicResult {
            var buff = nativeBuffer.get()
            if (buff == null || buff.capacity() < input.size) {
                buff = ByteBuffer.allocateDirect(input.size) ?: error("Unable to allocate direct ByteBuffer")
                buff.order(ByteOrder.BIG_ENDIAN)
                nativeBuffer.set(buff)
            }
            buff.rewind()
            buff.put(input)
            return bip3xEncodeBytes(buff, language, entropy)
        }

        @JvmStatic
        fun validateMnemonic(mnemonic: String?, language: String = LANG_DEFAULT): Boolean {
            return if (mnemonic == null) {
                false
            } else bip3xValidateMnemonic(
                mnemonic,
                language
            )
        }

        @JvmStatic
        fun mnemonicToBip39Seed(mnemonic: String): ByteArray {
            return bip3xWordsToSeed(mnemonic)
        }

        @JvmStatic
        fun getEthAddress(privateKey: ByteArray): String {
            return bip3xGetEthAddress(privateKey)
        }

        @JvmStatic
        fun signMessage(privateKey: ByteArray, message: ByteArray): ECDSASignatureRecoverable {
            val signature = bip3xSignMessage(privateKey, message)
            val r = signature.copyOfRange(0, 32)
            val s = signature.copyOfRange(32, 64)
            val v = signature.copyOfRange(64, 65)
            return ECDSASignatureRecoverable(r, s, v)
        }

        private external fun bip3xGenerate(language: String, entropy: Int): MnemonicResult
        private external fun bip3xGetLanguages(): Array<String>
        private external fun bip3xGetWordsFromLanguage(language: String): Array<String>
        private external fun bip3xEncodeBytes(input: ByteBuffer, language: String, entropy: Int): MnemonicResult
        private external fun bip3xValidateMnemonic(mnemonic: String, language: String): Boolean
        private external fun bip3xWordsToSeed(mnemonic: String): ByteArray
        private external fun bip3xGetEthAddress(privateKey: ByteArray): String
        private external fun bip3xSignMessage(privateKey: ByteArray, message: ByteArray): ByteArray
    }
}