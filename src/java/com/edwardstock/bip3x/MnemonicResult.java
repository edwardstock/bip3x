package com.edwardstock.bip3x;

/**
 * native-bip39. 2018
 *
 * @author Eduard Maximovich <edward.vstock@gmail.com>
 */
public final class MnemonicResult {
    private int status = NativeBip39.MR_OK;
    private String words;
    private int len;

    public MnemonicResult(final String mnemonicPhrase) {
        if (mnemonicPhrase == null || mnemonicPhrase.isEmpty())
            throw new IllegalArgumentException("Mnemonic phrase can't be null");
        words = mnemonicPhrase;
        len = mnemonicPhrase.split("\\s+").length;
    }

    public String getMnemonic() {
        return words;
    }

    public int size() {
        return len;
    }

    public boolean isOk() {
        return status == NativeBip39.MR_OK;
    }

    public byte[] toSeed() {
        return NativeBip39.mnemonicToBip39Seed(words);
    }

    public String getStatus() {
        switch (status) {
            case NativeBip39.MR_OK:
                return "OK";
            case NativeBip39.MR_UNSUPPORTED_ENTROPY:
                return "Unsupported entropy";
            case NativeBip39.MR_UNKNOWN_ERROR:
            default:
                return "Unknown error";
        }
    }
}
