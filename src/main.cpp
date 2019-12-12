/**
 * bip39. 2018
 * main.cpp
 *
 * @author Eduard Maximovich <edward.vstock@gmail.com>
 * @link https://github.com/edwardstock
 */

#include "bip3x/Bip39Mnemonic.h"
#include "bip3x/HDKeyEncoder.h"
#include "bip3x/utils.h"

#include <iomanip>
#include <iostream>
#include <sstream>
#include <vector>
using namespace minter;

int main(int argc, char** argv) {
    /*
     * entropy: f0b9c942b9060af6a82d3ac340284d7e
     * words: vague soft expose improve gaze kitten pass point select access battle wish
     * bip39seed: f01e96ba468700a7fa92b8fdf500b8d3cef5cd88e1592a83f31631e9c3f3ed86cffbaba747e2d3f00476b17f3c8b4c19f3f6577cf619464886402ce0faeef01c
     * bip32root key: xprv9s21ZrQH143K2Pr9zz5gPaxJHrJj1YR5As1SA2z6D5a9yTkN9nhUMt2Z1CJxFfSe8VzxmGYeeuVi26Uim7papujvs4hf5dwauQFrqgEU7Nf
     *
     * bip44:
     * network: 36 (ETH)
     * purpose: 44
     * coin: 60
     * accout: 0
     * external/internal: 0/1
     *
     * derivation path: m/44'/60'/0'/0
     * acc ext. private: xprv9zPRRprz3mGyL7YLgAFT1PoJ787ZZroCHxPpVdEhTaxsLh1uowZyX8cGMdbrmibV9bXBNMUtA6TGePGQrw5tWaM4VFFwwqFo52xTL8EXzZH
     * acc ext. public:  xpub6DNmqLPst8qGYbconBnTNXk2f9x3yKX3fBKRJ1eK1vVrDVM4MUtE4vvkCw6N6Zj5YYTQB9G723vkNHaxEA7acuM5J2qH7QSs1ryRJ8Mb8kF
     *
     *
     *
     */

    /*
    Data64 entropy("f0b9c942b9060af6a82d3ac340284d7e");
    Bip39Mnemonic::MnemonicResult
        encodedMnemonic = Bip39Mnemonic::encodeBytes(entropy.data(), "en", BIP39_ENTROPY_LEN_128);
    */

//    Bip39Mnemonic::MnemonicResult encodedMnemonic = Bip39Mnemonic::generate();
//    Bip39Mnemonic::MnemonicResult encodedMnemonic;
//    encodedMnemonic.raw = "crystal enable mask thought ask upgrade custom broccoli green zero wrestle cloth";
//    encodedMnemonic.words = {"crystal", "enable", "mask", "thought","ask", "upgrade", "custom", "broccoli", "green","zero", "wrestle", "cloth"};
//    encodedMnemonic.len = 12;
//    encodedMnemonic.status = Bip39Mnemonic::MnemonicStatus::Ok;
//
//    Data64 seed = HDKeyEncoder::makeBip39Seed(encodedMnemonic.words);
//
//
//    HDKey bip32RootKey = HDKeyEncoder::makeBip32RootKey(seed);
//    HDKey bip32ExtKey = HDKeyEncoder::makeExtendedKey(bip32RootKey, "m/44'/60'/0'/0");
//    HDKey bip44ExtKey = HDKeyEncoder::makeExtendedKey(bip32RootKey, "m/44'/60'/0'");
//
//    std::cout << "Mnemonic words count:    " << encodedMnemonic.len << std::endl;
//    std::cout << "Mnemonic words:          " << encodedMnemonic.raw << std::endl;
//    std::cout << "Seed:                    " << seed.toHex() << std::endl;
//    std::cout << "Bip32 root key:          " << bip32RootKey.extPrivateKey.toString() << std::endl;
//    std::cout << "Bip32 extended priv key: " << bip32ExtKey.extPrivateKey.toString() << std::endl;
//    std::cout << "Bip32 extended pub key:  " << bip32ExtKey.extPublicKey.toString() << std::endl;
//    std::cout << "Bip44 priv key:          " << bip44ExtKey.privateKey.toHex() << std::endl;
//    std::cout << "Bip44 pub key:           " << bip44ExtKey.publicKey.toHex() << std::endl;
//    std::cout << "Bip44 extended priv key: " << bip44ExtKey.extPrivateKey.toString() << std::endl;
//    std::cout << "Bip44 extended pub key:  " << bip44ExtKey.extPublicKey.toString() << std::endl;
//    std::cout << "address:                 " << HDKeyEncoder::getAddress(bip44ExtKey) << std::endl;
//https://github.com/LairdCP/UwTerminalX/wiki/Granting-non-root-USB-device-access-(Linux)
//    bip32RootKey.clear();
//    bip32ExtKey.clear();
//    bip44ExtKey.clear();

//    std::vector<int> tmp;
//    const int n = 10;
//    tmp.resize(n);
//    char** test = new char*[tmp.size()];
//    for (int i = 0; i < n; i++) {
//        test[i] = new char[10];
//    }
//
//    for(int i = 0; i < 10; i++) {
//        sprintf((test[i]), "aaa %d", i);
//    }
//
//    for(int i = 0; i < 10; i++) {
//        std::cout << test[i] << std::endl;
//    }
//
//    std::cout << "\n\n" << std::endl;
//    char** copy = new char*[n];
//    for(int i = 0; i < n; i++) {
//        copy[i] = new char[sizeof(test[i])];
//        memcpy(copy[i], test[i], sizeof(test[i]));
//    }
//
//    for(int i = 0; i < 10; i++) {
//        std::cout << copy[i] << std::endl;
//    }
//
//    for(int i = 0; i < n; i++) {
//        delete [] copy[i];
//        delete [] test[i];
//    }
//    delete [] copy;
//    delete [] test;

    return 0;
}
 
 