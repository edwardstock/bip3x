package com.edwardstock.bip3x

import java.nio.ByteBuffer
import java.nio.ByteOrder

object HDKeyEncoder {
    const val ETH_DERIVATION_PATH = "m/44'/60'/0'/0/0"

    val MAIN_NET = BTCNetwork(
        "bitcoin",
        "bc",
        intArrayOf(0x0488b21e, 0x0488ade4),
        0x00.toByte(),
        0x05.toByte(),
        0x80.toByte()
    )
    val TEST_NET = BTCNetwork(
        "testnet",
        "tb",
        intArrayOf(0x043587cf, 0x04358394),
        0x6f.toByte(),
        0xc4.toByte(),
        0xef.toByte()
    )


    fun makeBip32RootKey(seed: ByteArray, network: BTCNetwork = MAIN_NET): HDKey {
        val buffer = ByteBuffer.allocateDirect(seed.size)
        buffer.order(ByteOrder.BIG_ENDIAN)
        buffer.rewind()
        buffer.put(seed)
        return encoderMakeBip32RootKey(buffer, network)
    }

    fun makeExtendedKey(
        rootKey: HDKey,
        network: BTCNetwork = MAIN_NET,
        derivationPath: String = ETH_DERIVATION_PATH
    ): HDKey = encoderMakeExtendedKey(rootKey, network, derivationPath)

    private external fun encoderMakeBip32RootKey(seed: ByteBuffer, net: BTCNetwork): HDKey
    private external fun encoderMakeExtendedKey(rootKey: HDKey, net: BTCNetwork, derivationPath: String): HDKey
}