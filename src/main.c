#ifdef __USE_CMSIS
#include "LPC17xx.h"
#endif

#include <cr_section_macros.h>

#include <lpc17xx_adc.h>
#include <lpc17xx_gpdma.h>
#include <lpc17xx_gpio.h>
#include <lpc17xx_pinsel.h>
#include <lpc17xx_ssp.h>
#include <lpc17xx_systick.h>
#include <lpc17xx_timer.h>

#include "fsm.h"
#include "lcd5110.h"
#include "menu.h"


#define SEL (1 << 12)
#define UP (1 << 11)
#define DOWN (1 << 10)


void init_gpio(void);
void init_spi(void);
void init_dma(void);
void init_adc(void);
void init_timers(void);
void init_systick(void);
void init_interrupts(void);

FSM_t fsm;

GPDMA_LLI_Type adc_lli;

//uint32_t *adc_data = (uint32_t *) 0x2007C000;

int main(void)
{
    for (int i=0; i<1024; i++)
        *(adc_ph_data + i) = (uint32_t) 0;

    LCD5110_t lcd5110;

    init_gpio();
    init_spi();
    init_dma();
    init_adc();
    init_timers();
    init_systick();
    init_interrupts();

    init_lcd5110(&lcd5110, LPC_SSP1);
    init_fsm(&fsm, &lcd5110);

    lcd5110.print_str("initializing");
    for (int i=0; i<10000000; i++);
    lcd5110.clear();

    fsm.run(&fsm);

  //  for(;;);
}


void init_gpio(void)
{
    // lcd5110 SPI GPIO
    PINSEL_CFG_Type p0_9_cfg = {.Portnum=PINSEL_PORT_0, .Pinnum=PINSEL_PIN_9, .Funcnum=PINSEL_FUNC_2, .Pinmode=PINSEL_PINMODE_TRISTATE, .OpenDrain=PINSEL_PINMODE_NORMAL}; // SPI MOSI_1
    PINSEL_ConfigPin(&p0_9_cfg);
    PINSEL_CFG_Type p0_8_cfg = {.Portnum=PINSEL_PORT_0, .Pinnum=PINSEL_PIN_8, .Funcnum=PINSEL_FUNC_2, .Pinmode=PINSEL_PINMODE_TRISTATE, .OpenDrain=PINSEL_PINMODE_NORMAL}; // SPI MISO_1
    PINSEL_ConfigPin(&p0_8_cfg);
    PINSEL_CFG_Type p0_7_cfg = {.Portnum=PINSEL_PORT_0, .Pinnum=PINSEL_PIN_7, .Funcnum=PINSEL_FUNC_2, .Pinmode=PINSEL_PINMODE_TRISTATE, .OpenDrain=PINSEL_PINMODE_NORMAL}; // SPI SCK_1
    PINSEL_ConfigPin(&p0_7_cfg);
    PINSEL_CFG_Type p0_6_cfg = {.Portnum=PINSEL_PORT_0, .Pinnum=PINSEL_PIN_6, .Funcnum=PINSEL_FUNC_2, .Pinmode=PINSEL_PINMODE_TRISTATE, .OpenDrain=PINSEL_PINMODE_NORMAL}; // SPI SSEL_1
    PINSEL_ConfigPin(&p0_6_cfg);
    PINSEL_CFG_Type p0_3_cfg = {.Portnum=PINSEL_PORT_0, .Pinnum=PINSEL_PIN_3, .Funcnum=PINSEL_FUNC_0, .Pinmode=PINSEL_PINMODE_TRISTATE, .OpenDrain=PINSEL_PINMODE_NORMAL}; // LCD5110 RST
    PINSEL_ConfigPin(&p0_3_cfg);
    PINSEL_CFG_Type p0_2_cfg = {.Portnum=PINSEL_PORT_0, .Pinnum=PINSEL_PIN_2, .Funcnum=PINSEL_FUNC_0, .Pinmode=PINSEL_PINMODE_TRISTATE, .OpenDrain=PINSEL_PINMODE_NORMAL}; // LCD5110 D/C
    PINSEL_ConfigPin(&p0_2_cfg);

    GPIO_SetDir(0, DC, 1);
    GPIO_SetDir(0, RST, 1);
    GPIO_SetValue(0, RST);

    // ADC pH sensor GPIO
    PINSEL_CFG_Type p0_23_cfg = {.Portnum=PINSEL_PORT_0, .Pinnum=PINSEL_PIN_23, .Funcnum=PINSEL_FUNC_1, .Pinmode=PINSEL_PINMODE_TRISTATE, .OpenDrain=PINSEL_PINMODE_NORMAL}; //AIN0
    PINSEL_ConfigPin(&p0_23_cfg);

    // Buttons GPIO
    PINSEL_CFG_Type p2_12_cfg = {.Portnum=PINSEL_PORT_2, .Pinnum=PINSEL_PIN_12, .Funcnum=PINSEL_FUNC_0, .Pinmode=PINSEL_PINMODE_PULLUP, .OpenDrain=PINSEL_PINMODE_NORMAL}; // SEL
    PINSEL_ConfigPin(&p2_12_cfg);
    PINSEL_CFG_Type p2_11_cfg = {.Portnum=PINSEL_PORT_2, .Pinnum=PINSEL_PIN_11, .Funcnum=PINSEL_FUNC_0, .Pinmode=PINSEL_PINMODE_PULLUP, .OpenDrain=PINSEL_PINMODE_NORMAL}; // UP
    PINSEL_ConfigPin(&p2_11_cfg);
    PINSEL_CFG_Type p2_10_cfg = {.Portnum=PINSEL_PORT_2, .Pinnum=PINSEL_PIN_10, .Funcnum=PINSEL_FUNC_0, .Pinmode=PINSEL_PINMODE_PULLUP, .OpenDrain=PINSEL_PINMODE_NORMAL}; // DOWN
    PINSEL_ConfigPin(&p2_10_cfg);

    GPIO_SetDir(2, SEL, 0);
    GPIO_SetDir(2, UP, 0);
    GPIO_SetDir(2, DOWN, 0);
    GPIO_IntCmd(2, SEL | UP | DOWN, 1);
}


