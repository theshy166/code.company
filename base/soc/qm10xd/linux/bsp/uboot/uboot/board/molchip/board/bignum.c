#include <stdlib.h>
//#include "type_use.h"
//#include "mci.h"
#include <common.h>
#include <malloc.h>

#include "sstring.h"
#include "pkcs5.h"
#include "rsa.h"
#include "bignum.h"
#include "system.h"


#define ciL			(sizeof(mbedtls_mpi_uint)) /* chars in limb  */
#define biL			(ciL << 3)	/* bits  in limb  */
#define biH			(ciL << 2)	/* half limb size */

#define MPI_SIZE_T_MAX		((u32) -1)	/* SIZE_T_MAX is not standard */

/*
 * Convert between bits/chars and number of limbs
 * Divide first in order to avoid potential overflows
 */
#define BITS_TO_LIMBS(i)	((i) / biL + ((i) % biL != 0))
#define CHARS_TO_LIMBS(i)	((i) / ciL + ((i) % ciL != 0))

/* Get a specific byte, without range checks. */
#define GET_BYTE(X, i)		\
	(((X)->p[(i) / ciL] >> (((i) % ciL) * 8)) & 0xff)

void *mbedtls_calloc(u32 n, u32 size)
{
	u8 *p;

	p =  smalloc(n * size);
	mmemset(p, 0, n * size);

	return p;
}

void mbedtls_free(void *ptr)
{
	sfree(ptr);
}

/* Implementation that should never be optimized out by the compiler */
static void mbedtls_mpi_zeroize(mbedtls_mpi_uint *v, u32 n)
{
	mmemset(v, 0, ciL * n);
}

/*
 * Initialize one MPI
 */
void mbedtls_mpi_init(mbedtls_mpi *X)
{
	X->s = 1;
	X->n = 0;
	X->p = NULL;
}

/*
 * Unallocate one MPI
 */
void mbedtls_mpi_free(mbedtls_mpi *X)
{
	if (X == NULL)
		return;

	if (X->p != NULL) {
		mbedtls_mpi_zeroize(X->p, X->n);
		mbedtls_free(X->p);
	}

	X->s = 1;
	X->n = 0;
	X->p = NULL;
}

/*
 * Enlarge to the specified number of limbs
 */
int mbedtls_mpi_grow(mbedtls_mpi *X, u32 nblimbs)
{
	mbedtls_mpi_uint *p;

	if (nblimbs > MBEDTLS_MPI_MAX_LIMBS)
		return MBEDTLS_ERR_MPI_ALLOC_FAILED;

	if (X->n < nblimbs) {
		p = (mbedtls_mpi_uint *)mbedtls_calloc(nblimbs, ciL);
		if (p == NULL)
			return MBEDTLS_ERR_MPI_ALLOC_FAILED;

		if (X->p != NULL) {
			mmemcpy(p, X->p, X->n * ciL);
			mbedtls_mpi_zeroize(X->p, X->n);
			mbedtls_free(X->p);
		}

		X->n = nblimbs;
		X->p = p;
	}

	return 0;
}

/*
 * Copy the contents of Y into X
 */
int mbedtls_mpi_copy(mbedtls_mpi *X, const mbedtls_mpi *Y)
{
	int ret = 0;
	u32 i;

	if (X == Y)
		return 0;

	if (Y->p == NULL) {
		mbedtls_mpi_free(X);
		return 0;
	}

	for (i = Y->n - 1; i > 0; i--)
		if (Y->p[i] != 0)
			break;
	i++;

	X->s = Y->s;

	if (X->n < i)
		MBEDTLS_MPI_CHK(mbedtls_mpi_grow(X, i));
	else
		mmemset(X->p + i, 0, (X->n - i) * ciL);

	mmemcpy(X->p, Y->p, i * ciL);

cleanup:

	return ret;
}

/*
 * Set value from integer
 */
int mbedtls_mpi_lset(mbedtls_mpi *X, mbedtls_mpi_sint z)
{
	int ret;

	MBEDTLS_MPI_CHK(mbedtls_mpi_grow(X, 1));
	mmemset(X->p, 0, X->n * ciL);

	X->p[0] = (z < 0) ? -z : z;
	X->s = (z < 0) ? -1 : 1;

cleanup:

	return ret;
}

/*
 * Return the number of less significant zero-bits
 */
u32 mbedtls_mpi_lsb(const mbedtls_mpi *X)
{
	u32 i, j, count = 0;

	for (i = 0; i < X->n; i++)
		for (j = 0; j < biL; j++, count++)
			if (((X->p[i] >> j) & 1) != 0)
				return count;

	return 0;
}

/*
 * Count leading zero bits in a given integer
 */
