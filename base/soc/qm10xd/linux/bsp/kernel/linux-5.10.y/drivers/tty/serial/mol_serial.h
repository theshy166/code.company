#ifndef MOL_SERIAL_H_
#define MOL_SERIAL_H_

struct mol_uart_port {
	struct uart_port port;
	char name[16];
	//struct uart_fh_dma *dma;
	/* For 485 */
	bool	rs485_en;
	int 	rs485_en_gpio;
	int	ch_drop_flag;
	int	baudrate;
};

enum uart_id {
	UART_DEV0 = 0,
	UART_DEV1,
	UART_DEV2,
	UART_DEV3,
};

void mol_uart_stop_tx(struct uart_port *port);

#endif /* MOL_SERIAL_H_ */
