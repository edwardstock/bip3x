/*
 * Copyright (C) by MinterTeam. 2018
 * @link <a href="https://github.com/MinterTeam">Org Github</a>
 * @link <a href="https://github.com/edwardstock">Maintainer Github</a>
 *
 * The MIT License
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */

package com.edwardstock.bip3x;

import java.nio.ByteBuffer;
import java.nio.ByteOrder;

import javax.annotation.Nonnull;

import static com.edwardstock.bip3x.Utils.checkNotNull;

/**
 * native-bip39. 2018
 *
 * @author Eduard Maximovich <edward.vstock@gmail.com>
 */
public final class NativeHDKeyEncoder {
    public static final BTCNetwork MAIN_NET = new BTCNetwork(
            "bitcoin",
            "bc",
            new int[]{0x0488b21e, 0x0488ade4},
            (byte) 0x00,
            (byte) 0x05,
            (byte) 0x80
    );

    public static final BTCNetwork TEST_NET = new BTCNetwork(
            "testnet",
            "tb",
            new int[]{0x043587cf, 0x04358394},
            (byte) 0x6f,
            (byte) 0xc4,
            (byte) 0xef
    );

    public static final String MINTER_DERIVATION_PATH = "m/44'/60'/0'/0/0";

    public static HDKey makeBip32RootKey(byte[] seed) {
        return makeBip32RootKey(seed, MAIN_NET);
    }

    public static HDKey makeBip32RootKey(@Nonnull byte[] seed, BTCNetwork network) {
        checkNotNull(seed, "Seed required");
        checkNotNull(network, "Network required");

        ByteBuffer buffer = ByteBuffer.allocateDirect(seed.length);
        buffer.order(ByteOrder.BIG_ENDIAN);
        buffer.rewind();
        buffer.put(seed);

        return encoderMakeBip32RootKey(buffer, network);
    }

    public static HDKey makeExtenderKey(HDKey rootKey) {
        return makeExtenderKey(rootKey, MAIN_NET, MINTER_DERIVATION_PATH);
    }

    public static HDKey makeExtenderKey(HDKey rootKey, BTCNetwork network, String derivationPath) {
        checkNotNull(rootKey, "Root HDKey required");
        checkNotNull(derivationPath, "Derivation path can't be null while getting extended key");

        return encoderMakeExtendedKey(rootKey, network, derivationPath);
    }

    private static native HDKey encoderMakeBip32RootKey(ByteBuffer seed, BTCNetwork net);

    private static native HDKey encoderMakeExtendedKey(HDKey rootKey, BTCNetwork net, String derivationPath);

}
