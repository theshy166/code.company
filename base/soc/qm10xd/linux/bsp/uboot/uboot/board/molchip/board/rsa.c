#include "type_use.h"
#include "mci.h"
#include "sstring.h"
#include "pkcs5.h"
#include "oid.h"
#include "rsa.h"
#include "bignum.h"
#include "mc_hw_rsa.h"
#include "hash.h"
#include "sha.h"
#include "usb_drv.h"
#include "uart_drv.h"
#include "spic.h"

extern u32 rsa_hw_sel;
/*
 * Set padding for an existing RSA context
 */
void mbedtls_rsa_set_padding(rsa_context *ctx, int padding,
	int hash_id)
{
	ctx->padding = padding;
	ctx->hash_id = hash_id;
}

/*
 * Initialize an RSA context
 */
void mbedtls_rsa_init(rsa_context *ctx,
	int padding,
	int hash_id)
{
	mmemset(ctx, 0, sizeof(rsa_context));

	mbedtls_rsa_set_padding(ctx, padding, hash_id);
}

/*
 * Do an RSA public key operation
 */
int rsa_public(rsa_context *ctx,
	const unsigned char *input,
	unsigned char *output)
{
	int ret;
	u32 olen;
	mbedtls_mpi T;
	int e_len;
	char *hw_rsa_result = NULL;

	mbedtls_mpi_init(&T);

	MBEDTLS_MPI_CHK(mbedtls_mpi_read_binary(&T, input, ctx->len));

	if (mbedtls_mpi_cmp_mpi(&T, &ctx->N) >= 0) {
		ret = MBEDTLS_ERR_MPI_BAD_INPUT_DATA;
		goto cleanup;
	}

	olen = ctx->len;
	if (rsa_hw_sel & BIT(0)) {
		hw_rsa_result = mbedtls_calloc(sizeof(unsigned long long), 64);
		e_len = mbedtls_mpi_bitlen(&ctx->E);
		hw_rsa_modexp(ctx->E.p, e_len, ctx->N.p, ctx->len << 3,
			(unsigned char *)input, hw_rsa_result, rsa_hw_sel);
		T.s = 1;
		T.n = ctx->N.n + 1;
		T.p = (u32 *)hw_rsa_result;
		MBEDTLS_MPI_CHK(mbedtls_mpi_write_binary(&T, output, olen));
		mbedtls_free(hw_rsa_result);
	} else {
		MBEDTLS_MPI_CHK(mbedtls_mpi_exp_mod(&T, &T, &ctx->E, &ctx->N,
			&ctx->RN));
		MBEDTLS_MPI_CHK(mbedtls_mpi_write_binary(&T, output, olen));
	}

cleanup:

	mbedtls_mpi_free(&T);

	if (ret != 0)
		return (MBEDTLS_ERR_RSA_PUBLIC_FAILED + ret);

	return 0;
}

/*
 * Exponent blinding supposed to prevent side-channel attacks using
 * multiple
 * traces of measurements to recover the RSA key. The more collisions
 * are there,
 * the more bits of the key can be recovered. See [3].
 *
 * Collecting n collisions with m bit long blinding value requires
 * 2^(m-m/n)
 * observations on avarage.
 *
 * For example with 28 byte blinding to achieve 2 collisions the
 * adversary has
 * to make 2^112 observations on avarage.
 *
 * (With the currently (as of 2017 April) known best algorithms
 * breaking 2048
 * bit RSA requires approximately as much time as trying out 2^112
 * random keys.
 * Thus in this sense with 28 byte blinding the security is not
 * reduced by
 * side-channel attacks like the one in [3])
 *
 * This countermeasure does not help if the key recovery is
 * possible with a
 * single trace.
 */
#define RSA_EXPONENT_BLINDING	28

/*
 * Do an RSA private key operation
 */
int rsa_private(rsa_context *ctx,
	 const unsigned char *input,
	 unsigned char *output)
{


	return 0;
}

const unsigned char rand[] = {
0x29, 0x23, 0xbe, 0x84, 0xe1, 0x6c, 0xd6, 0xae, 0x52, 0x90, 0x49, 0xf1,
0xf1, 0xbb,
0xe9, 0xeb, 0xb3, 0xa6, 0xdb, 0x3c, 0x87, 0x0c, 0x3e, 0x99, 0x24, 0x5e,
0x0d, 0x1c,
0x06, 0xb7, 0x47, 0xde, 0xb3, 0x12, 0x4d, 0xc8, 0x43, 0xbb, 0x8b, 0xa6,
0x1f, 0x03,
0x5a, 0x7d, 0x09, 0x38, 0x25, 0x1f, 0x5d, 0xd4, 0xcb, 0xfc, 0x96, 0xf5,
0x45, 0x3b,
0x13, 0x0d, 0x89, 0x0a, 0x1c, 0xdb, 0xae, 0x32, 0x20, 0x9a, 0x50, 0xee,
0x40, 0x78,
0x36, 0xfd, 0x12, 0x49, 0x32, 0xf6, 0x9e, 0x7d, 0x49, 0xdc, 0xad, 0x4f,
0x14, 0xf2,
0x44, 0x40, 0x66, 0xd0, 0x6b, 0xc4, 0x30, 0xb7, 0x32, 0x3b, 0xa1, 0x22,
0xf6, 0x22,
0x91, 0x9d, 0xe1, 0x00,
};

