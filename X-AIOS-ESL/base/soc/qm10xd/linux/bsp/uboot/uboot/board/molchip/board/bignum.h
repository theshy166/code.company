#ifndef _BIG_NUM_H_
#define _BIG_NUM_H_

/*
 * Maximum window size used for modular exponentiation. Default: 6
 * Minimum value: 1. Maximum value: 6.
 *
 * Result is an array of ( 2 << MBEDTLS_MPI_WINDOW_SIZE ) MPIs used
 * for the sliding window calculation. (So 64 by default)
 *
 * Reduction in size, reduces speed.
 */

/**< Maximum windows size used. */
#define MBEDTLS_MPI_WINDOW_SIZE			6

/**< Maximum number of bytes for usable MPIs. */
#define MBEDTLS_MPI_MAX_SIZE			1024

/**< An error occurred while reading from or writing to a file. */
#define MBEDTLS_ERR_MPI_FILE_IO_ERROR		-0x0002

/**< Bad input parameters to function. */
#define MBEDTLS_ERR_MPI_BAD_INPUT_DATA		-0x0004

/**< There is an invalid character in the digit string. */
#define MBEDTLS_ERR_MPI_INVALID_CHARACTER	-0x0006

/**< The buffer is too small to write to. */
#define MBEDTLS_ERR_MPI_BUFFER_TOO_SMALL	-0x0008

/**< The input arguments are negative or result in illegal output. */
#define MBEDTLS_ERR_MPI_NEGATIVE_VALUE		-0x000A

/**< The input argument for division is zero, which is not allowed. */
#define MBEDTLS_ERR_MPI_DIVISION_BY_ZERO	-0x000C

/**< The input arguments are not acceptable. */
#define MBEDTLS_ERR_MPI_NOT_ACCEPTABLE		-0x000E

/**< Memory allocation failed. */
#define MBEDTLS_ERR_MPI_ALLOC_FAILED		-0x0010

#define MBEDTLS_MPI_CHK(f)			\
	do {if ((ret = f) != 0) goto cleanup; } while (0)

/**< Maximum number of bits for usable MPIs. */
#define MBEDTLS_MPI_MAX_BITS			(8 * MBEDTLS_MPI_MAX_SIZE)

/*
 * When reading from files with mbedtls_mpi_read_file() and writing to
 * files with mbedtls_mpi_write_file() the buffer should have space
 * for a (short) label, the MPI (in the provided radix), the newline
 * characters and the '\0'.
 *
 * By default we assume at least a 10 char label, a minimum radix of 10
 * (decimal) and a maximum of 4096 bit numbers (1234 decimal chars).
 * Autosized at compile time for at least a 10 char label, a minimum radix
 * of 10 (decimal) for a number of MBEDTLS_MPI_MAX_BITS size.
 *
 * This used to be statically sized to 1250 for a maximum of 4096 bit
 * numbers (1234 decimal chars).
 *
 * Calculate using the formula:
 *  MBEDTLS_MPI_RW_BUFFER_SIZE = ceil(MBEDTLS_MPI_MAX_BITS / ln(10) *
 * ln(2)) + LabelSize + 6
 */
#define MBEDTLS_MPI_MAX_BITS_SCALE100		(100 * MBEDTLS_MPI_MAX_BITS)
#define MBEDTLS_LN_2_DIV_LN_10_SCALE100		332
#define MBEDTLS_MPI_RW_BUFFER_SIZE		\
	(((MBEDTLS_MPI_MAX_BITS_SCALE100 +	\
	MBEDTLS_LN_2_DIV_LN_10_SCALE100 - 1)	\
	/ MBEDTLS_LN_2_DIV_LN_10_SCALE100) + 10 + 6)

/*
 * Maximum size MPIs are allowed to grow to in number of limbs.
 */
#define MBEDTLS_MPI_MAX_LIMBS			10000
#define MULADDC_INIT				\
{						\
	mbedtls_mpi_uint s0, s1, b0, b1;	\
	mbedtls_mpi_uint r0, r1, rx, ry;	\
	b0 = (b << biH) >> biH;			\
	b1 = (b >> biH);

#define MULADDC_CORE				\
	s0 = (*s << biH) >> biH;		\
	s1 = (*s >> biH); s++;			\
	rx = s0 * b1; r0 = s0 * b0;		\
	ry = s1 * b0; r1 = s1 * b1;		\
	r1 += (rx >> biH);			\
	r1 += (ry >> biH);			\
	rx <<= biH; ry <<= biH;			\
	r0 += rx; r1 += (r0 < rx);		\
	r0 += ry; r1 += (r0 < ry);		\
	r0 +=  c; r1 += (r0 <  c);		\
	r0 += *d; r1 += (r0 < *d);		\
	c = r1; *(d++) = r0;

#define MULADDC_STOP				\
}
void mbedtls_mpi_init(mbedtls_mpi *X);
void mbedtls_mpi_free(mbedtls_mpi *X);
int mbedtls_mpi_grow(mbedtls_mpi *X, u32 nblimbs);
int mbedtls_mpi_copy(mbedtls_mpi *X, const mbedtls_mpi *Y);
void mbedtls_mpi_swap(mbedtls_mpi *X, mbedtls_mpi *Y);
int mbedtls_mpi_safe_cond_assign(mbedtls_mpi *X,
	const mbedtls_mpi *Y, unsigned char assign);
