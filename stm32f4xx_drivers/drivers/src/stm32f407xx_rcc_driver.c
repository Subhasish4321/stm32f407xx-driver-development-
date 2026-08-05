#include "stm32f407xx_rcc_driver.h"

uint16_t AHB1_PreScaler [8] = {2,4,8,16,64,128,256,512};
uint8_t APB1_PreScaler [4] = {2,4,8,16};
uint8_t APB2_PreScaler [4] = {2,4,8,16};

uint32_t RCC_GetPLLOutputClock(void)
{
    return 0;
}

/** 
  * RCC_Get_PCLK1Value
  * -------------------
  * Returns APB1 peripheral clock frequency (PCLK1) in Hz.
  *
  * Algorithm / steps:
  *  1) Determine system clock source from RCC->CFGR (SWS/PLLSRC bits):
  *     - 0 => HSI (internal RC, 16 MHz)
  *     - 1 => HSE (external crystal, typically 8 MHz here)
  *     - 2 => PLL  (output frequency from PLL)
  *     If PLL is selected, call RCC_GetPLLOutputClock() to obtain SystemClk.
  *
  *  2) Read AHB prescaler (HPRE) from RCC->CFGR and map to divisor:
  *     - If HPRE < 8, AHB divisor = 1 (no division)
  *     - Else divisor = AHB1_PreScaler[HPRE - 8]
  *
  *  3) Read APB1 prescaler (PPRE1 bits) and map to divisor:
  *     - If PPRE1 < 4, APB1 divisor = 1
  *     - Else divisor = APB1_PreScaler[PPRE1 - 4]
  *
  *  4) Compute HCLK = SystemClk / AHB_divisor, then PCLK1 = HCLK / APB1_divisor.
  *
  * Notes:
  *  - Uses AHB1_PreScaler and APB1_PreScaler arrays defined at top of file.
  *  - The returned PCLK1 value (Hz) is used to program peripheral timing
  *    (for example: I2C_CR2 frequency field, CCR and TRISE calculations).
  */

uint32_t RCC_Get_PCLK1Value(void)
{
    uint32_t pclk1, SystemClk;
    uint8_t clksrc, temp, ahb1p, apb1p;

    /* 1) System clock source selection */
    clksrc = ((RCC->CFGR >> 2) & 0x3);
    if (clksrc == 0)
    {
        /* HSI selected as system clock */
        SystemClk = 16000000U;
    }
    else if (clksrc == 1)
    {
        /* HSE selected as system clock */
        SystemClk = 8000000U;
    }
    else /* clksrc == 2 */
    {
        /* PLL selected as system clock -> get PLL output */
        SystemClk = RCC_GetPLLOutputClock();
    }

    /* 2) AHB prescaler (HPRE) */
    temp = ((RCC->CFGR >> 4) & 0xF);
    if (temp < 8)
    {
        ahb1p = 1U; /* no division */
    }
    else
    {
        ahb1p = AHB1_PreScaler[temp - 8];
    }

    /* 3) APB1 prescaler (PPRE1) */
    temp = ((RCC->CFGR >> 10) & 0x7);
    if (temp < 4)
    {
        apb1p = 1U; /* no division */
    }
    else
    {
        apb1p = APB1_PreScaler[temp - 4];
    }

    /* 4) Compute PCLK1 = (SystemClk / AHB_div) / APB1_div */
    pclk1 = (SystemClk / ahb1p) / apb1p;

    return pclk1;
}

uint32_t RCC_Get_PCLK2Value(void)
{
    uint32_t pclk2, SystemClk;
    uint8_t clksrc, temp, ahb1p, apb2p;

    /* 1) System clock source selection */
    clksrc = ((RCC->CFGR >> 2) & 0x3);
    if (clksrc == 0)
    {
        /* HSI selected as system clock */
        SystemClk = 16000000U;
    }
    else if (clksrc == 1)
    {
        /* HSE selected as system clock */
        SystemClk = 8000000U;
    }
    else /* clksrc == 2 */
    {
        /* PLL selected as system clock -> get PLL output */
        SystemClk = RCC_GetPLLOutputClock();
    }

    /* 2) AHB prescaler (HPRE) */
    temp = ((RCC->CFGR >> 4) & 0xF);
    if (temp < 8)
    {
        ahb1p = 1U; /* no division */
    }
    else
    {
        ahb1p = AHB1_PreScaler[temp - 8];
    }

    /* 3) APB2 prescaler (PPRE2) */
    temp = ((RCC->CFGR >> 13) & 0x7);
    if (temp < 4)
    {
        apb2p = 1U; /* no division */
    }
    else
    {
        apb2p = APB2_PreScaler[temp - 4];
    }

    /* 4) Compute PCLK2 = (SystemClk / AHB_div) / APB2_div */
    pclk2 = (SystemClk / ahb1p) / apb2p;

    return pclk2;
}