static int myrand(unsigned char *output, u32 len)
{
	u32 i;
	static u8 idx = 0x0;

	for (i = 0; i < len; i++) {
		output[i] = rand[idx];
		idx++;
	}

	return 0;
}

/*
 * Implementation of the PKCS#1 v2.1 RSAES-PKCS1-V1_5-ENCRYPT function
 */
int rsa_rsaes_pkcs1_v15_encrypt(rsa_context *ctx,
	int mode, u32 ilen,
	const unsigned char *input,
	unsigned char *output)
{
	u32 nb_pad, olen;
	int ret;
	unsigned char *p = output;

	if (mode == MBEDTLS_RSA_PRIVATE && ctx->padding !=
		MBEDTLS_RSA_PKCS_V15)
	return MBEDTLS_ERR_RSA_BAD_INPUT_DATA;

	olen = ctx->len;

	/* first comparison checks for overflow */
	if (ilen + 11 < ilen || olen < ilen + 11)
		return MBEDTLS_ERR_RSA_BAD_INPUT_DATA;

	nb_pad = olen - 3 - ilen;

	*p++ = 0;
	if (mode == MBEDTLS_RSA_PUBLIC) {
		*p++ = MBEDTLS_RSA_CRYPT;

		while (nb_pad-- > 0) {
			int rng_dl = 100;

			do {
				ret = myrand(p, 1);
			} while (*p == 0 && --rng_dl && ret == 0);

			/* Check if RNG failed to generate data */
			if (rng_dl == 0 || ret != 0)
				return (MBEDTLS_ERR_RSA_RNG_FAILED + ret);

			p++;
		}
	} else {
		*p++ = MBEDTLS_RSA_SIGN;

		while (nb_pad-- > 0)
			*p++ = 0xFF;
	}

	*p++ = 0;
	mmemcpy(p, input, ilen);

	if (mode == MBEDTLS_RSA_PUBLIC)
		ret = rsa_public(ctx, output, output);
	else
		ret = rsa_private(ctx, output, output);
	return ret;
}

int rsa_pkcs1_encrypt(rsa_context *ctx,
	int mode, u32 ilen,
	const unsigned char *input,
	unsigned char *output)
{
	return rsa_rsaes_pkcs1_v15_encrypt(ctx, mode, ilen,
		input, output);
}

/** Turn zero-or-nonzero into zero-or-all-bits-one, without branches.
 *
 * \param value	The value to analyze.
 * \return	Zero if \p value is zero, otherwise all-bits-one.
 */
static unsigned int all_or_nothing_int(unsigned int value)
{
	return (-((value | -value) >> (sizeof(value) * 8 - 1)));
}

/** Choose between two integer values, without branches.
 *
 * This is equivalent to `cond ? if1 : if0`, but is likely to be compiled
 * to code using bitwise operation rather than a branch.
 *
 * \param cond	   Condition to test.
 * \param if1	   Value to use if \p cond is nonzero.
 * \param if0	   Value to use if \p cond is zero.
 * \return		   \c if1 if \p cond is nonzero, otherwise \c if0.
 */
static unsigned int if_int(unsigned int cond, unsigned int if1,
	unsigned int if0)
{
	unsigned int mask = all_or_nothing_int(cond);

	return ((mask & if1) | (~mask & if0));
}

/** Check whether a size is out of bounds, without branches.
 *
 * This is equivalent to `size > max`, but is likely to be compiled to
 * to code using bitwise operation rather than a branch.
 *
 * \param size      Size to check.
 * \param max       Maximum desired value for \p size.
 * \return          \c 0 if `size <= max`.
 * \return          \c 1 if `size > max`.
 */
static unsigned int size_greater_than(u32 size, u32 max)
{
	/* Return the sign bit (1 for negative) of (max - size). */
	return ((max - size) >> (sizeof(u32) * 8 - 1));
}

/** Shift some data towards the left inside a buffer without leaking
 * the length of the data through side channels.
 *
 * `mem_move_to_left(start, total, offset)` is functionally equivalent to
 * ```
 * memmove(start, start + offset, total - offset);
 * memset(start + offset, 0, total - offset);
 * ```
 * but it strives to use a memory access pattern (and thus total timing)
 * that does not depend on \p offset. This timing independence comes at
 * the expense of performance.
 *
 * \param start     Pointer to the start of the buffer.
 * \param total     Total size of the buffer.
 * \param offset    Offset from which to copy \p total - \p offset bytes.
 */
static void mem_move_to_left(void *start,
	u32 total,
	u32 offset)
{
	volatile unsigned char *buf = start;
	u32 i, n;

	if (total == 0)
		return;
	for (i = 0; i < total; i++) {
		unsigned int no_op = size_greater_than(total - offset, i);
	/* The first `total - offset` passes are a no-op. The last
	 * `offset` passes shift the data one byte to the left and
	 * zero out the last byte.
	 */
		for (n = 0; n < total - 1; n++) {
			unsigned char current = buf[n];
			unsigned char next = buf[n+1];

			buf[n] = if_int(no_op, current, next);
		}
		buf[total-1] = if_int(no_op, buf[total-1], 0);
	}
}

/*
 * Implementation of the PKCS#1 v2.1 RSAES-PKCS1-V1_5-DECRYPT function
 */
