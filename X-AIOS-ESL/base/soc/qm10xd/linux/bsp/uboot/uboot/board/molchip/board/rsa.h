#ifndef _RSA_H_
#define _RSA_H_
#include "system.h"

/*
 * RSA Error codes
 */
/**< Bad input parameters to function. */
#define MBEDTLS_ERR_RSA_BAD_INPUT_DATA		-0x4080
/**< Input data contains invalid padding and is rejected. */
#define MBEDTLS_ERR_RSA_INVALID_PADDING		-0x4100
/**< Something failed during generation of a key. */
#define MBEDTLS_ERR_RSA_KEY_GEN_FAILED		-0x4180
/**< Key failed to pass the validity check of the library. */
#define MBEDTLS_ERR_RSA_KEY_CHECK_FAILED	-0x4200
/**< The public key operation failed. */
#define MBEDTLS_ERR_RSA_PUBLIC_FAILED		-0x4280
/**< The private key operation failed. */
#define MBEDTLS_ERR_RSA_PRIVATE_FAILED		-0x4300
/**< The PKCS#1 verification failed. */
#define MBEDTLS_ERR_RSA_VERIFY_FAILED		-0x4380
/**< The output buffer for decryption is not large enough. */
#define MBEDTLS_ERR_RSA_OUTPUT_TOO_LARGE	-0x4400
/**< The random generator failed to generate non-zeros. */
#define MBEDTLS_ERR_RSA_RNG_FAILED		-0x4480

/* MBEDTLS_ERR_RSA_UNSUPPORTED_OPERATION is deprecated
 *and should not be used.
 */
/**< The implementation does not offer the requested operation,
 *for example, because of security violations or lack of functionality.
 */
#define MBEDTLS_ERR_RSA_UNSUPPORTED_OPERATION	-0x4500

/* MBEDTLS_ERR_RSA_HW_ACCEL_FAILED is deprecated and should not be used.*/
/**< RSA hardware accelerator failed. */
#define MBEDTLS_ERR_RSA_HW_ACCEL_FAILED		-0x4580

/*
 * RSA constants
 */
/**< Request private key operation. */
#define MBEDTLS_RSA_PUBLIC			0
/**< Request public key operation. */
#define MBEDTLS_RSA_PRIVATE			1

/**< Use PKCS#1 v1.5 encoding. */
#define MBEDTLS_RSA_PKCS_V15			0
/**< Use PKCS#1 v2.1 encoding. */
#define MBEDTLS_RSA_PKCS_V21			1

/**< Identifier for RSA signature operations. */
#define MBEDTLS_RSA_SIGN			1
/**< Identifier for RSA encryption and decryption operations. */
#define MBEDTLS_RSA_CRYPT			2

#define MBEDTLS_RSA_SALT_LEN_ANY		-1

/**
 * \name DER constants
 * These constants comply with the DER encoded ASN.1 type tags.
 * DER encoding uses hexadecimal representation.
 * An example DER sequence is:\n
 * - 0x02 -- tag indicating INTEGER
 * - 0x01 -- length in octets
 * - 0x05 -- value
 * Such sequences are typically read into \c ::mbedtls_x509_buf.
 * \{
 */
#define MBEDTLS_ASN1_BOOLEAN			0x01
#define MBEDTLS_ASN1_INTEGER			0x02
#define MBEDTLS_ASN1_BIT_STRING			0x03
#define MBEDTLS_ASN1_OCTET_STRING		0x04
#define MBEDTLS_ASN1_NULL			0x05
#define MBEDTLS_ASN1_OID			0x06
#define MBEDTLS_ASN1_UTF8_STRING		0x0C
#define MBEDTLS_ASN1_SEQUENCE			0x10
#define MBEDTLS_ASN1_SET			0x11
#define MBEDTLS_ASN1_PRINTABLE_STRING		0x13
#define MBEDTLS_ASN1_T61_STRING			0x14
#define MBEDTLS_ASN1_IA5_STRING			0x16
#define MBEDTLS_ASN1_UTC_TIME			0x17
#define MBEDTLS_ASN1_GENERALIZED_TIME		0x18
#define MBEDTLS_ASN1_UNIVERSAL_STRING		0x1C
#define MBEDTLS_ASN1_BMP_STRING			0x1E
#define MBEDTLS_ASN1_PRIMITIVE			0x00
#define MBEDTLS_ASN1_CONSTRUCTED		0x20
#define MBEDTLS_ASN1_CONTEXT_SPECIFIC		0x80

