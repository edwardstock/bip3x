package network.minter.mintercore.bip39;

import network.minter.mintercore.crypto.PrivateKey;
import network.minter.mintercore.crypto.PublicKey;
import network.minter.mintercore.helpers.StringHelper;

/**
 * MinterWallet. 2018
 *
 * @author Eduard Maximovich <edward.vstock@gmail.com>
 */
public final class HDKey {
    public static final int PUB_KEY_LEN = 33;
    public static final int CHAIN_CODE_LEN = 32;
    public static final int PRIV_KEY_LEN = 32;
    public static final int EXT_PUB_KEY_LEN = 112;
    public static final int EXT_PRIV_KEY_LEN = 112;

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
        return StringHelper.bytesToString(getExtPrivateKeyBytes(), 111);
    }

    public PrivateKey getPrivateKey() {
        return new PrivateKey(getPrivateKeyBytes());
    }

    public PublicKey getPublicKey() {
        return new PublicKey(getPublicKeyBytes());
    }

    public String getExtPublicKeyString() {
        return StringHelper.bytesToString(getExtPublicKeyBytes(), 111);
    }

    private void clearInternal(byte[] d) {
        for (int i = 0; i < d.length; i++) {
            d[i] = 0;
        }
    }
}