int rsa_rsaes_pkcs1_v15_decrypt(rsa_context *ctx,
	int mode, u32 *olen,
	const unsigned char *input,
	unsigned char *output,
	u32 output_max_len)
{
	int ret;
	u32 ilen, i, plaintext_max_size;
	unsigned char buf[MBEDTLS_MPI_MAX_SIZE];
	/* The following variables take sensitive values: their value must
	 * not leak into the observable behavior of the function other than
	 * the designated outputs (output, olen, return value). Otherwise
	 * this would open the execution of the function to
	 * side-channel-based variants of the Bleichenbacher padding oracle
	 * attack. Potential side channels include overall timing, memory
	 * access patterns (especially visible to an adversary who has access
	 * to a shared memory cache), and branches (especially visible to
	 * an adversary who has access to a shared code cache or to a shared
	 * branch predictor).
	 */
	u32 pad_count = 0;
	unsigned int bad = 0;
	unsigned char pad_done = 0;
	u32 plaintext_size = 0;
	unsigned int output_too_large;

	ilen = ctx->len;
	plaintext_max_size = (output_max_len > ilen - 11 ?
		ilen - 11 : output_max_len);

	if (mode == MBEDTLS_RSA_PRIVATE && ctx->padding !=
		MBEDTLS_RSA_PKCS_V15)
		return MBEDTLS_ERR_RSA_BAD_INPUT_DATA;

	if (ilen < 16 || ilen > sizeof(buf))
		return MBEDTLS_ERR_RSA_BAD_INPUT_DATA;

	ret = (mode == MBEDTLS_RSA_PUBLIC)
		 ? rsa_public(ctx, input, buf)
		 : rsa_private(ctx, input, buf);

	if (ret != 0)
		goto cleanup;

	/* Check and get padding length in constant time and constant
	 * memory trace. The first byte must be 0.
	 */
	bad |= buf[0];

	if (mode == MBEDTLS_RSA_PRIVATE) {
	/* Decode EME-PKCS1-v1_5 padding: 0x00 || 0x02 || PS || 0x00
	 * where PS must be at least 8 nonzero bytes.
	 */
		bad |= buf[1] ^ MBEDTLS_RSA_CRYPT;

	/* Read the whole buffer. Set pad_done to nonzero if we find
	 * the 0x00 byte and remember the padding length in pad_count.
	 */
		for (i = 2; i < ilen; i++) {
			pad_done  |= ((buf[i] | (unsigned char)
				-buf[i]) >> 7) ^ 1;
			pad_count += ((pad_done | (unsigned char)
				-pad_done) >> 7) ^ 1;
		}
	} else {
	/* Decode EMSA-PKCS1-v1_5 padding: 0x00 || 0x01 || PS || 0x00
	 * where PS must be at least 8 bytes with the value 0xFF.
	 */
		bad |= buf[1] ^ MBEDTLS_RSA_SIGN;

	/* Read the whole buffer. Set pad_done to nonzero if we find
	 * the 0x00 byte and remember the padding length in pad_count.
	 * If there's a non-0xff byte in the padding, the padding is bad.
	 */
		for (i = 2; i < ilen; i++) {
			pad_done |= if_int(buf[i], 0, 1);
			pad_count += if_int(pad_done, 0, 1);
			bad |= if_int(pad_done, 0, buf[i] ^ 0xFF);
		}
	}

	/* If pad_done is still zero, there's no data,
	 *only unfinished padding.
	 */
	bad |= if_int(pad_done, 0, 1);

	/* There must be at least 8 bytes of padding.
	 */
	bad |= size_greater_than(8, pad_count);

	/* If the padding is valid, set plaintext_size to the number of
	 * remaining bytes after stripping the padding. If the padding
	 * is invalid, avoid leaking this fact through the size of the
	 * output: use the maximum message size that fits in the output
	 * buffer. Do it without branches to avoid leaking the padding
	 * validity through timing. RSA keys are small enough that all the
	 * size_t values involved fit in unsigned int.
	 */
	plaintext_size = if_int(bad,
		(unsigned int) plaintext_max_size,
		(unsigned int) (ilen - pad_count - 3));

	/* Set output_too_large to 0 if the plaintext fits in the output
	 * buffer and to 1 otherwise.
	 */
	output_too_large = size_greater_than(plaintext_size,
		plaintext_max_size);

	/* Set ret without branches to avoid timing attacks. Return:
	 * - INVALID_PADDING if the padding is bad (bad != 0).
	 * - OUTPUT_TOO_LARGE if the padding is good but the decrypted
	 *	plaintext does not fit in the output buffer.
	 * - 0 if the padding is correct.
	 */
	ret = -(int) if_int(bad, -MBEDTLS_ERR_RSA_INVALID_PADDING,
		if_int(output_too_large, -MBEDTLS_ERR_RSA_OUTPUT_TOO_LARGE, 0));

	/* If the padding is bad or the plaintext is too large, zero the
	 * data that we're about to copy to the output buffer.
	 * We need to copy the same amount of data
	 * from the same buffer whether the padding is good or not to
	 * avoid leaking the padding validity through overall timing or
	 * through memory or cache access patterns.
	 */
	bad = all_or_nothing_int(bad | output_too_large);
	for (i = 11; i < ilen; i++)
		buf[i] &= ~bad;

	/* If the plaintext is too large, truncate it to the buffer size.
	 * Copy anyway to avoid revealing the length through timing, because
	 * revealing the length is as bad as revealing the padding validity
	 * for a Bleichenbacher attack.
	 */
	plaintext_size = if_int(output_too_large,
		(unsigned int) plaintext_max_size,
		(unsigned int) plaintext_size);

	/* Move the plaintext to the leftmost position where it can start in
	 * the working buffer, i.e. make it start plaintext_max_size from
	 * the end of the buffer. Do this with a memory access trace that
	 * does not depend on the plaintext size. After this move, the
	 * starting location of the plaintext is no longer sensitive
	 * information.
	 */
	mem_move_to_left(buf + ilen - plaintext_max_size,
		plaintext_max_size,
		plaintext_max_size - plaintext_size);

	/* Finally copy the decrypted plaintext plus trailing zeros
	 * into the output buffer.
	 */
	mmemcpy(output, buf + ilen - plaintext_max_size, plaintext_max_size);

	/* Report the amount of data we copied to the output buffer. In case
	 * of errors (bad padding or output too large), the value of *olen
	 * when this function returns is not specified. Making it equivalent
	 * to the good case limits the risks of leaking the padding validity.
	 */
	*olen = plaintext_size;

cleanup:
	mmemset(buf, 0, sizeof(buf));

	return ret;
}

