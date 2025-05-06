#pragma once

#include "main.h"

#define _RX_BUF_SIZE 32
#define _TX_BUF_SIZE 32

extern char RX_buff[_RX_BUF_SIZE];
extern uint8_t RX_put_ptr;
extern uint8_t RX_get_ptr;
extern volatile uint8_t RXchar;

#define RX_available() (RX_put_ptr - RX_get_ptr)

void RX_init(void);
uint8_t RX_read(void);
