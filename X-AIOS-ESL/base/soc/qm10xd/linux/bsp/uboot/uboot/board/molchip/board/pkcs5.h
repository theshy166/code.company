/**
 * \file pkcs5.h
 *
 * \brief PKCS#5 functions
 *
 * \author Mathias Olsson <mathias@kompetensum.com>
 */
/*
 *  Copyright (C) 2006-2015, ARM Limited, All Rights Reserved
 *  SPDX-License-Identifier: Apache-2.0
 *
 *  Licensed under the Apache License, Version 2.0 (the "License");
 * you may
 *  not use this file except in compliance with the License.
 *  You may obtain a copy of the License at
 *
 *  http://www.apache.org/licenses/LICENSE-2.0
 *
 *  Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT
 *  WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the License for the specific language governing permissions and
 *  limitations under the License.
 *
 *  This file is part of mbed TLS (https://tls.mbed.org)
 */
#ifndef MBEDTLS_PKCS5_H
#define MBEDTLS_PKCS5_H

/**< The selected feature is not available. */
#define MBEDTLS_ERR_MD_FEATURE_UNAVAILABLE	-0x5080
/**< Bad input parameters to function. */
#define MBEDTLS_ERR_MD_BAD_INPUT_DATA		-0x5100
/**< Failed to allocate memory. */
#define MBEDTLS_ERR_MD_ALLOC_FAILED		-0x5180
/**< Opening or reading of file failed. */
#define MBEDTLS_ERR_MD_FILE_IO_ERROR		-0x5200

/* MBEDTLS_ERR_MD_HW_ACCEL_FAILED is deprecated and should not be used. */
/**< MD hardware accelerator failed. */
#define MBEDTLS_ERR_MD_HW_ACCEL_FAILED		-0x5280

/* longest known is SHA256 or less */
#define MBEDTLS_MD_MAX_SIZE			32

#define PBKDF2_IT_CNT				64
#define PBKDF2_SALT_LEN				16

/**< Bad input parameters to function. */
#define MBEDTLS_ERR_PKCS5_BAD_INPUT_DATA	-0x2f80
/**< Unexpected ASN.1 data. */
#define MBEDTLS_ERR_PKCS5_INVALID_FORMAT	-0x2f00
/**< Requested encryption or digest alg not available. */
#define MBEDTLS_ERR_PKCS5_FEATURE_UNAVAILABLE	-0x2e80
/**< Given private key password does not allow for correct decryption. */
#define MBEDTLS_ERR_PKCS5_PASSWORD_MISMATCH	-0x2e00

#define MBEDTLS_PKCS5_DECRYPT			0
#define MBEDTLS_PKCS5_ENCRYPT			1

/**
 * Message digest information.
 * Allows message digest functions to be called in a generic way.
 */
struct mbedtls_md_info_t {
	/** Output length of the digest function in bytes */
	int size;

	/** Block length of the digest function in bytes */
	int block_size;

	/** Digest initialisation function */
	int (*starts_func)(void *ctx);

	/** Digest update function */
	int (*update_func)(void *ctx, const unsigned char *input, u32 ilen);

	/** Digest finalisation function */
	int (*finish_func)(void *ctx, unsigned char *output);
	/** Allocate a new context */
	void * (*ctx_alloc_func)(void);

	/** Free the given context */
	void (*ctx_free_func)(void *ctx);

};

struct pbkdf_param {
	u8 *key;
	u32 key_len;
	u8 *pwd;
	u32 pwd_len;
	u8 *salt;
	u32 salt_len;
	u32 it_cnt;
};

/**
 * Opaque struct defined in md_internal.h.
 */
typedef struct mbedtls_md_info_t mbedtls_md_info_t;

/**
 * The generic message-digest context.
 */
typedef struct mbedtls_md_context_t {
	/** Information about the associated message digest. */
	const mbedtls_md_info_t *md_info;

	/** The digest-specific context. */
	void *md_ctx;

	/** The HMAC part of the context. */
	void *hmac_ctx;
} mbedtls_md_context_t;

/**
 * \brief     Supported message digests.
 *
 *\warning MD2, MD4, MD5 and SHA-1 are considered weak message digests and
 * their use constitutes a security risk. We recommend considering
 * stronger message digests instead.
 *
 */
typedef enum {
	MBEDTLS_MD_NONE = 0,	/**< None. */
	MBEDTLS_MD_SHA1,	/**< The SHA-1 message digest. */
	MBEDTLS_MD_SHA256,	/**< The SHA-256 message digest. */
	MBEDTLS_MD_SHA512,	/**< The SHA-512 message digest. */
} md_type_t;

const mbedtls_md_info_t *md_info_from_type(md_type_t md_type);
unsigned char mbedtls_md_get_size(const mbedtls_md_info_t *md_info);

int pbkdf2(struct pbkdf_param *pbkdf);
int mbedtls_md_setup(mbedtls_md_context_t *ctx,
	const mbedtls_md_info_t *md_info, int hmac);
void mbedtls_md_init(mbedtls_md_context_t *ctx);
int mbedtls_md_starts(mbedtls_md_context_t *ctx);
int mbedtls_md_update(mbedtls_md_context_t *ctx,
	const unsigned char *input, u32 ilen);
int mbedtls_md_finish(mbedtls_md_context_t *ctx,
	unsigned char *output);
void mbedtls_md_free(mbedtls_md_context_t *ctx);

#endif /* pkcs5.h */
