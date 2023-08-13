#include <bip3x/bip3x_hdkey_encoder.h>
#include <bip3x/bip3x_mnemonic.h>
#include <gtest/gtest.h>

TEST(IssueTest, No11) {
    auto mnemonic = bip3x::bip3x_mnemonic::generate();
    auto root = bip3x::bip3x_hdkey_encoder::make_bip32_root_key(mnemonic.raw);

    bool thrown = false;
    bip3x::derivation_path derivation("m'/44'/60'/0'/0");
    try {
        bip3x::bip3x_hdkey_encoder::make_extended_key(root, derivation);
    } catch (const std::exception& e) {
        thrown = true;
        ASSERT_STREQ(
            "Derivation path has wrong format: only digit segments can be hardened; remove "
            "apostrophe from m'/",
            e.what()
        );
    }

    ASSERT_TRUE(thrown);
}
