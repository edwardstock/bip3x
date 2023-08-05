package com.edwardstock.bip3x

import java.nio.ByteBuffer
import java.nio.ByteOrder


class NativeBip39 private constructor(
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
        val SONAME = arrayOf("bip3x", "bip3x_jni")
        val LIB_FILES = arrayOf("libbip3x_jni.so", "libbip3x.so")
        private var INSTANCE: NativeBip39? = null
        var error: Throwable? = null
            private set
        private val nativeBuffer = ThreadLocal<ByteBuffer?>()

        @JvmStatic
        fun init() {
            if (INSTANCE == null) {
                val enabled: Boolean = try {
                    for (soname in SONAME) {
                        System.loadLibrary(soname)
                    }
                    true
                } catch (e: UnsatisfiedLinkError) {
                    System.err.println(
                        String.format(
                            "Unable to load bip39 library in %s: %s",
                            System.getProperty("java.library.path"),
                            e.message
                        )
                    )
                    error = e
                    false
                }
                INSTANCE = NativeBip39(enabled)
            }
        }

        @JvmStatic
        val languages: Array<String>
            get() = bip39GetLanguages()

        @JvmStatic
        fun getWordsFromLanguage(lang: String): Array<String> {
            require(lang.isNotEmpty()) { "Language required" }
            return bip39GetWordsFromLanguage(lang)
        }

        /**
         * Generates random mnemonic with PCGRandom for english language and entropy: 16 bytes
         * @return
         */
        @JvmOverloads
        fun generate(language: String = LANG_DEFAULT, entropy: Int = ENTROPY_LEN_128): MnemonicResult {
            return bip39Generate(language, entropy)
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
                buff = ByteBuffer.allocateDirect(input.size)
                buff.order(ByteOrder.BIG_ENDIAN)
                nativeBuffer.set(buff)
            }
            buff!!.rewind()
            buff.put(input)
            return bip39EncodeBytes(buff, language, entropy)
        }

        @JvmStatic
        fun validateMnemonic(mnemonic: String?, language: String = LANG_DEFAULT): Boolean {
            return if (mnemonic == null) {
                false
            } else bip39ValidateMnemonic(
                mnemonic,
                language
            )
        }

        @JvmStatic
        fun mnemonicToBip39Seed(mnemonic: String): ByteArray {
            return bip39WordsToSeed(mnemonic)
        }

        @JvmStatic
        fun getEthAddress(privateKey: ByteArray): String {
            return bip39GetEthAddress(privateKey)
        }

        @JvmStatic
        fun signMessage(privateKey: ByteArray, message: ByteArray): EcdsaRecoverableSignature {
            val signature = bip39SignMessage(privateKey, message)
            val r = signature.copyOfRange(0, 32)
            val s = signature.copyOfRange(32, 64)
            val v = signature.copyOfRange(64, 65)
            return EcdsaRecoverableSignature(r, s, v)
        }

        private external fun bip39Generate(language: String, entropy: Int): MnemonicResult
        private external fun bip39GetLanguages(): Array<String>
        private external fun bip39GetWordsFromLanguage(language: String): Array<String>
        private external fun bip39EncodeBytes(input: ByteBuffer, language: String, entropy: Int): MnemonicResult
        private external fun bip39ValidateMnemonic(mnemonic: String, language: String): Boolean
        private external fun bip39WordsToSeed(mnemonic: String): ByteArray
        private external fun bip39GetEthAddress(privateKey: ByteArray): String
        private external fun bip39SignMessage(privateKey: ByteArray, message: ByteArray): ByteArray
    }
}