static u32 mbedtls_clz(const mbedtls_mpi_uint x)
{
	u32 j;
	mbedtls_mpi_uint mask = (mbedtls_mpi_uint) 1 << (biL - 1);

	for (j = 0; j < biL; j++) {
		if (x & mask)
			break;

		mask >>= 1;
	}

	return j;
}

/*
 * Return the number of bits
 */
u32 mbedtls_mpi_bitlen(const mbedtls_mpi *X)
{
	u32 i, j;

	if (X->n == 0)
		return 0;

	for (i = X->n - 1; i > 0; i--)
		if (X->p[i] != 0)
			break;

	j = biL - mbedtls_clz(X->p[i]);

	return ((i * biL) + j);
}

/*
 * Return the total size in bytes
 */
u32 mbedtls_mpi_size(const mbedtls_mpi *X)
{
	return ((mbedtls_mpi_bitlen(X) + 7) >> 3);
}

/*
 * Import X from unsigned binary data, big endian
 */
int mbedtls_mpi_read_binary(mbedtls_mpi *X, const unsigned char *buf,
	u32 buflen)
{
	int ret;
	u32 i, j;
	u32 const limbs = CHARS_TO_LIMBS(buflen);

	/* Ensure that target MPI has exactly the necessary number of limbs */
	if (X->n != limbs) {
		mbedtls_mpi_free(X);
		mbedtls_mpi_init(X);
		MBEDTLS_MPI_CHK(mbedtls_mpi_grow(X, limbs));
	}

	MBEDTLS_MPI_CHK(mbedtls_mpi_lset(X, 0));

	for (i = buflen, j = 0; i > 0; i--, j++)
		X->p[j / ciL] |= ((mbedtls_mpi_uint) buf[i - 1]) <<
			((j % ciL) << 3);

cleanup:

	return ret;
}

/*
 * Export X into unsigned binary data, big endian
 */
int mbedtls_mpi_write_binary(const mbedtls_mpi *X,
	unsigned char *buf, u32 buflen)
{
	u32 stored_bytes;
	u32 bytes_to_copy;
	unsigned char *p;
	u32 i;

	stored_bytes = X->n * ciL;

	if (stored_bytes < buflen) {
		/* There is enough space in the output buffer. Write initial
		 * null bytes and record the position at which to start
		 * writing the significant bytes. In this case, the execution
		 * trace of this function does not depend on the value of the
		 * number.
		 */
		bytes_to_copy = stored_bytes;
		p = buf + buflen - stored_bytes;
		mmemset(buf, 0, buflen - stored_bytes);
	} else {
		/* The output buffer is smaller than the allocated size of X.
		 * However X may fit if its leading bytes are zero.
		 */
		bytes_to_copy = buflen;
		p = buf;
		for (i = bytes_to_copy; i < stored_bytes; i++) {
			if (GET_BYTE(X, i) != 0)
				return MBEDTLS_ERR_MPI_BUFFER_TOO_SMALL;
		}
	}

	for (i = 0; i < bytes_to_copy; i++)
		p[bytes_to_copy - i - 1] = GET_BYTE(X, i);

	return 0;
}

/*
 * Left-shift: X <<= count
 */
int mbedtls_mpi_shift_l(mbedtls_mpi *X, u32 count)
{
	int ret;
	u32 i, v0, t1;
	mbedtls_mpi_uint r0 = 0, r1;

	v0 = count / (biL);
	t1 = count & (biL - 1);

	i = mbedtls_mpi_bitlen(X) + count;

	if (X->n * biL < i)
		MBEDTLS_MPI_CHK(mbedtls_mpi_grow(X, BITS_TO_LIMBS(i)));

	ret = 0;

	/*
	 * shift by count / limb_size
	 */
	if (v0 > 0) {
		for (i = X->n; i > v0; i--)
			X->p[i - 1] = X->p[i - v0 - 1];

		for (; i > 0; i--)
			X->p[i - 1] = 0;
	}

	/*
	 * shift by count % limb_size
	 */
	if (t1 > 0) {
		for (i = v0; i < X->n; i++) {
			r1 = X->p[i] >> (biL - t1);
			X->p[i] <<= t1;
			X->p[i] |= r0;
			r0 = r1;
		}
	}

cleanup:

	return ret;
}

/*
 * Right-shift: X >>= count
 */
int mbedtls_mpi_shift_r(mbedtls_mpi *X, u32 count)
{
	u32 i, v0, v1;
	mbedtls_mpi_uint r0 = 0, r1;

	v0 = count / biL;
	v1 = count & (biL - 1);

	if (v0 > X->n || (v0 == X->n && v1 > 0))
		return mbedtls_mpi_lset(X, 0);

	/*
	 * shift by count / limb_size
	 */
	if (v0 > 0) {
		for (i = 0; i < X->n - v0; i++)
			X->p[i] = X->p[i + v0];

		for (; i < X->n; i++)
			X->p[i] = 0;
	}

	/*
	 * shift by count % limb_size
	 */
	if (v1 > 0) {
		for (i = X->n; i > 0; i--) {
			r1 = X->p[i - 1] << (biL - v1);
			X->p[i - 1] >>= v1;
			X->p[i - 1] |= r0;
			r0 = r1;
		}
	}

	return 0;
}