void init_spi(void)
{
    SSP_CFG_Type ssp_config;
    SSP_ConfigStructInit(&ssp_config);
    SSP_Init(LPC_SSP1, &ssp_config);
    SSP_Cmd(LPC_SSP0, DISABLE);
    SSP_Cmd(LPC_SSP1, ENABLE);
    SSP_LoopBackCmd(LPC_SSP1, DISABLE);
}


void init_dma(void)
{
    // ADC channel 0
    GPDMA_Channel_CFG_Type dma_ch0_cfg;
    dma_ch0_cfg.ChannelNum = 0;
    dma_ch0_cfg.TransferSize = adc_ph_samples;
    dma_ch0_cfg.TransferWidth = 0;
    dma_ch0_cfg.SrcMemAddr = 0;
    dma_ch0_cfg.DstMemAddr = (uint32_t) adc_ph_data;
    dma_ch0_cfg.TransferType = GPDMA_TRANSFERTYPE_P2M;
    dma_ch0_cfg.SrcConn = GPDMA_CONN_ADC;
    dma_ch0_cfg.DstConn = 0;

    adc_lli.SrcAddr = (uint32_t) &(LPC_ADC->ADGDR);
    adc_lli.DstAddr = (uint32_t) adc_ph_data;
    adc_lli.NextLLI = (uint32_t) &adc_lli;
    adc_lli.Control = adc_ph_samples
                    | (0x02 << 18)
                    | (0x02 << 21)
                    | (0x01 << 27);

    dma_ch0_cfg.DMALLI = (uint32_t) &adc_lli;

    GPDMA_Init();
    GPDMA_Setup(&dma_ch0_cfg);
    GPDMA_ChannelCmd(0, ENABLE);
}


void init_adc(void)
{
    ADC_Init(LPC_ADC, 200000);
    ADC_IntConfig(LPC_ADC, ADC_ADINTEN0, ENABLE);
    ADC_ChannelCmd(LPC_ADC, 0, ENABLE);

    ADC_EdgeStartConfig(LPC_ADC, ADC_START_ON_RISING);
    ADC_StartCmd(LPC_ADC, ADC_START_ON_MAT01);
}


void init_timers(void)
{
    // ADC timer
    TIM_TIMERCFG_Type timer0_cfg;
    timer0_cfg.PrescaleOption = TIM_PRESCALE_TICKVAL;
    timer0_cfg.PrescaleValue = 250;
    TIM_Init(LPC_TIM0, TIM_TIMER_MODE, &timer0_cfg);

    // ldc5110 refresh timer
    TIM_TIMERCFG_Type timer1_cfg;
    timer1_cfg.PrescaleOption = TIM_PRESCALE_TICKVAL;
    timer1_cfg.PrescaleValue = 2500;
    TIM_Init(LPC_TIM1, TIM_TIMER_MODE, &timer1_cfg);
}


void init_systick(void)
{
    SYSTICK_InternalInit(35);
    SYSTICK_Cmd(DISABLE);
    SYSTICK_IntCmd(DISABLE);
}