/*
 * Do an RSA operation, then remove the message padding
 */
int rsa_pkcs1_decrypt(rsa_context *ctx,
	int mode, u32 *olen,
	const unsigned char *input,
	unsigned char *output,
	u32 output_max_len)
{
	return rsa_rsaes_pkcs1_v15_decrypt(ctx, mode, olen,
		input, output, output_max_len);
}

/* constant-time buffer comparison */
static int safer_memcmp(const void *a, const void *b, unsigned int n)
{
	unsigned int i;
	const unsigned char *A = (const unsigned char *) a;
	const unsigned char *B = (const unsigned char *) b;
	unsigned char diff = 0;

	for (i = 0; i < n; i++)
		diff |= A[i] ^ B[i];

	return diff;
}

/*
 * For digestAlgorithm
 */
typedef struct {
	mbedtls_oid_descriptor_t descriptor;
	md_type_t md_alg;
} oid_md_alg_t;

static const oid_md_alg_t oid_md_alg[] = {
	{
		{
			ADD_LEN(MBEDTLS_OID_DIGEST_ALG_SHA256),
			"id-sha256",
			"SHA-256"
		},
		MBEDTLS_MD_SHA256,
	},
	{
		{
			NULL,
			0,
			NULL,
			NULL
		},
		MBEDTLS_MD_NONE,
	},
};

/*
 * Macro to generate a function for retrieving the OID based on a single
 * attribute from a mbedtls_oid_descriptor_t wrapper.
 */
#define FN_OID_GET_OID_BY_ATTR1(FN_NAME, TYPE, LIST, ATTR1_TYPE, ATTR1) \
int FN_NAME(ATTR1_TYPE ATTR1, const char **oid, unsigned int *olen)	\
{									\
	const TYPE *cur = LIST;					\
	while (cur->descriptor.asn1 != NULL) {			\
		if (cur->ATTR1 == ATTR1) {				\
			*oid = cur->descriptor.asn1;			\
			*olen = cur->descriptor.asn1_len;		\
			return 0;					\
		}							\
		cur++;							\
	}								\
	return MBEDTLS_ERR_OID_NOT_FOUND;				\
}

FN_OID_GET_OID_BY_ATTR1(oid_get_oid_by_md, oid_md_alg_t,
	oid_md_alg, md_type_t, md_alg)

/*
 * Implementation of the PKCS#1 v2.1 RSASSA-PKCS1-V1_5-SIGN function
 */

/* Construct a PKCS v1.5 encoding of a hashed message
 *
 * This is used both for signature generation and verification.
 *
 * Parameters:
 * - md_alg: Identifies the hash algorithm used to generate the given hash;
 *			 MBEDTLS_MD_NONE if raw data is signed.
 * - hashlen: Length of hash in case hashlen is MBEDTLS_MD_NONE.
 * - hash:	 Buffer containing the hashed message or the raw data.
 * - dst_len: Length of the encoded message.
 * - dst:	 Buffer to hold the encoded message.
 *
 * Assumptions:
 * - hash has size hashlen if md_alg == MBEDTLS_MD_NONE.
 * - hash has size corresponding to md_alg if md_alg != MBEDTLS_MD_NONE.
 * - dst points to a buffer of size at least dst_len.
 *
 */
