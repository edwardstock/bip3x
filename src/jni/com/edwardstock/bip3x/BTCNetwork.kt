package com.edwardstock.bip3x

class BTCNetwork(
    val name: String,
    val bech32: String,
    val bip32: IntArray,
    val pubKeyHash: Byte,
    val scriptHash: Byte,
    val wif: Byte
)