int mbedtls_mpi_safe_cond_swap(mbedtls_mpi *X,
	mbedtls_mpi *Y, unsigned char swap);
int mbedtls_mpi_lset(mbedtls_mpi *X, mbedtls_mpi_sint z);
int mbedtls_mpi_get_bit(const mbedtls_mpi *X, u32 pos);
int mbedtls_mpi_set_bit(mbedtls_mpi *X, u32 pos,
	unsigned char val);
u32 mbedtls_mpi_lsb(const mbedtls_mpi *X);
u32 mbedtls_mpi_bitlen(const mbedtls_mpi *X);
u32 mbedtls_mpi_size(const mbedtls_mpi *X);
int mbedtls_mpi_read_string(mbedtls_mpi *X, int radix,
	const char *s);
int mbedtls_mpi_write_string(const mbedtls_mpi *X, int radix,
	char *buf, u32 buflen, u32 *olen);
int mbedtls_mpi_read_binary(mbedtls_mpi *X,
	const unsigned char *buf, u32 buflen);
int mbedtls_mpi_write_binary(const mbedtls_mpi *X,
	unsigned char *buf, u32 buflen);
int mbedtls_mpi_shift_l(mbedtls_mpi *X, u32 count);
int mbedtls_mpi_cmp_abs(const mbedtls_mpi *X, const mbedtls_mpi *Y);
int mbedtls_mpi_cmp_mpi(const mbedtls_mpi *X, const mbedtls_mpi *Y);
int mbedtls_mpi_cmp_int(const mbedtls_mpi *X, mbedtls_mpi_sint z);
int mbedtls_mpi_add_abs(mbedtls_mpi *X, const mbedtls_mpi *A,
	const mbedtls_mpi *B);
int mbedtls_mpi_sub_abs(mbedtls_mpi *X, const mbedtls_mpi *A,
	const mbedtls_mpi *B);
int mbedtls_mpi_add_mpi(mbedtls_mpi *X, const mbedtls_mpi *A,
	const mbedtls_mpi *B);
int mbedtls_mpi_sub_mpi(mbedtls_mpi *X, const mbedtls_mpi *A,
	const mbedtls_mpi *B);
int mbedtls_mpi_add_int(mbedtls_mpi *X, const mbedtls_mpi *A,
	mbedtls_mpi_sint b);
int mbedtls_mpi_sub_int(mbedtls_mpi *X, const mbedtls_mpi *A,
	mbedtls_mpi_sint b);
void mpi_mul_hlp(u32 i, mbedtls_mpi_uint *s, mbedtls_mpi_uint *d,
	mbedtls_mpi_uint b);
int mbedtls_mpi_mul_mpi(mbedtls_mpi *X, const mbedtls_mpi *A,
	const mbedtls_mpi *B);
int mbedtls_mpi_mul_int(mbedtls_mpi *X, const mbedtls_mpi *A,
	mbedtls_mpi_uint b);
int mbedtls_mpi_div_mpi(mbedtls_mpi *Q, mbedtls_mpi *R,
	const mbedtls_mpi *A,
	const mbedtls_mpi *B);
int mbedtls_mpi_div_int(mbedtls_mpi *Q, mbedtls_mpi *R,
	const mbedtls_mpi *A,
	mbedtls_mpi_sint b);
int mbedtls_mpi_mod_mpi(mbedtls_mpi *R, const mbedtls_mpi *A,
	const mbedtls_mpi *B);
int mbedtls_mpi_mod_int(mbedtls_mpi_uint *r, const mbedtls_mpi *A,
	mbedtls_mpi_sint b);
int mbedtls_mpi_exp_mod(mbedtls_mpi *X, const mbedtls_mpi *A,
	const mbedtls_mpi *E, const mbedtls_mpi *N,
	mbedtls_mpi *_RR);
int mbedtls_mpi_gcd(mbedtls_mpi *G, const mbedtls_mpi *A,
	const mbedtls_mpi *B);
int mbedtls_mpi_fill_random(mbedtls_mpi *X, u32 size,
	int (*f_rng)(void *, unsigned char *, u32),
	void *p_rng);
int mbedtls_mpi_inv_mod(mbedtls_mpi *X, const mbedtls_mpi *A,
	const mbedtls_mpi *N);
int mbedtls_mpi_shift_r(mbedtls_mpi *X, u32 count);
void *mbedtls_calloc(u32 n, u32 size);
void mbedtls_free(void *ptr);

#endif