/*
 * Compare unsigned values
 */
int mbedtls_mpi_cmp_abs(const mbedtls_mpi *X, const mbedtls_mpi *Y)
{
	u32 i, j;

	for (i = X->n; i > 0; i--)
		if (X->p[i - 1] != 0)
			break;

	for (j = Y->n; j > 0; j--)
		if (Y->p[j - 1] != 0)
			break;

	if (i == 0 && j == 0)
		return 0;

	if (i > j)
		return 1;
	if (j > i)
		return -1;

	for (; i > 0; i--) {
		if (X->p[i - 1] > Y->p[i - 1])
			return 1;
		if (X->p[i - 1] < Y->p[i - 1])
			return -1;
	}

	return 0;
}

/*
 * Compare signed values
 */
int mbedtls_mpi_cmp_mpi(const mbedtls_mpi *X, const mbedtls_mpi *Y)
{
	u32 i, j;

	for (i = X->n; i > 0; i--)
		if (X->p[i - 1] != 0)
			break;

	for (j = Y->n; j > 0; j--)
		if (Y->p[j - 1] != 0)
			break;

	if (i == 0 && j == 0)
		return 0;

	if (i > j)
		return X->s;
	if (j > i)
		return (-Y->s);

	if (X->s > 0 && Y->s < 0)
		return 1;
	if (Y->s > 0 && X->s < 0)
		return -1;

	for (; i > 0; i--) {
		if (X->p[i - 1] > Y->p[i - 1])
			return X->s;
		if (X->p[i - 1] < Y->p[i - 1])
			return (-X->s);
	}

	return 0;
}

/*
 * Compare signed values
 */
int mbedtls_mpi_cmp_int(const mbedtls_mpi *X, mbedtls_mpi_sint z)
{
	mbedtls_mpi Y;
	mbedtls_mpi_uint p[1];

	*p  = (z < 0) ? -z : z;
	Y.s = (z < 0) ? -1 : 1;
	Y.n = 1;
	Y.p = p;

	return mbedtls_mpi_cmp_mpi(X, &Y);
}

/*
 * Unsigned addition: X = |A| + |B|  (HAC 14.7)
 */
int mbedtls_mpi_add_abs(mbedtls_mpi *X, const mbedtls_mpi *A,
	const mbedtls_mpi *B)
{
	int ret;
	u32 i, j;
	mbedtls_mpi_uint *o, *p, c, tmp;

	if (X == B) {
		const mbedtls_mpi *T = A;

		A = X;
		B = T;
	}

	if (X != A)
		MBEDTLS_MPI_CHK(mbedtls_mpi_copy(X, A));

	/*
	 * X should always be positive as a result of unsigned additions.
	 */
	X->s = 1;

	for (j = B->n; j > 0; j--)
		if (B->p[j - 1] != 0)
			break;

	MBEDTLS_MPI_CHK(mbedtls_mpi_grow(X, j));

	o = B->p;
	p = X->p;
	c = 0;

	/*
	 * tmp is used because it might happen that p == o
	 */
	for (i = 0; i < j; i++, o++, p++) {
		tmp = *o;
		*p += c;
		c = (*p < c);
		*p += tmp;
		c += (*p < tmp);
	}

	while (c != 0) {
		if (i >= X->n) {
			MBEDTLS_MPI_CHK(mbedtls_mpi_grow(X, i + 1));
			p = X->p + i;
		}

		*p += c;
		c = (*p < c);
		i++;
		p++;
	}

cleanup:

	return ret;
}

/*
 * Helper for mbedtls_mpi subtraction
 */
static void mpi_sub_hlp(u32 n, mbedtls_mpi_uint *s, mbedtls_mpi_uint *d)
{
	u32 i;
	mbedtls_mpi_uint c, z;

	for (i = c = 0; i < n; i++, s++, d++) {
		z = (*d < c);
		*d -= c;
		c = (*d < *s) + z;
		*d -= *s;
	}

	while (c != 0) {
		z = (*d < c);
		*d -= c;
		c = z;
		d++;
	}
}

/*
 * Unsigned subtraction: X = |A| - |B|  (HAC 14.9)
 */
