package network.minter.mintercore.bip39;

/**
 * MinterWallet. 2018
 *
 * @author Eduard Maximovich <edward.vstock@gmail.com>
 */
public final class BTCNetwork {
    public final String name;
    public final String bech32;
    public int[] bip32;
    public final byte pubKeyHash;
    public final byte scriptHash;
    public final byte wif;

    public BTCNetwork(String name, String bech32, int[] bip32, byte pubKeyHash, byte scriptHash, byte wif) {
        this.name = name;
        this.bech32 = bech32;
        this.bip32 = bip32;
        this.pubKeyHash = pubKeyHash;
        this.scriptHash = scriptHash;
        this.wif = wif;

    }
}
