package com.edwardstock.bip3x

class HDKey(
    val depth: Byte = 0,
    val index: Int = 0,
    val fingerprint: Int = 0,
    val publicKeyBytes: ByteArray,
    val privateKeyBytes: ByteArray,
    val chainCodeBytes: ByteArray,
    val extPublicKeyBytes: ByteArray,
    val extPrivateKeyBytes: ByteArray,
) {

    val extPrivateKeyString: String
        get() = bytesToString(extPrivateKeyBytes, 111)
    val extPublicKeyString: String
        get() = bytesToString(extPublicKeyBytes, 111)

    fun clear() {
        clearInternal(publicKeyBytes)
        clearInternal(privateKeyBytes)
        clearInternal(chainCodeBytes)
        clearInternal(extPublicKeyBytes)
        clearInternal(extPrivateKeyBytes)
    }

    fun extend(derivationPath: String, network: BTCNetwork = HDKeyEncoder.MAIN_NET): HDKey {
        return HDKeyEncoder.makeExtendedKey(this, network, derivationPath)
    }

    private fun clearInternal(d: ByteArray) {
        d.fill(0.toByte())
    }

    companion object {
        const val PUB_KEY_LEN = 33
        const val CHAIN_CODE_LEN = 32
        const val PRIV_KEY_LEN = 32
        const val EXT_PUB_KEY_LEN = 112
        const val EXT_PRIV_KEY_LEN = 112

        fun bytesToString(data: ByteArray, readLength: Int): String {
            if (data.size < readLength) {
                throw ArrayIndexOutOfBoundsException("Read length less than array size: " + readLength.toString() + " of " + data.size.toString())
            }
            val out = CharArray(readLength)
            for (i in 0 until readLength) {
                out[i] = Char(data[i].toUShort())
            }
            return String(out)
        }
    }
}