static int rsa_rsassa_pkcs1_v15_encode(md_type_t md_alg,
	unsigned int hashlen,
	const unsigned char *hash,
	unsigned int dst_len,
	unsigned char *dst)
{
	unsigned int oid_size = 0;
	unsigned int nb_pad = dst_len;
	unsigned char *p = dst;
	const char *oid = NULL;

	/* Are we signing hashed or raw data? */
	if (md_alg != MBEDTLS_MD_NONE) {
		const mbedtls_md_info_t *md_info = md_info_from_type(md_alg);

		if (md_info == NULL)
			return MBEDTLS_ERR_RSA_BAD_INPUT_DATA;

		if (oid_get_oid_by_md(md_alg, &oid, &oid_size) != 0)
			return MBEDTLS_ERR_RSA_BAD_INPUT_DATA;

		hashlen = mbedtls_md_get_size(md_info);

		/* Double-check that 8 + hashlen + oid_size can be used as a
		 * 1-byte ASN.1 length encoding and that there's no overflow.
		 */
		if (8 + hashlen + oid_size >= 0x80 ||
			10 + hashlen < hashlen ||
			10 + hashlen + oid_size < 10 + hashlen)
			return MBEDTLS_ERR_RSA_BAD_INPUT_DATA;

		/*
		 * Static bounds check:
		 * - Need 10 bytes for five tag-length pairs.
		 * (Insist on 1-byte length encodings to protect against
		 * variants of Bleichenbacher's forgery attack against
		 * lax PKCS#1v1.5 verification)
		 * - Need hashlen bytes for hash
		 * - Need oid_size bytes for hash alg OID.
		 */
		if (nb_pad < 10 + hashlen + oid_size)
			return MBEDTLS_ERR_RSA_BAD_INPUT_DATA;
		nb_pad -= 10 + hashlen + oid_size;
	} else {
		if (nb_pad < hashlen)
			return MBEDTLS_ERR_RSA_BAD_INPUT_DATA;

		nb_pad -= hashlen;
	}

	/* Need space for signature header and padding delimiter (3 bytes),
	 * and 8 bytes for the minimal padding
	 */
	if (nb_pad < 3 + 8)
		return MBEDTLS_ERR_RSA_BAD_INPUT_DATA;
	nb_pad -= 3;

	/* Now nb_pad is the amount of memory to be filled
	 * with padding, and at least 8 bytes long.
	 */

	/* Write signature header and padding */
	*p++ = 0;
	*p++ = MBEDTLS_RSA_SIGN;
	mmemset(p, 0xFF, nb_pad);
	p += nb_pad;
	*p++ = 0;

	/* Are we signing raw data? */
	if (md_alg == MBEDTLS_MD_NONE) {
		mmemcpy(p, hash, hashlen);
		return 0;
	}

	/* Signing hashed data, add corresponding ASN.1 structure
	 *
	 * DigestInfo ::= SEQUENCE {
	 *	digestAlgorithm DigestAlgorithmIdentifier,
	 *	digest Digest }
	 * DigestAlgorithmIdentifier ::= AlgorithmIdentifier
	 * Digest ::= OCTET STRING
	 *
	 * Schematic:
	 * TAG-SEQ + LEN [TAG-SEQ + LEN [TAG-OID + LEN [OID]
	 * TAG-NULL + LEN [NULL]]
	 * TAG-OCTET + LEN [HASH]]
	 */
	*p++ = MBEDTLS_ASN1_SEQUENCE | MBEDTLS_ASN1_CONSTRUCTED;
	*p++ = (unsigned char)(0x08 + oid_size + hashlen);
	*p++ = MBEDTLS_ASN1_SEQUENCE | MBEDTLS_ASN1_CONSTRUCTED;
	*p++ = (unsigned char)(0x04 + oid_size);
	*p++ = MBEDTLS_ASN1_OID;
	*p++ = (unsigned char) oid_size;
	mmemcpy(p, oid, oid_size);
	p += oid_size;
	*p++ = MBEDTLS_ASN1_NULL;
	*p++ = 0x00;
	*p++ = MBEDTLS_ASN1_OCTET_STRING;
	*p++ = (unsigned char) hashlen;
	mmemcpy(p, hash, hashlen);
	p += hashlen;

	/* Just a sanity-check, should be automatic
	 * after the initial bounds check.
	 */
	if (p != dst + dst_len) {
		mmemset(dst, 0, dst_len);
		return MBEDTLS_ERR_RSA_BAD_INPUT_DATA;
	}

	return 0;
}

/*
 * Do an RSA operation to sign the message digest
 */
int rsa_rsassa_pkcs1_v15_sign(rsa_context *ctx,
	 int mode,
	 md_type_t md_alg,
	 unsigned int hashlen,
	 const unsigned char *hash,
	 unsigned char *sig)
{
	int ret;
	unsigned char *sig_try = NULL, *verif = NULL;


	if (mode == MBEDTLS_RSA_PRIVATE && ctx->padding != MBEDTLS_RSA_PKCS_V15)
		return MBEDTLS_ERR_RSA_BAD_INPUT_DATA;

	/*
	 * Prepare PKCS1-v1.5 encoding (padding and hash identifier)
	 */
	ret = rsa_rsassa_pkcs1_v15_encode(md_alg, hashlen, hash,
				ctx->len, sig);

	if (ret != 0)
		return ret;

	/*
	 * Call respective RSA primitive
	 */

	if (mode == MBEDTLS_RSA_PUBLIC) {
		/* Skip verification on a public key operation */
		return rsa_public(ctx, sig, sig);
	}

	/* Private key operation
	 *
	 * In order to prevent Lenstra's attack, make the signature in a
	 * temporary buffer and check it before returning it.
	 */

	sig_try = mbedtls_calloc(1, ctx->len);
	if (sig_try == NULL)
		return MBEDTLS_ERR_MPI_ALLOC_FAILED;

	verif = mbedtls_calloc(1, ctx->len);
	if (verif == NULL) {
		mbedtls_free(sig_try);
		return MBEDTLS_ERR_MPI_ALLOC_FAILED;
	}

	MBEDTLS_MPI_CHK(rsa_private(ctx, sig, sig_try));
	MBEDTLS_MPI_CHK(rsa_public(ctx, sig_try, verif));

	if (safer_memcmp(verif, sig, ctx->len) != 0) {
		ret = MBEDTLS_ERR_RSA_PRIVATE_FAILED;
		goto cleanup;
	}

	mmemcpy(sig, sig_try, ctx->len);

cleanup:
	mbedtls_free(sig_try);
	mbedtls_free(verif);

	return ret;
}

/*
 * Do an RSA operation to sign the message digest
 */