int mbedtls_mpi_sub_abs(mbedtls_mpi *X, const mbedtls_mpi *A,
	const mbedtls_mpi *B)
{
	mbedtls_mpi TB;
	int ret;
	u32 n;

	if (mbedtls_mpi_cmp_abs(A, B) < 0)
		return MBEDTLS_ERR_MPI_NEGATIVE_VALUE;

	mbedtls_mpi_init(&TB);

	if (X == B) {
		MBEDTLS_MPI_CHK(mbedtls_mpi_copy(&TB, B));
		B = &TB;
	}

	if (X != A)
	MBEDTLS_MPI_CHK(mbedtls_mpi_copy(X, A));

	/*
	 * X should always be positive as a result of unsigned subtractions.
	 */
	X->s = 1;

	ret = 0;

	for (n = B->n; n > 0; n--)
		if (B->p[n - 1] != 0)
		break;

	mpi_sub_hlp(n, B->p, X->p);

cleanup:

	mbedtls_mpi_free(&TB);

	return ret;
}

/*
 * Signed addition: X = A + B
 */
int mbedtls_mpi_add_mpi(mbedtls_mpi *X, const mbedtls_mpi *A,
	const mbedtls_mpi *B)
{
	int ret, s;

	s = A->s;
	if (A->s * B->s < 0) {
		if (mbedtls_mpi_cmp_abs(A, B) >= 0) {
			MBEDTLS_MPI_CHK(mbedtls_mpi_sub_abs(X, A, B));
			X->s = s;
		} else {
			MBEDTLS_MPI_CHK(mbedtls_mpi_sub_abs(X, B, A));
			X->s = -s;
		}
	} else {
		MBEDTLS_MPI_CHK(mbedtls_mpi_add_abs(X, A, B));
		X->s = s;
	}

cleanup:

	return ret;
}

/*
 * Signed subtraction: X = A - B
 */
int mbedtls_mpi_sub_mpi(mbedtls_mpi *X, const mbedtls_mpi *A,
	const mbedtls_mpi *B)
{
	int ret, s;

	s = A->s;
	if (A->s * B->s > 0) {
		if (mbedtls_mpi_cmp_abs(A, B) >= 0) {
			MBEDTLS_MPI_CHK(mbedtls_mpi_sub_abs(X, A, B));
			X->s = s;
		} else {
			MBEDTLS_MPI_CHK(mbedtls_mpi_sub_abs(X, B, A));
			X->s = -s;
		}
	} else {
		MBEDTLS_MPI_CHK(mbedtls_mpi_add_abs(X, A, B));
		X->s = s;
	}

cleanup:

	return ret;
}

/*
 * Helper for mbedtls_mpi multiplication
 */
void mpi_mul_hlp(u32 i, mbedtls_mpi_uint *s, mbedtls_mpi_uint *d,
	mbedtls_mpi_uint b)
{
	mbedtls_mpi_uint c = 0, t = 0;

	for (; i >= 16; i -= 16) {
		MULADDC_INIT
		MULADDC_CORE	MULADDC_CORE
		MULADDC_CORE	MULADDC_CORE
		MULADDC_CORE	MULADDC_CORE
		MULADDC_CORE	MULADDC_CORE

		MULADDC_CORE	MULADDC_CORE
		MULADDC_CORE	MULADDC_CORE
		MULADDC_CORE	MULADDC_CORE
		MULADDC_CORE	MULADDC_CORE
		MULADDC_STOP
	}

	for (; i >= 8; i -= 8) {
		MULADDC_INIT
		MULADDC_CORE	MULADDC_CORE
		MULADDC_CORE	MULADDC_CORE

		MULADDC_CORE	MULADDC_CORE
		MULADDC_CORE	MULADDC_CORE
		MULADDC_STOP
	}

	for (; i > 0; i--) {
		MULADDC_INIT
		MULADDC_CORE
		MULADDC_STOP
	}

	t++;

	do {
		*d += c;
		c = (*d < c);
		d++;
	} while (c != 0);
}

/*
 * Baseline multiplication: X = A * B  (HAC 14.12)
 */
int mbedtls_mpi_mul_mpi(mbedtls_mpi *X, const mbedtls_mpi *A,
	const mbedtls_mpi *B)
{
	int ret;
	u32 i, j;
	mbedtls_mpi TA, TB;

	mbedtls_mpi_init(&TA); mbedtls_mpi_init(&TB);

	if (X == A) {
		MBEDTLS_MPI_CHK(mbedtls_mpi_copy(&TA, A));
		A = &TA;
	}

	if (X == B) {
		MBEDTLS_MPI_CHK(mbedtls_mpi_copy(&TB, B));
		B = &TB;
	}

	for (i = A->n; i > 0; i--)
		if (A->p[i - 1] != 0)
			break;

	for (j = B->n; j > 0; j--)
		if (B->p[j - 1] != 0)
			break;

	MBEDTLS_MPI_CHK(mbedtls_mpi_grow(X, i + j));
	MBEDTLS_MPI_CHK(mbedtls_mpi_lset(X, 0));

	for (; j > 0; j--)
		mpi_mul_hlp(i, A->p, X->p + j - 1, B->p[j - 1]);

	X->s = A->s * B->s;

cleanup:

	mbedtls_mpi_free(&TB);
	mbedtls_mpi_free(&TA);

	return ret;
}

