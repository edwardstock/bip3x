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
import static com.edwardstock.bip3x.Utils.firstNonNull;

/**
 * native-bip39. 2018
 * @author Eduard Maximovich <edward.vstock@gmail.com>
 */
public final class NativeBip39 {

    public static final int ENTROPY_LEN_128 = 16;
    public static final int ENTROPY_LEN_160 = 20;
    public static final int ENTROPY_LEN_192 = 24;
    public static final int ENTROPY_LEN_224 = 28;
    public static final int ENTROPY_LEN_256 = 32;
    public static final int ENTROPY_LEN_288 = 36;
    public static final int ENTROPY_LEN_320 = 40;

    public static final String LANG_DEFAULT = "en";
    public static final int MR_OK = 0;
    public static final int MR_UNSUPPORTED_ENTROPY = 1;
    public static final int MR_UNKNOWN_ERROR = 2;

    public final static String[] SONAME = new String[]{"bip39","bip39_jni"};
    public final static String[] LIB_FILES = new String[]{"libbip39_jni.so", "libbip39.so"};
    private static NativeBip39 INSTANCE;
    private static Throwable sError = null;
    private static ThreadLocal<ByteBuffer> nativeBuffer = new ThreadLocal<>();
    private static boolean sEnabled;

    private NativeBip39(boolean enabled) {
        sEnabled = enabled;
    }

    public static void init() {
        if (INSTANCE == null) {
            boolean enabled;
            try {
                for(String soname: SONAME) {
                    System.loadLibrary(soname);
                }
                enabled = true;
            } catch (UnsatisfiedLinkError e) {
                System.err.println(String.format("Unable to load bip39 library in %s: %s", System.getProperty("java.library.path"), e.getMessage()));
                sError = e;
                enabled = false;
            }

            INSTANCE = new NativeBip39(enabled);
        }
    }

    public static boolean isEnabled() {
        return sEnabled;
    }

    /**
     * Use this carefully, only if you have loaded native libs by yourself
     * @param enabled
     */
    public static void setEnabled(boolean enabled) {
        sEnabled = enabled;
    }

    public static Throwable getError() {
        return sError;
    }

    public static String[] getLanguages() {
        return bip39GetLanguages();
    }

    public static String[] getWordsFromLanguage(@Nonnull String lang) {
        checkNotNull(lang, "Language required");
        return bip39GetWordsFromLanguage(lang);
    }

    public static MnemonicResult encodeBytes(@Nonnull byte[] input) {
        return encodeBytes(input, LANG_DEFAULT, ENTROPY_LEN_128);
    }

    /**
     * Generates random mnemonic with PCGRandom for english language and entropy: 16 bytes
     * @return
     */
    public static MnemonicResult generate() {
        return generate(LANG_DEFAULT, ENTROPY_LEN_128);
    }

    public static MnemonicResult generate(String language, int entropy) {
        return (MnemonicResult) bip39Generate(language, entropy);
    }

    public static MnemonicResult encodeBytes(@Nonnull byte[] input, String language, int entropy) {
        checkNotNull(input, "Input data can't be null");

        ByteBuffer buff = nativeBuffer.get();
        if (buff == null || buff.capacity() < input.length) {
            buff = ByteBuffer.allocateDirect(input.length);
            buff.order(ByteOrder.BIG_ENDIAN);
            nativeBuffer.set(buff);
        }

        buff.rewind();
        buff.put(input);

        return ((MnemonicResult) bip39EncodeBytes(buff, firstNonNull(language, LANG_DEFAULT), entropy));
    }

    public static boolean validateMnemonic(String mnemonic, String language) {
        if (mnemonic == null) {
            return false;
        }

        return bip39ValidateMnemonic(mnemonic, firstNonNull(language, LANG_DEFAULT));
    }

    public static byte[] mnemonicToBip39Seed(String mnemonic) {
        return bip39WordsToSeed(mnemonic);
    }

    private static native Object bip39Generate(String language, int entropy);

    private static native String[] bip39GetLanguages();

    private static native String[] bip39GetWordsFromLanguage(String language);

    private static native Object bip39EncodeBytes(ByteBuffer input, String language, int entropy);

    private static native boolean bip39ValidateMnemonic(String mnemonic, String language);

    private static native byte[] bip39WordsToSeed(String mnemonic);

}