int rsa_pkcs1_sign(rsa_context *ctx,
	 const unsigned char *hash,
	 unsigned char *sig)
{
	switch (ctx->padding) {
	case MBEDTLS_RSA_PKCS_V15:
		return rsa_rsassa_pkcs1_v15_sign(ctx, MBEDTLS_RSA_PRIVATE,
		 MBEDTLS_MD_SHA1,
		 20, hash, sig);

	default:
		return MBEDTLS_ERR_RSA_INVALID_PADDING;
	}
}

/*
 * Implementation of the PKCS#1 v2.1 RSASSA-PKCS1-v1_5-VERIFY function
 */
int rsa_rsassa_pkcs1_v15_verify(rsa_context *ctx,
	 int mode,
	 md_type_t md_alg,
	 unsigned int hashlen,
	 const unsigned char *hash,
	 const unsigned char *sig)
{
	int ret = 0;
	unsigned int sig_len;
	unsigned char *encoded = NULL, *encoded_expected = NULL;

	sig_len = ctx->len;

	if (mode == MBEDTLS_RSA_PRIVATE && ctx->padding != MBEDTLS_RSA_PKCS_V15)
		return MBEDTLS_ERR_RSA_BAD_INPUT_DATA;

	/*
	 * Prepare expected PKCS1 v1.5 encoding of hash.
	 */
	encoded = mbedtls_calloc(1, sig_len);
	encoded_expected = mbedtls_calloc(1, sig_len);

	if ((encoded == NULL) || (encoded_expected == NULL)) {
		ret = MBEDTLS_ERR_MPI_ALLOC_FAILED;
		goto cleanup;
	}

	ret = rsa_rsassa_pkcs1_v15_encode(md_alg, hashlen, hash, sig_len,
			encoded_expected);

	if (ret != 0)
		goto cleanup;

	/*
	 * Apply RSA primitive to get what should be PKCS1 encoded hash.
	 */

	ret = (mode == MBEDTLS_RSA_PUBLIC)
		 ? rsa_public(ctx, sig, encoded)
		 : rsa_private(ctx, sig, encoded);
	if (ret != 0)
		goto cleanup;

	/*
	 * Compare
	 */
	ret = safer_memcmp(encoded, encoded_expected, sig_len);

	if (ret != 0) {
		ret = MBEDTLS_ERR_RSA_VERIFY_FAILED;
		goto cleanup;
	}

cleanup:

	if (encoded != NULL) {
		mmemset(encoded, 0, sig_len);
		mbedtls_free(encoded);
	}

	if (encoded_expected != NULL) {
		mmemset(encoded_expected, 0, sig_len);
		mbedtls_free(encoded_expected);
	}

	return ret;
}

/*
 * Do an RSA operation and check the message digest
 */
int rsa_pkcs1_verify(rsa_context *ctx,
	 const unsigned char *hash,
	 const unsigned char *sig, struct boot_header *header)
{
	switch (ctx->padding) {
	case MBEDTLS_RSA_PKCS_V15:
		return rsa_rsassa_pkcs1_v15_verify(ctx, MBEDTLS_RSA_PUBLIC,
			MBEDTLS_MD_SHA256, 0, hash, sig);
	case MBEDTLS_RSA_PKCS_V21:
		return mbedtls_rsa_rsassa_pss_verify(ctx, MBEDTLS_RSA_PUBLIC,
			MBEDTLS_MD_SHA256, 0, hash, sig,header);
	default:
		return MBEDTLS_ERR_RSA_INVALID_PADDING;
	}
}

int mbedtls_rsa_import(rsa_context *ctx,
	 const mbedtls_mpi *N,
	 const mbedtls_mpi *P, const mbedtls_mpi *Q,
	 const mbedtls_mpi *D, const mbedtls_mpi *E)
{
	int ret;

	if (((N != PNULL) && (ret = mbedtls_mpi_copy(&ctx->N, N)) != 0) ||
		((P != PNULL) && (ret = mbedtls_mpi_copy(&ctx->P, P)) != 0) ||
		((Q != PNULL) && (ret = mbedtls_mpi_copy(&ctx->Q, Q)) != 0) ||
		((D != PNULL) && (ret = mbedtls_mpi_copy(&ctx->D, D)) != 0) ||
		((E != PNULL) && (ret = mbedtls_mpi_copy(&ctx->E, E)) != 0))
		return MBEDTLS_ERR_RSA_BAD_INPUT_DATA + ret;

	if (N != PNULL)
		ctx->len = mbedtls_mpi_size(&ctx->N);

	return 0;
}

/**
 * Generate and apply the MGF1 operation (from PKCS#1 v2.1) to a buffer.
 *
 * \param dst	   buffer to mask
 * \param dlen	   length of destination buffer
 * \param src	   source of the mask generation
 * \param slen	   length of the source buffer
 * \param md_ctx    message digest context to use
 */
