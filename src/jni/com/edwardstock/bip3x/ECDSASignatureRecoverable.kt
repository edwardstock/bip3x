package com.edwardstock.bip3x


data class ECDSASignatureRecoverable(
    val r: ByteArray,
    val s: ByteArray,
    val v: ByteArray
) {
    val signature: ByteArray
        get() = r + s + v

    override fun equals(other: Any?): Boolean {
        if (this === other) return true
        if (other !is ECDSASignatureRecoverable) return false

        if (!r.contentEquals(other.r)) return false
        if (!s.contentEquals(other.s)) return false
        if (!v.contentEquals(other.v)) return false

        return true
    }

    override fun hashCode(): Int {
        var result = r.contentHashCode()
        result = 31 * result + s.contentHashCode()
        result = 31 * result + v.contentHashCode()
        return result
    }

}
