/* Board Support Package */
#include "CMSIS/TM4C123GH6PM.h"
#include "bsp.h"

__stackless void assert_failed(char const *file, int line)
{
    /* damage control functionality here */
    NVIC_SystemReset();
}

/*************************** My ISRs *******************************/

void SysTick_Handler(void)
{
    // toggle using bit-addressing with array indexing syntax
    GPIOF_AHB->DATA_Bits[RED_LED] ^= RED_LED;
}