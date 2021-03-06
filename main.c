#include <stdint.h>
/* defines structures corresponding to memory mapped registers */
#include "CMSIS/TM4C123GH6PM.h"
#include "bsp.h"
#include "intrinsics.h"

/* powers up GPIO port F and configures it for output */
void mcu_setup(void);


/* entry point */
int main()
{
    mcu_setup();
    
    SysTick_Handler();
    // blink the red led
    while(1)
    {
        // atomic operations that directly write to 
        // memory withoul any loading first
        GPIOF_AHB->DATA_Bits[GREEN_LED] = GREEN_LED;
        GPIOF_AHB->DATA_Bits[GREEN_LED] = 0U;
        
        // non-atomic operations, load-modify-write
        // could interfere with the SysTick_Handler() ISR
        // which also uses GPIOF_AHB->DATA
        // GPIOF_AHB->DATA |= GREEN_LED;
        // GPIOF_AHB->DATA &= (~ GREEN_LED);
        
        // we can solve this by disabling interrupts while 
        // accessing the shared data
        // __disable_interrupt();
        // GPIOF_AHB->DATA |= GREEN_LED;
        // __enable_interrupt();
        
        // __disable_interrupt();
        // GPIOF_AHB->DATA &= (~ GREEN_LED);
        // __enable_interrupt();
        // atomic operations are better in any case
    }
    
    // return 0;
}


void mcu_setup(){
    // enable the GPIO port F (clock gating control register)
    SYSCTL->RCGC2 |= 0x00000020;
    // enable AHB for GPIO port F
    SYSCTL->GPIOHBCTL = (1U << 5);
    // set GPIO port F's data direction as output for 3 pins
    GPIOF_AHB->DIR |= 0x0000000e;
    // set GPIO port F's digital enable for 3 bits
    GPIOF_AHB->DEN |= 0x0000000e;
    
    // setup the system tick timer
    // this timer will call the SysTick_Handler ISR each tick
    // half a second timer value
    SysTick->LOAD   = SYS_CLOCK_HZ / 2U - 1U;
    SysTick->VAL    = 0U;
    SysTick->CTRL   = (1U << 2) | (1U << 1) | 1U;
    
    // enable interrups using this intrinsic (clearing PRIMASK bit)
    __enable_interrupt();
}
