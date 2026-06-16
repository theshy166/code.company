
#ifndef __RESC_IMG_H_
#define __RESC_IMG_H_

/*
 * mc_read_resource_file - read file from resource partition
 *
 * @buf: destination buf to store file data
 * @name: file name
 * @offset: blocks offset in the file
 * @len: the size(by bytes) of file to read.
 *
 * return negative num on failed, otherwise the file size
 */
int mc_read_resource_file(void *buf, const char *name, int offset, int len);

#endif