/*
 * Baseline multiplication: X = A * b
 */
int mbedtls_mpi_mul_int(mbedtls_mpi *X, const mbedtls_mpi *A,
	mbedtls_mpi_uint b)
{
	mbedtls_mpi _B;
	mbedtls_mpi_uint p[1];

	_B.s = 1;
	_B.n = 1;
	_B.p = p;
	p[0] = b;

	return mbedtls_mpi_mul_mpi(X, A, &_B);
}

/*
 * Unsigned integer divide - double mbedtls_mpi_uint dividend, u1/u0, and
 * mbedtls_mpi_uint divisor, d
 */
static mbedtls_mpi_uint mbedtls_int_div_int(mbedtls_mpi_uint u1,
	mbedtls_mpi_uint u0, mbedtls_mpi_uint d, mbedtls_mpi_uint *r)
{
	const mbedtls_mpi_uint radix = (mbedtls_mpi_uint) 1 << biH;
	const mbedtls_mpi_uint uint_halfword_mask = ((mbedtls_mpi_uint) 1 <<
		biH) - 1;
	mbedtls_mpi_uint d0, d1, q0, q1, rAX, r0, quotient;
	mbedtls_mpi_uint u0_msw, u0_lsw;
	u32 s;

	/*
	 * Check for overflow
	 */
	if (d == 0 || u1 >= d) {
		if (r != NULL)
			*r = ~0;

		return ~0;
	}

	/*
	 * Algorithm D, Section 4.3.1 - The Art of Computer Programming
	 *   Vol. 2 - Seminumerical Algorithms, Knuth
	 */

	/*
	 * Normalize the divisor, d, and dividend, u0, u1
	 */
	s = mbedtls_clz(d);
	d = d << s;

	u1 = u1 << s;
	u1 |= (u0 >> (biL - s)) & (-(mbedtls_mpi_sint)s >> (biL - 1));
	u0 = u0 << s;

	d1 = d >> biH;
	d0 = d & uint_halfword_mask;

	u0_msw = u0 >> biH;
	u0_lsw = u0 & uint_halfword_mask;

	/*
	 * Find the first quotient and remainder
	 */
	q1 = u1 / d1;
	r0 = u1 - d1 * q1;

	while (q1 >= radix || (q1 * d0 > radix * r0 + u0_msw)) {
		q1 -= 1;
		r0 += d1;

		if (r0 >= radix)
			break;
	}

	rAX = (u1 * radix) + (u0_msw - q1 * d);
	q0 = rAX / d1;
	r0 = rAX - q0 * d1;

	while (q0 >= radix || (q0 * d0 > radix * r0 + u0_lsw)) {
		q0 -= 1;
		r0 += d1;

		if (r0 >= radix)
			break;
	}

	if (r != NULL)
		*r = (rAX * radix + u0_lsw - q0 * d) >> s;

	quotient = q1 * radix + q0;

	return quotient;
}

/*
 * Division by mbedtls_mpi: A = Q * B + R  (HAC 14.20)
 */
