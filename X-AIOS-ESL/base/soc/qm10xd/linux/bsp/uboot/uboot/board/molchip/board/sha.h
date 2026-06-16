#ifndef _SHA1_H_
#define _SHA1_H_

#define GET_UINT32_BE(n, b, i)				\
{							\
	(n) = ((u32) (b)[(i)] << 24)			\
	| ((u32) (b)[(i) + 1] << 16)			\
	| ((u32) (b)[(i) + 2] <<  8)			\
	| ((u32) (b)[(i) + 3]);				\
}

#define PUT_UINT32_BE(n, b, i)				\
do {							\
	(b)[(i)] = (unsigned char) ((n) >> 24);		\
	(b)[(i) + 1] = (unsigned char) ((n) >> 16);	\
	(b)[(i) + 2] = (unsigned char) ((n) >> 8);	\
	(b)[(i) + 3] = (unsigned char) ((n));		\
} while (0)

typedef struct sha256_context {
	u32 total[2];	/*!< The number of Bytes processed.*/
	u32 state[8];	/*!< The intermediate digest state.*/
	unsigned char buffer[64];	/*!< The data block being processed.*/
}
sha256_context;

int sha256_init(sha256_context *ctx);

int sha256_update(sha256_context *ctx,        const unsigned char *input,
	u32 ilen);

int sha256_final(sha256_context *ctx,
	unsigned char output[32]);

extern int (*sha256_ctx)(const u8 *in, unsigned int len, u8 *out);

int sha256_ctx_continue(const u8 *in, unsigned int len);
int sha256_ctx_end(u8 *out);

#endif
