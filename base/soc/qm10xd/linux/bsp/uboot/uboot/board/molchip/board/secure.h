#ifndef _SECURE_H_
#define _SECURE_H_

#define RSA_PULIC_KEY_LENGTH		256
#define RSA_KEY_N_LENGTH		RSA_PULIC_KEY_LENGTH
#define RSA_KEY_D_LENGTH		RSA_PULIC_KEY_LENGTH
#define RSA_CIPHER_LENGTH		RSA_PULIC_KEY_LENGTH
#define RSA_DECRPT_LENGTH		RSA_PULIC_KEY_LENGTH
#define RSA_E_LNENGTH			4
#define RSA_SIGN			1

struct rsa_pub_key {
	u8 rsa_n[RSA_KEY_N_LENGTH];
	u8 rsa_e[RSA_E_LNENGTH];
};

struct rsa_sign_info {
	u8 rsa_cipher[RSA_CIPHER_LENGTH];	//cipher text
	struct rsa_pub_key rsa_key;	//rsa public key
};

extern u8 aes_cbc_key[16];
extern u32 aes_cbc_key_pos;

bool read_efuse_hash(u8 *hash);
bool secure_verify(struct boot_header *header);
bool signature_verify(struct boot_header *header);

bool public_key_verify(struct boot_header *header);
bool normal_boot(u32 addr);

#endif
