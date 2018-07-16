package network.minter.mintercore.bip39;

import network.minter.mintercore.crypto.PrivateKey;
import network.minter.mintercore.crypto.PublicKey;

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

    public PrivateKey getPrivateKey() {
        return new PrivateKey(getPrivateKeyBytes());
    }

    public PublicKey getPublicKey() {
        return new PublicKey(getPublicKeyBytes());
    }

    public String getExtPublicKeyString() {
        return bytesToString(getExtPublicKeyBytes(), 111);
    }

    private void clearInternal(byte[] d) {
        for (int i = 0; i < d.length; i++) {
            d[i] = 0;
        }
    }
}
