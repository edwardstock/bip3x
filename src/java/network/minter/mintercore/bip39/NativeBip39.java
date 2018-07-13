package network.minter.mintercore.bip39;

import android.support.annotation.NonNull;

import java.nio.ByteBuffer;
import java.nio.ByteOrder;

import timber.log.Timber;

import static network.minter.mintercore.internal.common.Preconditions.checkNotNull;
import static network.minter.mintercore.internal.common.Preconditions.firstNonNull;

/**
 * MinterWallet. 2018
 *
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

    private final static String SONAME = "bip39_jni";
    private static NativeBip39 INSTANCE;
    private static boolean sEnabled;
    private static Throwable sError = null;
    private static ThreadLocal<ByteBuffer> nativeBuffer = new ThreadLocal<>();

    private NativeBip39() {
    }

    public static void init() {
        if (INSTANCE == null) {
            sEnabled = true;
            try {
                System.loadLibrary(SONAME);
            } catch (UnsatisfiedLinkError e) {
                Timber.e(e, "UnsatisfiedLinkError");
                sError = e;
                sEnabled = false;
            }

            INSTANCE = new NativeBip39();
        }
    }

    public static boolean isEnabled() {
        return sEnabled;
    }

    public static Throwable getError() {
        return sError;
    }

    public static String[] getLanguages() {
        return bip39GetLanguages();
    }

    public static String[] getWordsFromLanguage(@NonNull String lang) {
        checkNotNull(lang, "Language required");
        return bip39GetWordsFromLanguage(lang);
    }

    public static MnemonicResult encodeBytes(@NonNull byte[] input) {
        return encodeBytes(input, null, ENTROPY_LEN_128);
    }

    public static MnemonicResult encodeBytes(@NonNull byte[] input, String language, int entropy) {
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

    private static native String[] bip39GetLanguages();

    private static native String[] bip39GetWordsFromLanguage(String language);

    private static native Object bip39EncodeBytes(ByteBuffer input, String language, int entropy);

    private static native boolean bip39ValidateMnemonic(String mnemonic, String language);

    private static native byte[] bip39WordsToSeed(String mnemonic);

}
