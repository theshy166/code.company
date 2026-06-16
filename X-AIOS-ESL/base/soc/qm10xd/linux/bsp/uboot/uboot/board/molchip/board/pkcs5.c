/**
 * \file pkcs5.c
 *
 * \brief PKCS#5 functions
 *
 * \author Mathias Olsson <mathias@kompetensum.com>
 *
 * Copyright (C) 2006-2015, ARM Limited, All Rights Reserved
 * SPDX-License-Identifier: Apache-2.0
 *
 * Licensed under the Apache License, Version 2.0 (the "License"); you may
 * not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *  http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 *WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 * This file is part of mbed TLS (https://tls.mbed.org)
 */
/*
 * PKCS#5 includes PBKDF2 and more
 *
 * http://tools.ietf.org/html/rfc2898 (Specification)
 * http://tools.ietf.org/html/rfc6070 (Test vectors)
 */
#include "type_use.h"
#include "mci.h"
#include "sstring.h"
#include "sha.h"
#include "pkcs5.h"
#include "rsa.h"
#include "bignum.h"

void mbedtls_sha256_init(sha256_context *ctx)
{
	mmemset(ctx, 0, sizeof(sha256_context));
}

void mbedtls_sha256_free(sha256_context *ctx)
{
	if (ctx == NULL)
		return;
	mmemset(ctx, 0, sizeof(sha256_context));
}

static void *sha256_ctx_alloc(void)
{
	void *ctx = mbedtls_calloc(1, sizeof(sha256_context));

	if (ctx != NULL)
		mbedtls_sha256_init((sha256_context *) ctx);

	return ctx;
}

static void sha256_ctx_free(void *ctx)
{
	mbedtls_sha256_free((sha256_context *) ctx);
	mbedtls_free(ctx);
}

static int sha256_starts_wrap(void *ctx)
{
	return sha256_init((sha256_context *) ctx);
}

static int sha256_update_wrap(void *ctx, const unsigned char *input,
	u32 ilen)
{
	return sha256_update((sha256_context *) ctx, input, ilen);
}

static int sha256_finish_wrap(void *ctx, unsigned char *output)
{
	return sha256_final((sha256_context *) ctx, output);
}

const mbedtls_md_info_t mbedtls_sha256_info = {
	32,
	64,
	sha256_starts_wrap,
	sha256_update_wrap,
	sha256_finish_wrap,
	sha256_ctx_alloc,
	sha256_ctx_free,
};

const mbedtls_md_info_t *md_info_from_type(md_type_t md_type)
{
	switch (md_type) {
	case MBEDTLS_MD_SHA256:
		return &mbedtls_sha256_info;
	default:
		return NULL;
	}
}

unsigned char mbedtls_md_get_size(const mbedtls_md_info_t *md_info)
{
	if (md_info == NULL)
		return 0;

	return md_info->size;
}

int mbedtls_md_setup(mbedtls_md_context_t *ctx,
	const mbedtls_md_info_t *md_info, int hmac)
{
	if (md_info == NULL || ctx == NULL)
		return MBEDTLS_ERR_MD_BAD_INPUT_DATA;

	ctx->md_ctx = md_info->ctx_alloc_func();
	if (ctx->md_ctx == NULL)
		return MBEDTLS_ERR_MD_ALLOC_FAILED;

	if (hmac != 0) {
		ctx->hmac_ctx = mbedtls_calloc(2, md_info->block_size);
		if (ctx->hmac_ctx == NULL) {
			md_info->ctx_free_func(ctx->md_ctx);
			return MBEDTLS_ERR_MD_ALLOC_FAILED;
		}
	}

	ctx->md_info = md_info;

	return 0;
}

void mbedtls_md_init(mbedtls_md_context_t *ctx)
{
	mmemset(ctx, 0, sizeof(mbedtls_md_context_t));
}

int mbedtls_md_starts(mbedtls_md_context_t *ctx)
{
	if (ctx == NULL || ctx->md_info == NULL)
		return MBEDTLS_ERR_MD_BAD_INPUT_DATA;

	return ctx->md_info->starts_func(ctx->md_ctx);
}

int mbedtls_md_update(mbedtls_md_context_t *ctx,
	const unsigned char *input, u32 ilen)
{
	if (ctx == NULL || ctx->md_info == NULL)
		return MBEDTLS_ERR_MD_BAD_INPUT_DATA;

	return ctx->md_info->update_func(ctx->md_ctx, input, ilen);
}

int mbedtls_md_finish(mbedtls_md_context_t *ctx, unsigned char *output)
{
	if (ctx == NULL || ctx->md_info == NULL)
		return MBEDTLS_ERR_MD_BAD_INPUT_DATA;

	return ctx->md_info->finish_func(ctx->md_ctx, output);
}

int mbedtls_md_hmac_starts(mbedtls_md_context_t *ctx,
	const unsigned char *key, u32 keylen)
{
	int ret;
	unsigned char sum[MBEDTLS_MD_MAX_SIZE];
	unsigned char *ipad, *opad;
	u32 i;

	if (ctx == NULL || ctx->md_info == NULL || ctx->hmac_ctx == NULL)
		return MBEDTLS_ERR_MD_BAD_INPUT_DATA;

	if (keylen > (u32) ctx->md_info->block_size) {
		ret = ctx->md_info->starts_func(ctx->md_ctx);
		if (ret != 0)
			goto cleanup;
		ret = ctx->md_info->update_func(ctx->md_ctx, key, keylen);
		if (ret != 0)
			goto cleanup;
		ret = ctx->md_info->finish_func(ctx->md_ctx, sum);
		if (ret != 0)
			goto cleanup;

		keylen = ctx->md_info->size;
		key = sum;
	}

	ipad = (unsigned char *) ctx->hmac_ctx;
	opad = (unsigned char *) ctx->hmac_ctx + ctx->md_info->block_size;

	mmemset(ipad, 0x36, ctx->md_info->block_size);
	mmemset(opad, 0x5C, ctx->md_info->block_size);

	for (i = 0; i < keylen; i++) {
		ipad[i] = (unsigned char)(ipad[i] ^ key[i]);
		opad[i] = (unsigned char)(opad[i] ^ key[i]);
	}

	ret = ctx->md_info->starts_func(ctx->md_ctx);
	if (ret != 0)
		goto cleanup;
	ret = ctx->md_info->update_func(ctx->md_ctx, ipad,
		ctx->md_info->block_size);
	if (ret != 0)
		goto cleanup;

cleanup:
	mmemset(sum, 0, sizeof(sum));

	return ret;
}