int mbedtls_mpi_div_mpi(mbedtls_mpi *Q, mbedtls_mpi *R,
	const mbedtls_mpi *A,
	const mbedtls_mpi *B)
{
	int ret;
	u32 i, n, t, k;
	mbedtls_mpi X, Y, Z, T1, T2;

	if (mbedtls_mpi_cmp_int(B, 0) == 0)
		return MBEDTLS_ERR_MPI_DIVISION_BY_ZERO;

	mbedtls_mpi_init(&X);
	mbedtls_mpi_init(&Y);
	mbedtls_mpi_init(&Z);
	mbedtls_mpi_init(&T1);
	mbedtls_mpi_init(&T2);

	if (mbedtls_mpi_cmp_abs(A, B) < 0) {
		if (Q != NULL)
			MBEDTLS_MPI_CHK(mbedtls_mpi_lset(Q, 0));
		if (R != NULL)
			MBEDTLS_MPI_CHK(mbedtls_mpi_copy(R, A));
		return 0;
	}

	MBEDTLS_MPI_CHK(mbedtls_mpi_copy(&X, A));
	MBEDTLS_MPI_CHK(mbedtls_mpi_copy(&Y, B));
	X.s = Y.s = 1;

	MBEDTLS_MPI_CHK(mbedtls_mpi_grow(&Z, A->n + 2));
	MBEDTLS_MPI_CHK(mbedtls_mpi_lset(&Z, 0));
	MBEDTLS_MPI_CHK(mbedtls_mpi_grow(&T1, 2));
	MBEDTLS_MPI_CHK(mbedtls_mpi_grow(&T2, 3));

	k = mbedtls_mpi_bitlen(&Y) % biL;
	if (k < biL - 1) {
		k = biL - 1 - k;
		MBEDTLS_MPI_CHK(mbedtls_mpi_shift_l(&X, k));
		MBEDTLS_MPI_CHK(mbedtls_mpi_shift_l(&Y, k));
	} else {
		k = 0;
	}

	n = X.n - 1;
	t = Y.n - 1;
	MBEDTLS_MPI_CHK(mbedtls_mpi_shift_l(&Y, biL * (n - t)));

	while (mbedtls_mpi_cmp_mpi(&X, &Y) >= 0) {
		Z.p[n - t]++;
		MBEDTLS_MPI_CHK(mbedtls_mpi_sub_mpi(&X, &X, &Y));
	}
	MBEDTLS_MPI_CHK(mbedtls_mpi_shift_r(&Y, biL * (n - t)));

	for (i = n; i > t ; i--) {
		if (X.p[i] >= Y.p[t]) {
			Z.p[i - t - 1] = ~0;
		} else {
			Z.p[i - t - 1] = mbedtls_int_div_int(X.p[i], X.p[i - 1],
						Y.p[t], NULL);
		}

		Z.p[i - t - 1]++;
		do {
			Z.p[i - t - 1]--;

			MBEDTLS_MPI_CHK(mbedtls_mpi_lset(&T1, 0));
			T1.p[0] = (t < 1) ? 0 : Y.p[t - 1];
			T1.p[1] = Y.p[t];
			MBEDTLS_MPI_CHK(mbedtls_mpi_mul_int(&T1, &T1,
				Z.p[i - t - 1]));

			MBEDTLS_MPI_CHK(mbedtls_mpi_lset(&T2, 0));
			T2.p[0] = (i < 2) ? 0 : X.p[i - 2];
			T2.p[1] = (i < 1) ? 0 : X.p[i - 1];
			T2.p[2] = X.p[i];
		} while (mbedtls_mpi_cmp_mpi(&T1, &T2) > 0);

		MBEDTLS_MPI_CHK(mbedtls_mpi_mul_int(&T1, &Y, Z.p[i - t - 1]));
		MBEDTLS_MPI_CHK(mbedtls_mpi_shift_l(&T1,  biL * (i - t - 1)));
		MBEDTLS_MPI_CHK(mbedtls_mpi_sub_mpi(&X, &X, &T1));

		if (mbedtls_mpi_cmp_int(&X, 0) < 0) {
			MBEDTLS_MPI_CHK(mbedtls_mpi_copy(&T1, &Y));
			MBEDTLS_MPI_CHK(mbedtls_mpi_shift_l(&T1, biL *
				(i - t - 1)));
			MBEDTLS_MPI_CHK(mbedtls_mpi_add_mpi(&X, &X, &T1));
			Z.p[i - t - 1]--;
		}
	}

	if (Q != NULL) {
		MBEDTLS_MPI_CHK(mbedtls_mpi_copy(Q, &Z));
		Q->s = A->s * B->s;
	}

	if (R != NULL) {
		MBEDTLS_MPI_CHK(mbedtls_mpi_shift_r(&X, k));
		X.s = A->s;
		MBEDTLS_MPI_CHK(mbedtls_mpi_copy(R, &X));

		if (mbedtls_mpi_cmp_int(R, 0) == 0)
			R->s = 1;
	}

cleanup:

	mbedtls_mpi_free(&X);
	mbedtls_mpi_free(&Y);
	mbedtls_mpi_free(&Z);
	mbedtls_mpi_free(&T1);
	mbedtls_mpi_free(&T2);

	return ret;
}

/*
 * Modulo: R = A mod B
 */
int mbedtls_mpi_mod_mpi(mbedtls_mpi *R, const mbedtls_mpi *A,
	const mbedtls_mpi *B)
{
	int ret;

	if (mbedtls_mpi_cmp_int(B, 0) < 0)
		return MBEDTLS_ERR_MPI_NEGATIVE_VALUE;

	MBEDTLS_MPI_CHK(mbedtls_mpi_div_mpi(NULL, R, A, B));

	while (mbedtls_mpi_cmp_int(R, 0) < 0)
		MBEDTLS_MPI_CHK(mbedtls_mpi_add_mpi(R, R, B));

	while (mbedtls_mpi_cmp_mpi(R, B) >= 0)
		MBEDTLS_MPI_CHK(mbedtls_mpi_sub_mpi(R, R, B));

cleanup:

	return ret;
}

