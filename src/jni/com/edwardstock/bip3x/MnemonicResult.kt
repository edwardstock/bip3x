package com.edwardstock.bip3x

class MnemonicResult(
    val mnemonic: String,
    val status: Int = NativeBip39.MR_OK
) {
    val len: Int = mnemonic.split("\\s+".toRegex())
        .dropLastWhile { it.isEmpty() }
        .size

    fun size(): Int {
        return len
    }

    val isOk: Boolean
        get() = status == NativeBip39.MR_OK

    fun toSeed(): ByteArray {
        return NativeBip39.mnemonicToBip39Seed(mnemonic)
    }

    fun getStatus(): String {
        return when (status) {
            NativeBip39.MR_OK -> "OK"
            NativeBip39.MR_UNSUPPORTED_ENTROPY -> "Unsupported entropy"
            NativeBip39.MR_UNKNOWN_ERROR -> "Unknown error"
            else -> "Unknown error"
        }
    }
}