#include "mci.h"
#include "type_use.h"
#include <linux/string.h>
#include "glb_reg.h"
#include "system.h"
#include "secure.h"
#include "uart_drv.h"
#include "aes.h"
#include "pkcs5.h"
#include "rsa.h"
#include "bignum.h"
#include "sha.h"
#include "efuse.h"
#include "efuse_drv.h"
//#include "dma.h"
#include "hash.h"
#include "lzma.h"
#include "spi_nand_flash.h"
#include "mc_hw_rsa.h"


u8 calc_hash[32] = { 0 };
u8 corr_hash[32] = { 0 };
u8 aes_cbc_key[16] = { 0 };
volatile u32 (*rsa_hw_sel) = (volatile u32 *)(0x30048008);
void (*mbedtls_rsa_init)(rsa_context *, int , int ) = 
(void (*)(rsa_context *, int, int))(0x98b8);
int (*rsa_pkcs1_verify)(rsa_context *,	const unsigned char *, const unsigned char *, struct boot_header *) = 
(int (*)(rsa_context *,	const unsigned char *, const unsigned char *, struct boot_header *))(0xa880);
int (*sha256_ctx)(const u8 *in, unsigned int , u8 *out) = 
(int (*)(const u8 *, unsigned int, u8 *))(0x52ac);
int sha256_ctx_continue(const u8 *in, unsigned int len)
{
	return 0;
}

bool rsa_decrypte(struct rsa_sign_info *sign_info, u8 *hash, struct boot_header *header)
{
	rsa_context rsa;
	u8 *rsa_ciphertext;
	bool ret = true;

	mbedtls_rsa_init(&rsa, MBEDTLS_RSA_PKCS_V21, MBEDTLS_MD_SHA256);

	rsa.len = RSA_PULIC_KEY_LENGTH;
	rsa.N.s = RSA_SIGN;
	rsa.N.n = RSA_KEY_N_LENGTH >> 2;
	rsa.N.p = (mbedtls_mpi_uint *)(sign_info->rsa_key.rsa_n);

	rsa.E.s = RSA_SIGN;
	rsa.E.n = RSA_E_LNENGTH >> 2;
	rsa.E.p = (mbedtls_mpi_uint *)(sign_info->rsa_key.rsa_e);

	rsa_ciphertext = sign_info->rsa_cipher;
	if (rsa_pkcs1_verify(&rsa, hash, rsa_ciphertext,header) != 0) {
		ret = false;
		goto exit;
	}

exit:
	return ret;
}

bool aes_iv_valid(u8 *iv, u32 len)
{
	u32 i;

	for (i = 0; i < len; i++) {
		if (iv[i] != 0)
			return true;
	}

	return false;
}

bool public_key_verify(struct boot_header *header)
{

	struct rsa_sign_info *sign_info;
	bool ret;	

	u8 calc_hash[32] = {0};

	sign_info = (struct rsa_sign_info *) ((unsigned int)header + 0x400);

	if (header->sel & BIT(0)) {
		ret = hash_cfg(27, (unsigned int )(&sign_info->rsa_key), sizeof(struct rsa_pub_key));
		hash_start();
		ret = hash_done((header->sel >> 17) & 0xFFF,27);
		
		if(ret == false)
			return false;
			
		read_result((u32 *)calc_hash, HASH_SHA256);
		
	} else {
		sha256_ctx((u8 *)&sign_info->rsa_key, sizeof(struct rsa_pub_key),
		calc_hash);
	}

	ret = efuse_compare((u32 *)calc_hash);
	return ret;
}


bool signature_verify(struct boot_header *header)
{
	struct rsa_sign_info *sign_info;
	bool ret;
	
	sign_info = (struct rsa_sign_info *) ((unsigned int)header + 0x400);
	*rsa_hw_sel = (header->sel);

	ret = rsa_decrypte(sign_info, corr_hash,header);
	if (ret == false) {
		uart_puts("RDF\r\n");
		memset(sign_info, 0, sizeof(struct rsa_sign_info));
		return false;
	}

	return true;
}

bool secure_verify(struct boot_header *header)
{
	struct rsa_sign_info *sign_info;
	bool ret;
	u8 corr_hash[32] = {0};
	
	*rsa_hw_sel = header->sel;

	sign_info = (struct rsa_sign_info *) ((unsigned int)header + 0x400);

	if (aes_sel())
	{
		aes_cbc_decode(header->aes_src, header->aes_dst, header->decode_length);
	}

	if (header->lzma)
	{
		lzma_cfg(header->lzma_src, header->lzma_dst, header);
		lzma_start();
		ret = lzma_done((header->lzma_ctrl & 0xFFFF));
		if (false == ret){
			uart_puts("LND\r\n");
			return false;
		}

	}

	if (rsa_sel())
	{
		hash_enable((header->sel >> 29) & 0x3);
		rsa_enable((header->sel >> 1) & 0x7);
		ret = public_key_verify(header);
		if (ret == false)
		{
			uart_puts("HCF\r\n");
			return false;
		}



		if(header->sel & BIT(0)){
			if (HASH_GLITCH_EXIST == header->rev0) {
				sha256_ctx((u8 *)header->addr, header->length, corr_hash);
			} else {

				ret = hash_cfg(26, header->addr, header->length);
				hash_start();
				ret = hash_done((header->sel >> 17) & 0xFFF,26);
			
				if(ret == false){
					return false;
				}
				read_result((u32 *)corr_hash, HASH_SHA256);
			}
		}else{

			sha256_ctx((u8 *)header->addr, header->length, corr_hash);

		}

		ret = rsa_decrypte(sign_info, corr_hash,header);
		if (ret == false) {
			uart_puts("RDF\r\n");
			memset(sign_info, 0, sizeof(struct rsa_sign_info));
		
			return false;
		}
		
	}
	return true;
}


