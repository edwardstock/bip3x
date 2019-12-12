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

import java.util.Arrays;

/**
 * native-bip39. 2018
 *
 * @author Eduard Maximovich <edward.vstock@gmail.com>
 */
public final class HDKey {
    public static final int PUB_KEY_LEN = 33;
    public static final int CHAIN_CODE_LEN = 32;
    public static final int PRIV_KEY_LEN = 32;
    public static final int EXT_PUB_KEY_LEN = 112;
    public static final int EXT_PRIV_KEY_LEN = 112;

    private byte depth;
    private int index;
    private int fingerprint;

    /**
     * 33 bytes
     */
    private byte[] publicKey;
    /**
     * 32 bytes
     */
    private byte[] privateKey;
    /**
     * 32 bytes
     */
    private byte[] chainCode;
    /**
     * 112 bytes
     */
    private byte[] extPublicKey;
    /**
     * 112 bytes
     */
    private byte[] extPrivateKey;

    @SuppressWarnings("SameParameterValue")
    static String bytesToString(byte[] data, int readLength) {
        if (data.length < readLength) {
            throw new ArrayIndexOutOfBoundsException("Read length less than array size: " + String.valueOf(readLength) + " of " + String.valueOf(data.length));
        }
        final char[] out = new char[readLength];
        for (int i = 0; i < readLength; i++) {
            out[i] = (char) data[i];
        }

        return new String(out);
    }

    public void clear() {
        clearInternal(publicKey);
        clearInternal(privateKey);
        clearInternal(chainCode);
        clearInternal(extPublicKey);
        clearInternal(extPrivateKey);
    }

    public byte[] getPublicKeyBytes() {
        return publicKey;
    }

    public byte[] getPrivateKeyBytes() {
        return privateKey;
    }

    public byte[] getChainCodeBytes() {
        return chainCode;
    }

    public byte[] getExtPublicKeyBytes() {
        return extPublicKey;
    }

    public byte[] getExtPrivateKeyBytes() {
        return extPrivateKey;
    }

    public String getExtPrivateKeyString() {
        return bytesToString(getExtPrivateKeyBytes(), 111);
    }

    public String getExtPublicKeyString() {
        return bytesToString(getExtPublicKeyBytes(), 111);
    }

    private void clearInternal(byte[] d) {
        Arrays.fill(d, (byte)0);
    }
}