/*
 * Fast Montgomery initialization (thanks to Tom St Denis)
 */
static void mpi_montg_init(mbedtls_mpi_uint *mm, const mbedtls_mpi *N)
{
	mbedtls_mpi_uint x, m0 = N->p[0];
	unsigned int i;

	x = m0;
	x += ((m0 + 2) & 4) << 1;

	for (i = biL; i >= 8; i /= 2)
		x *= (2 - (m0 * x));

	*mm = ~x + 1;
}

/*
 * Montgomery multiplication: A = A * B * R^-1 mod N  (HAC 14.36)
 */
static int mpi_montmul(mbedtls_mpi *A, const mbedtls_mpi *B,
	const mbedtls_mpi *N, mbedtls_mpi_uint mm,
	const mbedtls_mpi *T)
{
	u32 i, n, m;
	mbedtls_mpi_uint u0, u1, *d;

	if (T->n < N->n + 1 || T->p == NULL)
		return MBEDTLS_ERR_MPI_BAD_INPUT_DATA;

	mmemset(T->p, 0, T->n * ciL);

	d = T->p;
	n = N->n;
	m = (B->n < n) ? B->n : n;

	for (i = 0; i < n; i++) {
		/*
		 * T = (T + u0*B + u1*N) / 2^biL
		 */
		u0 = A->p[i];
		u1 = (d[0] + u0 * B->p[0]) * mm;

		mpi_mul_hlp(m, B->p, d, u0);
		mpi_mul_hlp(n, N->p, d, u1);

		*d++ = u0;
		d[n + 1] = 0;
	}

	mmemcpy(A->p, d, (n + 1) * ciL);

	if (mbedtls_mpi_cmp_abs(A, N) >= 0)
		mpi_sub_hlp(n, N->p, A->p);
	else
		/* prevent timing attacks */
		mpi_sub_hlp(n, A->p, T->p);

	return 0;
}

/*
 * Montgomery reduction: A = A * R^-1 mod N
 */
static int mpi_montred(mbedtls_mpi *A, const mbedtls_mpi *N,
	mbedtls_mpi_uint mm, const mbedtls_mpi *T)
{
	mbedtls_mpi_uint z = 1;
	mbedtls_mpi U;

	U.n = U.s = (int) z;
	U.p = &z;

	return mpi_montmul(A, &U, N, mm, T);
}

/*
 * Sliding-window exponentiation: X = A^E mod N  (HAC 14.85)
 */
