package com.edwardstock.bip3x

class MnemonicResult(
    val mnemonic: String,
    val status: Int = BIP3X.MR_OK
) {
    val wordsCount: Int = mnemonic.split("\\s+".toRegex())
        .dropLastWhile { it.isEmpty() }
        .size

    val isOk: Boolean
        get() = status == BIP3X.MR_OK

    fun toSeed(): ByteArray {
        return BIP3X.mnemonicToBip39Seed(mnemonic)
    }

    fun getStatus(): String {
        return when (status) {
            BIP3X.MR_OK -> "OK"
            BIP3X.MR_UNSUPPORTED_ENTROPY -> "Unsupported entropy"
            BIP3X.MR_UNKNOWN_ERROR -> "Unknown error"
            else -> "Unknown error"
        }
    }
}