int mbedtls_md_hmac_update(mbedtls_md_context_t *ctx,
	const unsigned char *input, u32 ilen)
{
	if (ctx == NULL || ctx->md_info == NULL || ctx->hmac_ctx == NULL)
		return MBEDTLS_ERR_MD_BAD_INPUT_DATA;

	return ctx->md_info->update_func(ctx->md_ctx, input, ilen);
}

int mbedtls_md_hmac_finish(mbedtls_md_context_t *ctx,
	unsigned char *output)
{
	int ret;
	unsigned char tmp[MBEDTLS_MD_MAX_SIZE];
	unsigned char *opad;

	if (ctx == NULL || ctx->md_info == NULL || ctx->hmac_ctx == NULL)
		return MBEDTLS_ERR_MD_BAD_INPUT_DATA;

	opad = (unsigned char *) ctx->hmac_ctx + ctx->md_info->block_size;

	ret = ctx->md_info->finish_func(ctx->md_ctx, tmp);
	if (ret != 0)
		return ret;
	ret = ctx->md_info->starts_func(ctx->md_ctx);
	if (ret != 0)
		return ret;
	ret = ctx->md_info->update_func(ctx->md_ctx, opad,
		ctx->md_info->block_size);
	if (ret != 0)
		return ret;
	ret = ctx->md_info->update_func(ctx->md_ctx, tmp,
		ctx->md_info->size);
	if (ret != 0)
		return ret;

	return ctx->md_info->finish_func(ctx->md_ctx, output);
}

int mbedtls_pkcs5_pbkdf2_hmac(mbedtls_md_context_t *ctx,
	const unsigned char *password,
	u32 plen, const unsigned char *salt, u32 slen,
	unsigned int iteration_count,
	u32 key_length, unsigned char *output)
{
	int ret, j;
	unsigned int i;
	unsigned char md1[MBEDTLS_MD_MAX_SIZE];
	unsigned char work[MBEDTLS_MD_MAX_SIZE];
	unsigned char md_size = mbedtls_md_get_size(ctx->md_info);
	u32 use_len;
	unsigned char *out_p = output;
	unsigned char counter[4];

	mmemset(counter, 0, 4);
	counter[3] = 1;

	while (key_length) {
		// U1 ends up in work
		//
		ret = mbedtls_md_hmac_starts(ctx, password, plen);
		if (ret != 0)
			return ret;

		ret = mbedtls_md_hmac_update(ctx, salt, slen);
		if (ret != 0)
			return ret;

		ret = mbedtls_md_hmac_update(ctx, counter, 4);
		if (ret != 0)
			return ret;

		ret = mbedtls_md_hmac_finish(ctx, work);
		if (ret != 0)
			return ret;

		mmemcpy(md1, work, md_size);

		for (i = 1; i < iteration_count; i++) {
			// U2 ends up in md1
			//
			ret = mbedtls_md_hmac_starts(ctx, password,
				plen);
			if (ret != 0)
				return ret;

			ret = mbedtls_md_hmac_update(ctx, md1,
							md_size);
			if (ret != 0)
				return ret;

			ret = mbedtls_md_hmac_finish(ctx, md1);
			if (ret != 0)
				return ret;

			// U1 xor U2
			//
			for (j = 0; j < md_size; j++)
				work[j] ^= md1[j];
		}

		use_len = (key_length < md_size) ? key_length : md_size;
		mmemcpy(out_p, work, use_len);

		key_length -= (u32) use_len;
		out_p += use_len;

		for (i = 4; i > 0; i--)
			if (++counter[i - 1] != 0)
				break;
	}

	return 0;
}

void mbedtls_md_free(mbedtls_md_context_t *ctx)
{
	if (ctx == NULL || ctx->md_info == NULL)
		return;

	if (ctx->md_ctx != NULL)
		ctx->md_info->ctx_free_func(ctx->md_ctx);

	if (ctx->hmac_ctx != NULL) {
		mmemset(ctx->hmac_ctx, 0, 2 * ctx->md_info->block_size);
		mbedtls_free(ctx->hmac_ctx);
	}

	mmemset(ctx, 0, sizeof(mbedtls_md_context_t));
}

int pbkdf2(struct pbkdf_param *pbkdf)
{
	mbedtls_md_context_t sha_ctx;
	int ret;

	mmemset(&sha_ctx, 0, sizeof(sha_ctx));

	ret = mbedtls_md_setup(&sha_ctx, &mbedtls_sha256_info, 1);
	if (ret != 0) {
		ret = 1;
		goto exit;
	}

	ret = mbedtls_pkcs5_pbkdf2_hmac(&sha_ctx, pbkdf->pwd, pbkdf->pwd_len,
		pbkdf->salt,
		pbkdf->salt_len, pbkdf->it_cnt, pbkdf->key_len, pbkdf->key);
	if (ret != 0) {
		ret = 1;
		goto exit;
	}

exit:
	mbedtls_md_free(&sha_ctx);

	return ret;
}