int mbedtls_mpi_exp_mod(mbedtls_mpi *X, const mbedtls_mpi *A,
	const mbedtls_mpi *E, const mbedtls_mpi *N,
	mbedtls_mpi *_RR)
{
	int ret;
	u32 wbits, wsize, one = 1;
	u32 i, j, nblimbs;
	u32 bufsize, nbits;
	mbedtls_mpi_uint ei, mm, state;
	mbedtls_mpi RR, T, W[2 << MBEDTLS_MPI_WINDOW_SIZE], Apos;
	int neg;

	if (mbedtls_mpi_cmp_int(N, 0) <= 0 || (N->p[0] & 1) == 0)
		return MBEDTLS_ERR_MPI_BAD_INPUT_DATA;

	if (mbedtls_mpi_cmp_int(E, 0) < 0)
		return MBEDTLS_ERR_MPI_BAD_INPUT_DATA;

	/*
	 * Init temps and window size
	 */
	mpi_montg_init(&mm, N);
	mbedtls_mpi_init(&RR);
	mbedtls_mpi_init(&T);
	mbedtls_mpi_init(&Apos);
	mmemset(W, 0, sizeof(W));

	i = mbedtls_mpi_bitlen(E);

	wsize = (i > 671) ? 6 : (i > 239) ? 5 :
		(i > 79) ? 4 : (i > 23) ? 3 : 1;

	if (wsize > MBEDTLS_MPI_WINDOW_SIZE)
		wsize = MBEDTLS_MPI_WINDOW_SIZE;

	j = N->n + 1;
	MBEDTLS_MPI_CHK(mbedtls_mpi_grow(X, j));
	MBEDTLS_MPI_CHK(mbedtls_mpi_grow(&W[1], j));
	MBEDTLS_MPI_CHK(mbedtls_mpi_grow(&T, j * 2));

	/*
	 * Compensate for negative A (and correct at the end)
	 */
	neg = (A->s == -1);
	if (neg) {
		MBEDTLS_MPI_CHK(mbedtls_mpi_copy(&Apos, A));
		Apos.s = 1;
		A = &Apos;
	}

	/*
	 * If 1st call, pre-compute R^2 mod N
	 */
	if ((_RR == NULL) || (_RR->p == NULL)) {
		MBEDTLS_MPI_CHK(mbedtls_mpi_lset(&RR, 1));
		MBEDTLS_MPI_CHK(mbedtls_mpi_shift_l(&RR, N->n * 2 * biL));
		MBEDTLS_MPI_CHK(mbedtls_mpi_mod_mpi(&RR, &RR, N));

		if (_RR != NULL)
			mmemcpy(_RR, &RR, sizeof(mbedtls_mpi));
	} else {
		mmemcpy(&RR, _RR, sizeof(mbedtls_mpi));
	}

	/*
	 * W[1] = A * R^2 * R^-1 mod N = A * R mod N
	 */
	if (mbedtls_mpi_cmp_mpi(A, N) >= 0)
		MBEDTLS_MPI_CHK(mbedtls_mpi_mod_mpi(&W[1], A, N));
	else
		MBEDTLS_MPI_CHK(mbedtls_mpi_copy(&W[1], A));

	MBEDTLS_MPI_CHK(mpi_montmul(&W[1], &RR, N, mm, &T));

	/*
	 * X = R^2 * R^-1 mod N = R mod N
	 */
	MBEDTLS_MPI_CHK(mbedtls_mpi_copy(X, &RR));
	MBEDTLS_MPI_CHK(mpi_montred(X, N, mm, &T));

	if (wsize > 1) {
		/*
		 * W[1 << (wsize - 1)] = W[1] ^ (wsize - 1)
		 */
		j = one << (wsize - 1);

		MBEDTLS_MPI_CHK(mbedtls_mpi_grow(&W[j], N->n + 1));
		MBEDTLS_MPI_CHK(mbedtls_mpi_copy(&W[j], &W[1]));

		for (i = 0; i < wsize - 1; i++)
			MBEDTLS_MPI_CHK(mpi_montmul(&W[j], &W[j], N, mm, &T));

		/*
		 * W[i] = W[i - 1] * W[1]
		 */
		for (i = j + 1; i < (one << wsize); i++) {
			MBEDTLS_MPI_CHK(mbedtls_mpi_grow(&W[i], N->n + 1));
			MBEDTLS_MPI_CHK(mbedtls_mpi_copy(&W[i], &W[i - 1]));

			MBEDTLS_MPI_CHK(mpi_montmul(&W[i], &W[1], N, mm, &T));
		}
	}

	nblimbs = E->n;
	bufsize = 0;
	nbits   = 0;
	wbits   = 0;
	state   = 0;

	while (1) {
		if (bufsize == 0) {
			if (nblimbs == 0)
				break;

			nblimbs--;

			bufsize = sizeof(mbedtls_mpi_uint) << 3;
		}

		bufsize--;

		ei = (E->p[nblimbs] >> bufsize) & 1;

		/*
		 * skip leading 0s
		 */
		if (ei == 0 && state == 0)
			continue;

		if (ei == 0 && state == 1) {
			/*
			 * out of window, square X
			 */
			MBEDTLS_MPI_CHK(mpi_montmul(X, X, N, mm, &T));
				continue;
		}

		/*
		 * add ei to current window
		 */
		state = 2;

		nbits++;
		wbits |= (ei << (wsize - nbits));

		if (nbits == wsize) {
			/*
			 * X = X^wsize R^-1 mod N
			 */
			for (i = 0; i < wsize; i++)
				MBEDTLS_MPI_CHK(mpi_montmul(X, X, N, mm, &T));

			/*
			 * X = X * W[wbits] R^-1 mod N
			 */
			MBEDTLS_MPI_CHK(mpi_montmul(X, &W[wbits], N, mm, &T));

			state--;
			nbits = 0;
			wbits = 0;
		}
	}

	/*
	 * process the remaining bits
	 */
	for (i = 0; i < nbits; i++) {
		MBEDTLS_MPI_CHK(mpi_montmul(X, X, N, mm, &T));

		wbits <<= 1;

		if ((wbits & (one << wsize)) != 0)
			MBEDTLS_MPI_CHK(mpi_montmul(X, &W[1], N, mm, &T));
	}

	/*
	 * X = A^E * R * R^-1 mod N = A^E mod N
	 */
	MBEDTLS_MPI_CHK(mpi_montred(X, N, mm, &T));

	if (neg && E->n != 0 && (E->p[0] & 1) != 0) {
		X->s = -1;
		MBEDTLS_MPI_CHK(mbedtls_mpi_add_mpi(X, N, X));
	}

cleanup:

	for (i = (one << (wsize - 1)); i < (one << wsize); i++)
		mbedtls_mpi_free(&W[i]);

	mbedtls_mpi_free(&W[1]);
	mbedtls_mpi_free(&T);
	mbedtls_mpi_free(&Apos);

	if ((_RR == NULL) || (_RR->p == NULL))
		mbedtls_mpi_free(&RR);

	return ret;
}
