#ifndef __SCHED_INTERNAL_H__
#define __SCHED_INTERNAL_H__


#define BOOT_PROC_NODE_MAX		(4)

typedef enum module_state {
	MOD_STS_IDLE = 0,
	MOD_STS_BUSY,
	MOD_STS_DONE,
	MOD_STS_INVALID
} MOD_STATE_E;

typedef enum buffer_state {
	BUF_STS_IDLE = 0,
	BUF_STS_BUSY,
	BUF_STS_READY
}BUF_STATE_E;

struct sched_buffer {
	BUF_STATE_E state;
	unsigned int seq;
	void *buffer;
};

struct sched_node_runtime {
	unsigned int cur_handle_size;
	unsigned int seq;
	struct sched_buffer *cur_buf;
	struct sched_buffer *pre_buf;
};

enum sched_buffer_mode {
	PP_BUF_MODE = 0x1,
	CONTI_MEM_MODE
};

struct sched_node {
	MOD_STATE_E state;
	/* output mode. */
	unsigned char in_mode;
	unsigned char out_mode;
	struct sched_buffer pp_buf[2];
	struct sched_node *prev;
	struct sched_node *next;
	unsigned int src_base;
	unsigned int dst_base;
	unsigned int total_size;
	unsigned int finished_size;
	unsigned int actual_out_size;

	struct sched_node_runtime runtime;

};


#define SFC_NODE_INIT(header, out_m, size) do { \
	sfc_node.out_mode = out_m; \
	sfc_node.total_size = size; \
	sfc_node.finished_size = 0; \
	sfc_node.dst_base = 0; \
	sfc_node.src_base = (header)->data_src; \
} while (0)

#define AES_NODE_INIT(header, in_m, out_m, size)	do { \
	(aes_node).in_mode = in_m; \
	(aes_node).out_mode = out_m; \
	(aes_node).src_base = (header)->aes_src; \
	(aes_node).dst_base = (header)->aes_dst; \
	(aes_node).total_size = size; \
	(aes_node).finished_size = 0; \
	(aes_node).actual_out_size = 0; \
} while (0)

#define LZMA_NODE_INIT(header, in_m, out_m, size) do { \
	lzma_node.in_mode = in_m; \
	lzma_node.out_mode = out_m; \
	lzma_node.src_base = (header)->lzma_src; \
	lzma_node.dst_base = (header)->lzma_dst; \
	lzma_node.total_size = size; \
	lzma_node.finished_size = 0; \
	lzma_node.actual_out_size=0;\
} while (0)

#define HASH_NODE_INIT(header, in_m, size) do { \
	hash_node.src_base = (header)->lzma_dst; \
	hash_node.total_size = size; \
	hash_node.finished_size = 0; \
	hash_node.in_mode = in_m; \
} while (0)


#define VMEM_NODE_INIT(header, out_m, size) do { \
	mem_node.src_base = (header)->aes_src; \
	mem_node.total_size = size; \
	mem_node.finished_size = 0; \
	mem_node.out_mode = out_m; \
	mem_node.actual_out_size=0;\
} while (0)
#define VMEM_HASH_NODE_INIT(header, in_m, size) do { \
	hash_node.src_base = (header)->aes_dst; \
	hash_node.total_size = size; \
	hash_node.finished_size = 0; \
	hash_node.in_mode = in_m; \
} while (0)
#endif
