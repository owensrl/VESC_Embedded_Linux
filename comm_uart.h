/*
 * comm_uart.h
 *
 *  Created on: 17 aug 2015
 *      Author: benjamin
 *  Modified on: 7 MAR 2017
 *      By: Ryan Owens
 *          - Added function to close fd
 *          - Added function to receive data
 */

#ifndef COMM_UART_H_
#define COMM_UART_H_

#ifdef __cplusplus
extern "C" {
#endif

// Functions
void comm_uart_init(char* modemDevice);
void comm_uart_close(void);
int receive_packet(void);

#ifdef __cplusplus
}
#endif
#endif /* COMM_UART_H_ */
