//----------------------------------irq.h----------------------------------//
#ifndef IRQ_H
#define IRQ_H

#include "../timer/sys_timer.h"

#define IRQ_BASE            0x3F00B000

// Registers
#define IRQ_BASIC_PENDING   (*(volatile unsigned int*)(IRQ_BASE+0x200)) // R
#define IRQ_PENDING_1       (*(volatile unsigned int*)(IRQ_BASE+0x204)) // R
#define IRQ_PENDING_2       (*(volatile unsigned int*)(IRQ_BASE+0x208)) // R

// FIQ control
#define FIQ_CONTROL         (*(volatile unsigned int*)(IRQ_BASE+0x20C)) // R/W 6:0

// IRQ enable
#define ENABLE_IRQS_1      (*(volatile unsigned int*)(IRQ_BASE+0x210)) // Enable interrupt 31:0
#define ENABLE_IRQS_2      (*(volatile unsigned int*)(IRQ_BASE+0x214)) // Enable interrupt 63:32
#define ENABLE_BASIC_IRQS  (*(volatile unsigned int*)(IRQ_BASE+0x218))

// IRQ disable
#define DISABLE_IRQS_1     (*(volatile unsigned int*)(IRQ_BASE+0x21C))
#define DISABLE_IRQS_2     (*(volatile unsigned int*)(IRQ_BASE+0x220))
#define DISABLE_BASIC_IRQS (*(volatile unsigned int*)(IRQ_BASE+0x224))

// ARM peripherals interrupt
// System Timer Interrupts
#define SYS_TIMER_MATCH_1_INT   1
#define SYS_TIMER_MATCH_3_INT   3
#define USB_CONTROLLER_INT      9
#define AUX_INT                 29
#define I2C_SPI_SLV_INT         43
#define PWA0_INT                45
#define PWA1_INT                46
#define SMI_INT                 48
#define GPIO_INT_0              49
#define GPIO_INT_1              50
#define GPIO_INT_2              51
#define GPIO_INT_3              52
#define I2C_INT                 53
#define SPI_INT                 54
#define PCM_INT                 55
#define UART_INT                57

// Function prototypes
void enable_irq();
void disable_irq();
void handle_irq();

#endif /* IRQ_H */