static int mgf_mask(unsigned char *dst, u32 dlen, unsigned char *src,
	 u32 slen, mbedtls_md_context_t *md_ctx)
{
	unsigned char mask[MBEDTLS_MD_MAX_SIZE];
	unsigned char counter[4];
	unsigned char *p;
	unsigned int hlen;
	u32 i, use_len;
	int ret = 0;

	mmemset(mask, 0, MBEDTLS_MD_MAX_SIZE);
	mmemset(counter, 0, 4);

	hlen = mbedtls_md_get_size(md_ctx->md_info);

	/* Generate and apply dbMask */
	p = dst;

	while (dlen > 0) {
		use_len = hlen;
		if (dlen < hlen)
			use_len = dlen;

		ret = mbedtls_md_starts(md_ctx);
		if (ret != 0)
			goto exit;
		ret = mbedtls_md_update(md_ctx, src, slen);
		if (ret != 0)
			goto exit;
		ret = mbedtls_md_update(md_ctx, counter, 4);
		if (ret != 0)
			goto exit;
		ret = mbedtls_md_finish(md_ctx, mask);
		if (ret != 0)
			goto exit;

		for (i = 0; i < use_len; ++i)
			*p++ ^= mask[i];

		counter[3]++;

		dlen -= use_len;
	}

exit:
	mmemset(mask, 0, sizeof(mask));

	return ret;
}

int mbedtls_rsa_rsassa_pss_sign(rsa_context *ctx,
	int mode,
	md_type_t md_alg,
	unsigned int hashlen,
	const unsigned char *hash,
	unsigned char *sig)
{
	u32 olen;
	unsigned char *p = sig;
	unsigned char salt[MBEDTLS_MD_MAX_SIZE];
	u32 slen, min_slen, hlen, offset = 0;
	int ret;
	u32 msb;
	const mbedtls_md_info_t *md_info;
	mbedtls_md_context_t md_ctx;

	if (mode == MBEDTLS_RSA_PRIVATE && ctx->padding != MBEDTLS_RSA_PKCS_V21)
		return MBEDTLS_ERR_RSA_BAD_INPUT_DATA;

	olen = ctx->len;

	if (md_alg != MBEDTLS_MD_NONE) {
		/* Gather length of hash to sign */
		md_info = md_info_from_type(md_alg);
		if (md_info == NULL)
			return MBEDTLS_ERR_RSA_BAD_INPUT_DATA;

		hashlen = mbedtls_md_get_size(md_info);
	}

	md_info = md_info_from_type((md_type_t) ctx->hash_id);
	if (md_info == NULL)
		return MBEDTLS_ERR_RSA_BAD_INPUT_DATA;

	hlen = mbedtls_md_get_size(md_info);

/* Calculate the largest possible salt length. Normally this is the hash
 * length, which is the maximum length the salt can have. If there is not
 * enough room, use the maximum salt length that fits. The constraint is
 * that the hash length plus the salt length plus 2 bytes must be at most
 * the key length. This complies with FIPS 186-4 ¡ì5.5 (e) and RFC 8017
 * (PKCS#1 v2.2) ¡ì9.1.1 step 3.
 */
	min_slen = hlen - 2;
	if (olen < hlen + min_slen + 2)
		return MBEDTLS_ERR_RSA_BAD_INPUT_DATA;
	else if (olen >= hlen + hlen + 2)
		slen = hlen;
	else
		slen = olen - hlen - 2;

	mmemset(sig, 0, olen);

	/* Generate salt of length slen */
	ret = myrand(salt, slen);
	if (ret != 0)
		return (MBEDTLS_ERR_RSA_RNG_FAILED + ret);

	/* Note: EMSA-PSS encoding is over the length of N - 1 bits */
	msb = mbedtls_mpi_bitlen(&ctx->N) - 1;
	p += olen - hlen - slen - 2;
	*p++ = 0x01;
	mmemcpy(p, salt, slen);
	p += slen;

	mbedtls_md_init(&md_ctx);
	ret = mbedtls_md_setup(&md_ctx, md_info, 0);
	if (ret != 0)
		goto exit;

	/* Generate H = Hash( M' ) */
	ret = mbedtls_md_starts(&md_ctx);
	if (ret != 0)
		goto exit;
	ret = mbedtls_md_update(&md_ctx, p, 8);
	if (ret != 0)
		goto exit;
	ret = mbedtls_md_update(&md_ctx, hash, hashlen);
	if (ret != 0)
		goto exit;
	ret = mbedtls_md_update(&md_ctx, salt, slen);
	if (ret != 0)
		goto exit;
	ret = mbedtls_md_finish(&md_ctx, p);
	if (ret != 0)
		goto exit;

	/* Compensate for boundary condition when applying mask */
	if (msb % 8 == 0)
		offset = 1;

	/* maskedDB: Apply dbMask to DB */
	ret = mgf_mask(sig + offset, olen - hlen - 1 - offset, p, hlen,
		 &md_ctx);
	if (ret != 0)
		goto exit;

	msb = mbedtls_mpi_bitlen(&ctx->N) - 1;
	sig[0] &= 0xFF >> (olen * 8 - msb);

	p += hlen;
	*p++ = 0xBC;

	mmemset(salt, 0, sizeof(salt));

exit:
	mbedtls_md_free(&md_ctx);

	if (ret != 0)
		return ret;

	return ((mode == MBEDTLS_RSA_PUBLIC)
		 ? rsa_public(ctx, sig, sig)
		 : rsa_private(ctx, sig, sig));
}

