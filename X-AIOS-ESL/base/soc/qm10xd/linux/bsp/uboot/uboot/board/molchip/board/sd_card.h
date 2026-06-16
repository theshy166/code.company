#ifndef SD_CARD_H
#define SD_CARD_H

int unstuff_bits(u32 *resp, u32 start, u32 size);
int mmc_go_idle(void);
int mmc_send_if_cond(u32 ocr);
int mmc_app_cmd(u32 rca);
int mmc_send_app_op_cond(u32 ocr, u32 *rocr);
int mmc_send_cxd_native(u32 arg, u32 *cxd, int opcode);
int mmc_send_cid(u32 *cid);
int mmc_send_relative_addr(unsigned int *rca);
int mmc_send_csd(u32 rca, u32 *csd);
int mmc_select_card(u32 rca);
int mmc_set_blocklen(u32 rca, unsigned int blocklen);
int mmc_app_set_bus_width(u32 rca, int width);
int mmc_blk_mq_issue_rw_rq(u32 req, u32 addr, u32 sector, u32 len);
int init_sdcard(u32 *rca, u32 *csd);
int sdcard_boot(u32 boot_mode);
int sdio_init(void *data);
int sdio_write(unsigned int offset, unsigned char *buf, unsigned int size);

#endif

