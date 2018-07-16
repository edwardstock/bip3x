package network.minter.mintercore.bip39;

import android.support.annotation.NonNull;

import java.nio.ByteBuffer;
import java.nio.ByteOrder;

import static network.minter.mintercore.internal.common.Preconditions.checkNotNull;

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

    public static HDKey makeBip32RootKey(@NonNull byte[] seed, BTCNetwork network) {
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
