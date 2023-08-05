/**
 * Copyright (c) 2013-2014 Tomas Dzetkulic
 * Copyright (c) 2013-2014 Pavol Rusnak
 *
 * Permission is hereby granted, free of charge, to any person obtaining
 * a copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation
 * the rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included
 * in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
 * OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
 * THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES
 * OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
 * ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
 * OTHER DEALINGS IN THE SOFTWARE.
 */

#ifndef __ECDSA_H__
#define __ECDSA_H__

#include "bignum.h"
#include "bip3x/bip3x_config.h"
#include "hasher.h"
#include "options.h"

#include <cstdint>

// curve point x and y
typedef struct {
	bignum256 x, y;
} curve_point;

typedef struct {

	bignum256 prime;       // prime order of the finite field
	curve_point G;         // initial curve point
	bignum256 order;       // order of G
	bignum256 order_half;  // order of G divided by 2
	int       a;           // coefficient 'a' of the elliptic curve
	bignum256 b;           // coefficient 'b' of the elliptic curve

#if USE_PRECOMPUTED_CP
	const curve_point cp[64][8];
#endif

} ecdsa_curve;

// 4 byte prefix + 40 byte data (segwit)
// 1 byte prefix + 64 byte data (cashaddr)
#define MAX_ADDR_RAW_SIZE 65
// bottle neck is cashaddr
// segwit is at most 90 characters plus NUL separator
// cashaddr: human readable prefix + 1 separator + 104 data + 8 checksum + 1 NUL
// we choose 130 as maximum (including NUL character)
#define MAX_ADDR_SIZE 130
// 4 byte prefix + 32 byte privkey + 1 byte compressed marker
#define MAX_WIF_RAW_SIZE (4 + 32 + 1)
// (4 + 32 + 1 + 4 [checksum]) * 8 / log2(58) plus NUL.
#define MAX_WIF_SIZE (57)

BIP3X_CORE_API void point_copy(const curve_point *cp1, curve_point *cp2);
BIP3X_CORE_API void point_add(const ecdsa_curve *curve, const curve_point *cp1, curve_point *cp2);
BIP3X_CORE_API void point_double(const ecdsa_curve *curve, curve_point *cp);
BIP3X_CORE_API void point_multiply(const ecdsa_curve *curve, const bignum256 *k, const curve_point *p, curve_point *res);
BIP3X_CORE_API void point_set_infinity(curve_point *p);
BIP3X_CORE_API int point_is_infinity(const curve_point *p);
BIP3X_CORE_API int point_is_equal(const curve_point *p, const curve_point *q);
BIP3X_CORE_API int point_is_negative_of(const curve_point *p, const curve_point *q);
BIP3X_CORE_API void scalar_multiply(const ecdsa_curve *curve, const bignum256 *k, curve_point *res);
BIP3X_CORE_API int ecdh_multiply(const ecdsa_curve *curve, const uint8_t *priv_key, const uint8_t *pub_key, uint8_t *session_key);
BIP3X_CORE_API void uncompress_coords(const ecdsa_curve *curve, uint8_t odd, const bignum256 *x, bignum256 *y);
BIP3X_CORE_API int ecdsa_uncompress_pubkey(const ecdsa_curve *curve, const uint8_t *pub_key, uint8_t *uncompressed);

BIP3X_CORE_API int ecdsa_sign(const ecdsa_curve *curve, HasherType hasher_sign, const uint8_t *priv_key, const uint8_t *msg, uint32_t msg_len, uint8_t *sig, uint8_t *pby, int (*is_canonical)(uint8_t by, uint8_t sig[64]));
BIP3X_CORE_API int ecdsa_sign_digest(const ecdsa_curve *curve, const uint8_t *priv_key, const uint8_t *digest, uint8_t *sig, uint8_t *pby, int (*is_canonical)(uint8_t by, uint8_t sig[64]));
BIP3X_CORE_API void ecdsa_get_public_key33(const ecdsa_curve *curve, const uint8_t *priv_key, uint8_t *pub_key);
BIP3X_CORE_API void ecdsa_get_public_key65(const ecdsa_curve *curve, const uint8_t *priv_key, uint8_t *pub_key);
BIP3X_CORE_API void ecdsa_get_pubkeyhash(const uint8_t *pub_key, HasherType hasher_pubkey, uint8_t *pubkeyhash);
BIP3X_CORE_API void ecdsa_get_address_raw(const uint8_t *pub_key, uint32_t version, HasherType hasher_pubkey, uint8_t *addr_raw);
BIP3X_CORE_API void ecdsa_get_address(const uint8_t *pub_key, uint32_t version, HasherType hasher_pubkey, HasherType hasher_base58, char *addr, int addrsize);
BIP3X_CORE_API void ecdsa_get_address_segwit_p2sh_raw(const uint8_t *pub_key, uint32_t version, HasherType hasher_pubkey, uint8_t *addr_raw);
BIP3X_CORE_API void ecdsa_get_address_segwit_p2sh(const uint8_t *pub_key, uint32_t version, HasherType hasher_pubkey, HasherType hasher_base58, char *addr, int addrsize);
BIP3X_CORE_API void ecdsa_get_wif(const uint8_t *priv_key, uint32_t version, HasherType hasher_base58, char *wif, int wifsize);

BIP3X_CORE_API int ecdsa_address_decode(const char *addr, uint32_t version, HasherType hasher_base58, uint8_t *out);
BIP3X_CORE_API int ecdsa_read_pubkey(const ecdsa_curve *curve, const uint8_t *pub_key, curve_point *pub);
BIP3X_CORE_API int ecdsa_validate_pubkey(const ecdsa_curve *curve, const curve_point *pub);
BIP3X_CORE_API int ecdsa_verify(const ecdsa_curve *curve, HasherType hasher_sign, const uint8_t *pub_key, const uint8_t *sig, const uint8_t *msg, uint32_t msg_len);
BIP3X_CORE_API int ecdsa_verify_digest(const ecdsa_curve *curve, const uint8_t *pub_key, const uint8_t *sig, const uint8_t *digest);
BIP3X_CORE_API int ecdsa_verify_digest_recover(const ecdsa_curve *curve, uint8_t *pub_key, const uint8_t *sig, const uint8_t *digest, int recid);
BIP3X_CORE_API int ecdsa_sig_to_der(const uint8_t *sig, uint8_t *der);

#endif
