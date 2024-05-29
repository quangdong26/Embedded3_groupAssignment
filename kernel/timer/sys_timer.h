//--------------------------------sys_timer.h--------------------------------//
#ifndef SYS_TIMER_H
#define SYS_TIMER_H

#include "../uart/uart1.h"
#include "../uart/uart0.h"
#include "../gpio/gpio.h"
#include "../irq/irq.h"

#define CLK_HZ 1000000
#define TIMER_BASE 0x3F003000

// Timer registers
#define TIMER_CONTROL_STATUS (*(volatile unsigned int *)(TIMER_BASE + 0x0)) // R/W 3:0
#define TIMER_COUNTER_LOW (*(volatile unsigned int *)(TIMER_BASE + 0x4)) // R
#define TIMER_COUNTER_HIGH (*(volatile unsigned int *)(TIMER_BASE + 0x8)) // R
#define TIMER_COMPARE_0 (*(volatile unsigned int *)(TIMER_BASE + 0xC)) // R/W
#define TIMER_COMPARE_1 (*(volatile unsigned int *)(TIMER_BASE + 0x10)) // R/W
#define TIMER_COMPARE_2 (*(volatile unsigned int *)(TIMER_BASE + 0x14)) // R/W
#define TIMER_COMPARE_3 (*(volatile unsigned int *)(TIMER_BASE + 0x18)) // R/W

// Timer control/status bits
#define SYS_TIMER_MATCH_0_INT 0
#define SYS_TIMER_MATCH_1_INT 1
#define SYS_TIMER_MATCH_2_INT 2
#define SYS_TIMER_MATCH_3_INT 3

// Function prototypes
void timer_init();
void handle_timer_1();
void handle_timer_3();
uint64_t timer_get_tick();
void delay_ms(uint32_t ms);

#endif