void init_interrupts(void)
{
    NVIC_EnableIRQ(EINT3_IRQn);
}


void EINT3_IRQHandler(void)
{
    if (GPIO_GetIntStatus(2, 12, 1))
        GPIO_ClearInt(2, SEL);
    if (GPIO_GetIntStatus(2, 11, 1))
        GPIO_ClearInt(2, UP);
    if (GPIO_GetIntStatus(2, 10, 1))
	    GPIO_ClearInt(2, DOWN);

    SYSTICK_Cmd(ENABLE);
    SYSTICK_IntCmd(ENABLE);
    GPIO_IntCmd(2, 0, 1);
}


void SysTick_Handler(void)
{
    static uint8_t count = 0;

    // SEL
    if (!((GPIO_ReadValue(2) >> 12) & 0x01)) {
        if (count < 2) {
            count++;
            return;
        }
        else {
            switch (fsm.current_state) {
            case ST_PH:
                event = EV_SELECT_PUSHED;
                break;
            case ST_MV:
                event = EV_SELECT_PUSHED;
                break;
            case ST_MAIN_MENU:
                if (select == 0)
                    event = EV_PH_SELECTED;
                else if (select == 1)
                    event = EV_MV_SELECTED;
                else if (select == 2)
                    event = EV_CAL_SELECTED;
                else
                    event = EV_NONE;
                break;
            case ST_CAL_MENU:
                if (select == 0)
                    event = EV_ONE_P_SELECTED;
                else if (select == 1)
                    event = EV_TWO_P_SELECTED;
                else if (select == 2)
                    event = EV_THREE_P_SELECTED;
                else if (select == 3)
                    event = EV_BACK_SELECTED;
                else
                    event = EV_NONE;
                break;
            case ST_1P_CAL_START:
                event = EV_START_CAL;
                break;
            case ST_2P_CAL_START:
                event = EV_START_CAL;
                break;
            case ST_3P_CAL_START:
                event = EV_START_CAL;
                break;
            default:
                event = EV_NONE;
                break;
            }
            count = 0;
            SYSTICK_Cmd(DISABLE);
            SYSTICK_IntCmd(DISABLE);
            GPIO_IntCmd(2, SEL | UP | DOWN, 1);
            return;
        }
    }

    // UP
    if (!((GPIO_ReadValue(2) >> 11) & 0x01)) {
        if (count < 2) {
            count++;
            return;
        }
        else {
            switch (fsm.current_state) {
            case ST_MAIN_MENU:
                select--;
                if (select == 0xFF)
                    select = 2;
                break;
            case ST_CAL_MENU:
                select--;
                if (select == 0xFF)
                    select = 3;
                break;
            case ST_1P_CAL_START:
                event = EV_ABORT_CAL;
                break;
            case ST_1P_CAL:
                event = EV_ABORT_CAL;
                break;
            case ST_2P_CAL_START:
                event = EV_ABORT_CAL;
                break;
            case ST_2P_CAL:
                event = EV_ABORT_CAL;
                break;
            case ST_3P_CAL_START:
                event = EV_ABORT_CAL;
                break;
            case ST_3P_CAL:
                event = EV_ABORT_CAL;
                break;
            default:
                event = EV_NONE;
                break;
            }
            count = 0;
            SYSTICK_Cmd(DISABLE);
            SYSTICK_IntCmd(DISABLE);
            GPIO_IntCmd(2, SEL | UP | DOWN, 1);
            return;
        }
    }

    // DOWN
    if (!((GPIO_ReadValue(2) >> 10) & 0x01)) {
        if (count < 2) {
            count++;
            return;
        }
        else {
            switch (fsm.current_state) {
            case ST_MAIN_MENU:
                select++;
                if (select > 2)
                    select = 0;
                break;
            case ST_CAL_MENU:
                select++;
                if (select > 3)
                    select = 0;
                break;
            case ST_1P_CAL_START:
                event = EV_ABORT_CAL;
                break;
            case ST_1P_CAL:
                event = EV_ABORT_CAL;
                break;
            case ST_2P_CAL_START:
                event = EV_ABORT_CAL;
                break;
            case ST_2P_CAL:
                event = EV_ABORT_CAL;
                break;
            case ST_3P_CAL_START:
                event = EV_ABORT_CAL;
                break;
            case ST_3P_CAL:
                event = EV_ABORT_CAL;
                break;
            default:
                event = EV_NONE;
                break;
            }
            count = 0;
            SYSTICK_Cmd(DISABLE);
            SYSTICK_IntCmd(DISABLE);
            GPIO_IntCmd(2, SEL | UP | DOWN, 1);
            return;
        }
    }
}

