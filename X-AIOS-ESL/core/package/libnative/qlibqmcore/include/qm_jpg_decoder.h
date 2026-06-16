
#ifndef _qm_jpg_decoder_h_
#define _qm_jpg_decoder_h_

int qm_jpg_decoder(const char *image_path, void **pData, int *pLen, short *pWidth, short *pHeight);
void deinit_jpge_pipeline(void);

#endif //_qm_jpg_decoder_h_