int mbedtls_rsa_rsassa_pss_verify_ext(rsa_context *ctx,
	 int mode,
	 md_type_t md_alg,
	 unsigned int hashlen,
	 const unsigned char *hash,
	 md_type_t mgf1_hash_id,
	 int expected_salt_len,
	 const unsigned char *sig,struct boot_header *header)
{
	int ret;

	u8 corr_hash[32] = {0};
	u32 siglen;
	unsigned char *p;
	unsigned char *hash_start;
	unsigned char result[MBEDTLS_MD_MAX_SIZE];
	unsigned char zeros[8];
	unsigned int hlen;
	u32 observed_salt_len, msb;
	const mbedtls_md_info_t *md_info;
	mbedtls_md_context_t md_ctx;
	unsigned char buf[MBEDTLS_MPI_MAX_SIZE];

	if (mode == MBEDTLS_RSA_PRIVATE && ctx->padding != MBEDTLS_RSA_PKCS_V21)
		return MBEDTLS_ERR_RSA_BAD_INPUT_DATA;

	siglen = ctx->len;

	if (siglen < 16 || siglen > sizeof(buf))
		return MBEDTLS_ERR_RSA_BAD_INPUT_DATA;

	ret = (mode == MBEDTLS_RSA_PUBLIC)
		 ? rsa_public(ctx, sig, buf)
		 : rsa_private(ctx, sig, buf);

	if (ret != 0)
		return ret;

	p = buf;

	if (buf[siglen - 1] != 0xBC)
		return MBEDTLS_ERR_RSA_INVALID_PADDING;

	if (md_alg != MBEDTLS_MD_NONE) {
		/* Gather length of hash to sign */
		md_info = md_info_from_type(md_alg);
		if (md_info == NULL)
			return MBEDTLS_ERR_RSA_BAD_INPUT_DATA;

		hashlen = mbedtls_md_get_size(md_info);
	}

	md_info = md_info_from_type(mgf1_hash_id);
	if (md_info == NULL)
		return MBEDTLS_ERR_RSA_BAD_INPUT_DATA;

	hlen = mbedtls_md_get_size(md_info);

	mmemset(zeros, 0, 8);

	/*
	 * Note: EMSA-PSS verification is over the length of N - 1 bits
	 */
	msb = mbedtls_mpi_bitlen(&ctx->N) - 1;

	if (buf[0] >> (8 - siglen * 8 + msb))
		return MBEDTLS_ERR_RSA_BAD_INPUT_DATA;

	/* Compensate for boundary condition when applying mask */
	if (msb % 8 == 0) {
		p++;
		siglen -= 1;
	}

	if (siglen < hlen + 2)
		return MBEDTLS_ERR_RSA_BAD_INPUT_DATA;
	hash_start = p + siglen - hlen - 1;

	mbedtls_md_init(&md_ctx);
	ret = mbedtls_md_setup(&md_ctx, md_info, 0);
	if (ret != 0)
		goto exit;

	ret = mgf_mask(p, siglen - hlen - 1, hash_start, hlen, &md_ctx);
	if (ret != 0)
		goto exit;

	buf[0] &= 0xFF >> (siglen * 8 - msb);

	while (p < hash_start - 1 && *p == 0)
		p++;

	if (*p++ != 0x01) {
		ret = MBEDTLS_ERR_RSA_INVALID_PADDING;
		goto exit;
	}

	observed_salt_len = hash_start - p;

	if (expected_salt_len != MBEDTLS_RSA_SALT_LEN_ANY &&
		observed_salt_len != (u32) expected_salt_len) {
		ret = MBEDTLS_ERR_RSA_INVALID_PADDING;
		goto exit;
	}

	/*
	 * Generate H = Hash( M' )
	 */
	ret = mbedtls_md_starts(&md_ctx);
	if (ret != 0)
		goto exit;
	ret = mbedtls_md_update(&md_ctx, zeros, 8);
	if (ret != 0)
		goto exit;

if (header->sel & BIT(31)){


	if(header->rev1){
		/* wait cpu1 schedule done. */
		ret = spi_cpu_sched_finish();
		if (ret) {
			goto exit;
		}
	}
	
	if (HASH_GLITCH_EXIST == header->rev0) {
		sha256_ctx_end(corr_hash);
	} else {
		read_result((u32 *)corr_hash, HASH_SHA256);
	}
	ret = mbedtls_md_update(&md_ctx, corr_hash, hashlen);
}else{
	ret = mbedtls_md_update(&md_ctx, hash, hashlen);

}

	if (ret != 0)
		goto exit;
	ret = mbedtls_md_update(&md_ctx, p, observed_salt_len);

	if (ret != 0)
		goto exit;
	ret = mbedtls_md_finish(&md_ctx, result);
	if (ret != 0)
		goto exit;


	if (mmemcmp(hash_start, result, hlen) != 0) {
		ret = MBEDTLS_ERR_RSA_VERIFY_FAILED;
		goto exit;
	}
	//timestamp_record(6);

	uart_puts("RVS\r\n");
	
exit:
	mbedtls_md_free(&md_ctx);

	return ret;
}

/*
 * Simplified PKCS#1 v2.1 RSASSA-PSS-VERIFY function
 */
int mbedtls_rsa_rsassa_pss_verify(rsa_context *ctx,
	 int mode,
	 md_type_t md_alg,
	 unsigned int hashlen,
	 const unsigned char *hash,
	 const unsigned char *sig,struct boot_header *header)
{
	md_type_t mgf1_hash_id;

	mgf1_hash_id = (ctx->hash_id != MBEDTLS_MD_NONE)
		 ? (md_type_t) ctx->hash_id
		 : md_alg;

	return mbedtls_rsa_rsassa_pss_verify_ext(ctx, mode,
		 md_alg, hashlen, hash,
		 mgf1_hash_id, MBEDTLS_RSA_SALT_LEN_ANY,
		 sig,header);

}