typedef int mbedtls_mpi_sint;
typedef u32 mbedtls_mpi_uint;

typedef struct mbedtls_mpi {
	int s;	/*!<  integer sign      */
	u32 n;	/*!<  total # of limbs  */
	mbedtls_mpi_uint *p;	/*!<  pointer to limbs  */
}
mbedtls_mpi;

// Regular implementation
//

/**
 * \brief   The RSA context structure.
 *
 * \note    Direct manipulation of the members of this structure
 *          is deprecated. All manipulation should instead be done through
 *          the public interface functions.
 */
typedef struct rsa_context {
	int ver;	/*!<  Always 0.*/
	u32 len;	/*!<  The size of \p N in Bytes. */

	mbedtls_mpi N;	/*!<  The public modulus. */
	mbedtls_mpi E;	/*!<  The public exponent. */

	mbedtls_mpi D;	/*!<  The private exponent. */
	mbedtls_mpi P;	/*!<  The first prime factor. */
	mbedtls_mpi Q;	/*!<  The second prime factor. */

	mbedtls_mpi DP;	/*!<  <code>D % (P - 1)</code>. */
	mbedtls_mpi DQ;	/*!<  <code>D % (Q - 1)</code>. */
	mbedtls_mpi QP;	/*!<  <code>1 / (Q % P)</code>. */

	mbedtls_mpi RN;	/*!<  cached <code>R^2 mod N</code>. */

	mbedtls_mpi RP;	/*!<  cached <code>R^2 mod P</code>. */
	mbedtls_mpi RQ;	/*!<  cached <code>R^2 mod Q</code>. */

	mbedtls_mpi Vi;	/*!<  The cached blinding value. */
	mbedtls_mpi Vf;	/*!<  The cached un-blinding value. */

	int padding;	/*!< Selects padding mode:
			 * MBEDTLS_RSA_PKCS_V15 for 1.5 padding and
			 * #MBEDTLS_RSA_PKCS_V21 for OAEP or PSS.
			 */
	int hash_id;	/*!< Hash identifier of mbedtls_md_type_t type,
			 *as specified in md.h for use in the MGF
			 *mask generating function used in the
			 *EME-OAEP and EMSA-PSS encodings.
			 */
}
rsa_context;

/**
 * \name Functions to parse ASN.1 data structures
 * \{
 */

/**
 * Type-length-value structure that allows for ASN1 using DER.
 */
typedef struct mbedtls_asn1_buf {
	int tag;	/**< ASN1 type, e.g. MBEDTLS_ASN1_UTF8_STRING. */
	unsigned int len;	/**< ASN1 length, in octets. */
	unsigned char *p;	/**< ASN1 data, e.g. in ASCII. */
}
mbedtls_asn1_buf;

/**
 * \brief Base OID descriptor structure
 */
typedef struct mbedtls_oid_descriptor_t {
	const char *asn1;	/*!< OID ASN.1 representation */
	unsigned int asn1_len;	/*!< length of asn1 */
	const char *name;	/*!< official name (e.g. from RFC) */
	const char *description;	/*!< human friendly description */
} mbedtls_oid_descriptor_t;

/**
 * \brief          Translate md_type into hash algorithm OID
 *
 * \param md_alg   message digest algorithm
 * \param oid      place to store ASN.1 OID string pointer
 * \param olen     length of the OID
 *
 * \return         0 if successful, or MBEDTLS_ERR_OID_NOT_FOUND
 */
int oid_get_oid_by_md(md_type_t md_alg, const char **oid,
	unsigned int *olen);

int rsa_pkcs1_decrypt(rsa_context *ctx,
	int mode, u32 *olen,
	const unsigned char *input,
	unsigned char *output,
	u32 output_max_len);

extern void (*mbedtls_rsa_init)(rsa_context *ctx,
	int padding,
	int hash_id);

int rsa_pkcs1_sign(rsa_context *ctx,
	const unsigned char *hash,
	unsigned char *sig);

extern int (*rsa_pkcs1_verify)(rsa_context *ctx,
	const unsigned char *hash,
	const unsigned char *sig,struct boot_header *header);

int mbedtls_rsa_rsassa_pss_verify(rsa_context *ctx,
	int mode,
	md_type_t md_alg,
	unsigned int hashlen,
	const unsigned char *hash,
	const unsigned char *sig,struct boot_header *header);

#endif
