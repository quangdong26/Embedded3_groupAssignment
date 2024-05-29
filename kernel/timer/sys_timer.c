//--------------------------------sys_timer.c----------------------------------//
#include "../kernel/timer/sys_timer.h"


const uint32_t interval_1 = CLK_HZ;
uint32_t cur_val_1 = 0;

const uint32_t interval_3 = CLK_HZ/4;
uint32_t cur_val_3 = 0;

void timer_init() {
    // clear timer status
    TIMER_CONTROL_STATUS &= ~(0xF<<0); //clear status of timer
    
    // update next value for timer 1
    cur_val_1 = TIMER_COUNTER_LOW;
    cur_val_1 += interval_1;
    TIMER_COMPARE_1 += cur_val_1;
    
    // update next value for timer 3
    cur_val_3 = TIMER_COUNTER_LOW;
    cur_val_3 += interval_3;
    TIMER_COMPARE_3 += cur_val_3;
}

void handle_timer_1() {
    cur_val_1 += interval_1;
    TIMER_COMPARE_1 = cur_val_1;
    TIMER_CONTROL_STATUS |= (1 << SYS_TIMER_MATCH_1_INT);
    uart_puts("Timer 1.\n");
}

void handle_timer_3() {
    cur_val_3 += interval_3;
    TIMER_COMPARE_3 = cur_val_3;
    TIMER_CONTROL_STATUS |= (1 << SYS_TIMER_MATCH_3_INT);
    uart_puts("Timer 3.\n");
}

// Read the register and get the current count value
uint64_t timer_get_tick(){
    uint32_t high = TIMER_COUNTER_HIGH;
    uint32_t low = TIMER_COUNTER_LOW;
    
    // double check if value is updated
    if (high != TIMER_COUNTER_HIGH){
        high = TIMER_COUNTER_HIGH;
        low = TIMER_COUNTER_LOW;
    }
    
    return ((uint64_t)high << 32) | low;
}

// delay function using timer count value
void delay_ms(uint32_t ms){ //ms
    uint64_t start = timer_get_tick();
    
    // update and delay the time creating a busy loop
    while(timer_get_tick() < start + (ms*1000)) {
        asm volatile("nop");
    }
}