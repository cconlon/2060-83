/*
*********************************************************************************************************
*                                              uC/CSP
*                                        Chip Support Package
*
*                            (c) Copyright 2010; Micrium, Inc.; Weston, FL
*                         (c) Copyright 2003-2010; Micrium, Inc.; Weston, FL
*
*               All rights reserved. Protected by international copyright laws.
*
*               uC/CSP is provided in source form to registered licensees ONLY.  It is
*               illegal to distribute this source code to any third party unless you receive
*               written permission by an authorized Micrium representative.  Knowledge of
*               the source code may NOT be used to develop a similar product.
*
*               Please help us continue to provide the Embedded community with the finest
*               software available.  Your honesty is greatly appreciated.
*
*               You can contact us at www.micrium.com.
*********************************************************************************************************
*/

/*
*********************************************************************************************************
*
*                                    GENERAL PURPOSE I/O CONTROLLER
*                                           ATMEL  AT91SAM9G20
* Filename      : csp_gio.c
* Version       : V1.00
* Programmer(s) : FT
*********************************************************************************************************
* Note(s)       : 
*********************************************************************************************************
*/


/*
*********************************************************************************************************
*                                            INCLUDE FILES
*********************************************************************************************************
*/

#define    CSP_GPIO_MODULE
#include   <csp.h>

/*
*********************************************************************************************************
*                                            LOCAL DEFINES
*********************************************************************************************************
*/


#define  CSP_GPIO_ADDR_PIOA                     (CPU_ADDR  )(0xFFFFF400)

#define  CSP_GPIO_REG_PIO_PER(port_nbr)         (*(CPU_REG32 *)(CSP_GPIO_ADDR_PIOA  + ((port_nbr) * 512u) + 0x00))
#define  CSP_GPIO_REG_PIO_PDR(port_nbr)         (*(CPU_REG32 *)(CSP_GPIO_ADDR_PIOA  + ((port_nbr) * 512u) + 0x04))
#define  CSP_GPIO_REG_PIO_PSR(port_nbr)         (*(CPU_REG32 *)(CSP_GPIO_ADDR_PIOA  + ((port_nbr) * 512u) + 0x08))

#define  CSP_GPIO_REG_PIO_OER(port_nbr)         (*(CPU_REG32 *)(CSP_GPIO_ADDR_PIOA  + ((port_nbr) * 512u) + 0x10))
#define  CSP_GPIO_REG_PIO_ODR(port_nbr)         (*(CPU_REG32 *)(CSP_GPIO_ADDR_PIOA  + ((port_nbr) * 512u) + 0x14))
#define  CSP_GPIO_REG_PIO_OSR(port_nbr)         (*(CPU_REG32 *)(CSP_GPIO_ADDR_PIOA  + ((port_nbr) * 512u) + 0x10))

#define  CSP_GPIO_REG_PIO_IFER(port_nbr)        (*(CPU_REG32 *)(CSP_GPIO_ADDR_PIOA  + ((port_nbr) * 512u) + 0x20))
#define  CSP_GPIO_REG_PIO_IFDR(port_nbr)        (*(CPU_REG32 *)(CSP_GPIO_ADDR_PIOA  + ((port_nbr) * 512u) + 0x24))
#define  CSP_GPIO_REG_PIO_IFSR(port_nbr)        (*(CPU_REG32 *)(CSP_GPIO_ADDR_PIOA  + ((port_nbr) * 512u) + 0x28))

#define  CSP_GPIO_REG_PIO_SODR(port_nbr)        (*(CPU_REG32 *)(CSP_GPIO_ADDR_PIOA  + ((port_nbr) * 512u) + 0x30))
#define  CSP_GPIO_REG_PIO_CODR(port_nbr)        (*(CPU_REG32 *)(CSP_GPIO_ADDR_PIOA  + ((port_nbr) * 512u) + 0x34))
#define  CSP_GPIO_REG_PIO_ODSR(port_nbr)        (*(CPU_REG32 *)(CSP_GPIO_ADDR_PIOA  + ((port_nbr) * 512u) + 0x38))
#define  CSP_GPIO_REG_PIO_PDSR(port_nbr)        (*(CPU_REG32 *)(CSP_GPIO_ADDR_PIOA  + ((port_nbr) * 512u) + 0x3C))

#define  CSP_GPIO_REG_PIO_IER(port_nbr)         (*(CPU_REG32 *)(CSP_GPIO_ADDR_PIOA  + ((port_nbr) * 512u) + 0x40))
#define  CSP_GPIO_REG_PIO_IDR(port_nbr)         (*(CPU_REG32 *)(CSP_GPIO_ADDR_PIOA  + ((port_nbr) * 512u) + 0x44))
#define  CSP_GPIO_REG_PIO_IMR(port_nbr)         (*(CPU_REG32 *)(CSP_GPIO_ADDR_PIOA  + ((port_nbr) * 512u) + 0x48))
#define  CSP_GPIO_REG_PIO_ISR(port_nbr)         (*(CPU_REG32 *)(CSP_GPIO_ADDR_PIOA  + ((port_nbr) * 512u) + 0x4C))

#define  CSP_GPIO_REG_PIO_MDER(port_nbr)        (*(CPU_REG32 *)(CSP_GPIO_ADDR_PIOA  + ((port_nbr) * 512u) + 0x50))
#define  CSP_GPIO_REG_PIO_MDDR(port_nbr)        (*(CPU_REG32 *)(CSP_GPIO_ADDR_PIOA  + ((port_nbr) * 512u) + 0x54))
#define  CSP_GPIO_REG_PIO_MDSR(port_nbr)        (*(CPU_REG32 *)(CSP_GPIO_ADDR_PIOA  + ((port_nbr) * 512u) + 0x58))

#define  CSP_GPIO_REG_PIO_PUDR(port_nbr)        (*(CPU_REG32 *)(CSP_GPIO_ADDR_PIOA  + ((port_nbr) * 512u) + 0x60))
#define  CSP_GPIO_REG_PIO_PUER(port_nbr)        (*(CPU_REG32 *)(CSP_GPIO_ADDR_PIOA  + ((port_nbr) * 512u) + 0x64))
#define  CSP_GPIO_REG_PIO_PUSR(port_nbr)        (*(CPU_REG32 *)(CSP_GPIO_ADDR_PIOA  + ((port_nbr) * 512u) + 0x68))

#define  CSP_GPIO_REG_PIO_ASR(port_nbr)         (*(CPU_REG32 *)(CSP_GPIO_ADDR_PIOA  + ((port_nbr) * 512u) + 0x70))
#define  CSP_GPIO_REG_PIO_BSR(port_nbr)         (*(CPU_REG32 *)(CSP_GPIO_ADDR_PIOA  + ((port_nbr) * 512u) + 0x74))
#define  CSP_GPIO_REG_PIO_ABSR(port_nbr)        (*(CPU_REG32 *)(CSP_GPIO_ADDR_PIOA  + ((port_nbr) * 512u) + 0x78))

#define  CSP_GPIO_REG_PIO_OWER(port_nbr)        (*(CPU_REG32 *)(CSP_GPIO_ADDR_PIOA  + ((port_nbr) * 512u) + 0xA0))
#define  CSP_GPIO_REG_PIO_OWDR(port_nbr)        (*(CPU_REG32 *)(CSP_GPIO_ADDR_PIOA  + ((port_nbr) * 512u) + 0xA4))
#define  CSP_GPIO_REG_PIO_OWSR(port_nbr)        (*(CPU_REG32 *)(CSP_GPIO_ADDR_PIOA  + ((port_nbr) * 512u) + 0xA8))

#define  CSP_GPIO_REG_PIO_DELAY0(port_nbr)      (*(CPU_REG32 *)(CSP_GPIO_ADDR_PIOA  + ((port_nbr) * 512u) + 0xC0))
#define  CSP_GPIO_REG_PIO_DELAY1(port_nbr)      (*(CPU_REG32 *)(CSP_GPIO_ADDR_PIOA  + ((port_nbr) * 512u) + 0xC4))
#define  CSP_GPIO_REG_PIO_DELAY2(port_nbr)      (*(CPU_REG32 *)(CSP_GPIO_ADDR_PIOA  + ((port_nbr) * 512u) + 0xC8))
#define  CSP_GPIO_REG_PIO_DELAY3(port_nbr)      (*(CPU_REG32 *)(CSP_GPIO_ADDR_PIOA  + ((port_nbr) * 512u) + 0xCC))

#define  CSP_GPIO_REG_PIO_WPMR(port_nbr)        (*(CPU_REG32 *)(CSP_GPIO_ADDR_PIOA  + ((port_nbr) * 512u) + 0xE4))
#define  CSP_GPIO_REG_PIO_WPSR(port_nbr)        (*(CPU_REG32 *)(CSP_GPIO_ADDR_PIOA  + ((port_nbr) * 512u) + 0xE8))


/*
*********************************************************************************************************
*                                           LOCAL CONSTANTS
*********************************************************************************************************
*/


/*
*********************************************************************************************************
*                                          LOCAL DATA TYPES
*********************************************************************************************************
*/


/*
*********************************************************************************************************
*                                            LOCAL TABLES
*********************************************************************************************************
*/


/*
*********************************************************************************************************
*                                       LOCAL GLOBAL VARIABLES
*********************************************************************************************************
*/


/*
*********************************************************************************************************
*                                      LOCAL FUNCTION PROTOTYPES
*********************************************************************************************************
*/


/*
*********************************************************************************************************
*                                     LOCAL CONFIGURATION ERRORS
*********************************************************************************************************
*/



/*
*********************************************************************************************************
*********************************************************************************************************
*                                           LOCAL FUNCTIONS
*********************************************************************************************************
*********************************************************************************************************
*/

/*
*********************************************************************************************************
*                                          CSP_GPIO_Init()
*
* Description : Initialize general purpose I/O module.
*
* Argument(s) : none.
*
* Return(s)   : None.
*
* Caller(s)   : Application.
*
* Note(s)     : None.
*********************************************************************************************************
*/

void  CSP_GPIO_Init (void) 
{

}    


/*
*********************************************************************************************************
*                                          CSP_GPIO_Cfg()
*
* Description : Configure a set of pins for a specific I/O port
*
* Argument(s) : port_nbr    GPIO port number. (see note #1)
*
*               pins        Bit mask specifying which pins to configure.
*
*               dir         Pins direction :
*                               CSP_GPIO_DIR_IN                  Pins are configured as inputs.
*                               CSP_GPIO_DIR_OUT                 Pins are configured as outputs.
*                               CSP_GPIO_DIR_INOUT               Pins are configured as bidirectional pins.  
*    
*               drv_mode    Pins Drive mode flags:
*                               CSP_GPIO_FLAG_MODE_NONE          Pins have neither pull-down nor pull-up.
*                               CSP_GPIO_FLAG_MODE_PULLUP        Pins have a pull-up   resistor enabled.
*                               CSP_GPIO_FLAG_MODE_PULLDOWN      Pins have a pull-down resistor enabled.
*                               CSP_GPIO_FLAG_MODE_REPEATER      Pins have repeater mode enabled.
*                               CSP_GPIO_FLAG_MODE_OPEN_DRAIN    Pins have open drain mode enabled.
*
*               int_en      Pin Interrupt enable (not supported.)
*                               DEF_YES                          Pin interrupt is enabled.
*                               DEF_NO                           Pin Interrupt is disabled.
*              
*               int_pol     Pin interrupt polarity:
*                               CSP_INT_POL_LEVEL_HIGH           Pin interrupt is activated on a high level (if int_act = 'CSP_INT_ACT_LEVEL').
*                               CSP_INT_POL_LEVEL_LOW            Pin interrupt is activated on a low level  (if int_act = 'CSP_INT_ACT_LEVEL').
* 
*                               CSP_INT_POL_EDGE_POSITIVE        Pin interrupt is activated on the positive edge. (if int_act = 'CSP_INT_ACT_EDGE').
*                               CSP_INT_POL_EDGE_NEGATIVE        Pin interrupt is activated on the negative edge. (if int_act = 'CSP_INT_ACT_EDGE').
*                               CSP_INT_POL_EDGE_BOTH            Pin interrupt is activated on both edges.        (if int_act = 'CSP_INT_ACT_EDGE').
*
*               fnct        Pins function (see note #2).
*                               CSP_GPIO_FNCT_00                 Pins are controlled by the GPIO Controller.
*                               CSP_GPIO_FNCT_01                 Pins are controlled by peripheral A function.
*                               CSP_GPIO_FNCT_02                 Pins are controlled by peripheral B function.
*
* Return(s)   : DEF_OK    If the pins were configured successfully.
*               DEF_FAIL  If the pins could not be configured successfully.
*
* Caller(s)   : Application.
*
* Note(s)     : (1) The 'port_nbr' argument depends on the number of GPIO ports available for this 
*                   family definition.
*
*                   (a) Port numbers identifiers can be found in 'csp.h':
*
*                           CSP_GPIO_PORT_NBR_00
*                           CSP_GPIO_PORT_NBR_01
*                           CSP_GPIO_PORT_NBR_B
*                           CSP_GPIO_PORT_NBR_B
*
*                (2) Some architectures multiplex general purpose pins with peripheral functions the 
*                    'fnct' parameter defines the GPIO function. 
*
*                   (a) GPIO function identifier can be found in csp.h:
*
*                           CSP_GPIO_FNCT_00
*                           CSP_GPIO_FNCT_01
*                           CSP_GPIO_FNCT_02
*                           CSP_GPIO_FNCT_03
*
*                   (b) In the Atmel AT91SAMxxx devices pins can be configured with the following funtions:
*
*                               CSP_GPIO_FNCT_00    Pins are controlled by the GPIO Controller.
*                               CSP_GPIO_FNCT_01    Pins are controlled by peripheral A function.
*                               CSP_GPIO_FNCT_02    Pins are controlled by peripheral B function.
*********************************************************************************************************
*/

CPU_BOOLEAN  CSP_GPIO_Cfg (CSP_DEV_NBR    port_nbr,
                           CSP_GPIO_MSK   pins,
                           CSP_OPT        dir,                                    
                           CSP_OPT_FLAGS  drv_mode,
                           CPU_BOOLEAN    int_en,
                           CSP_OPT        int_pol,
                           CSP_OPT        fnct)                           
{   
                                                                /* -------------- ARGUMENTS CHECKING ---------------- */
#if (CSP_CFG_ARG_CHK_EN == DEF_ENABLED)
    if (port_nbr > CSP_GPIO_PORT_NBR_B) {
        return (DEF_FAIL);
    }
#endif
                                                                /* ----------- PIN DIRECTION DECODIFICATION ----------- */
    switch (fnct) {
        case CSP_GPIO_FNCT_00:
             CSP_GPIO_REG_PIO_PER(port_nbr) = pins;
             if (dir == CSP_GPIO_DIR_IN) {
                 CSP_GPIO_REG_PIO_ODR(port_nbr) = pins;
             } else {
                 CSP_GPIO_REG_PIO_OER(port_nbr) = pins;
             }
             break;
        
        case CSP_GPIO_FNCT_01:
             CSP_GPIO_REG_PIO_PDR(port_nbr) = pins;
             CSP_GPIO_REG_PIO_ASR(port_nbr) = pins;             
             break;
        
        case CSP_GPIO_FNCT_02:
             CSP_GPIO_REG_PIO_PDR(port_nbr) = pins;
             CSP_GPIO_REG_PIO_BSR(port_nbr) = pins;             
             break;
             
        default:
             return (DEF_FAIL);
    }
    
    if (DEF_BIT_IS_SET(drv_mode, CSP_GPIO_FLAG_MODE_OPEN_DRAIN)) {
        CSP_GPIO_REG_PIO_MDER(port_nbr) = pins;
    } else {
        CSP_GPIO_REG_PIO_MDDR(port_nbr) = pins;
    }    

    if (DEF_BIT_IS_SET(drv_mode, CSP_GPIO_FLAG_MODE_PULLUP)) {
        CSP_GPIO_REG_PIO_PUER(port_nbr) = pins;
    } else {
        CSP_GPIO_REG_PIO_PUDR(port_nbr) = pins;
    }

    if (int_en == DEF_ENABLED) {
        CSP_GPIO_REG_PIO_IER(port_nbr) = pins;
    } else {
        CSP_GPIO_REG_PIO_IDR(port_nbr) = pins;
    }
    
    return (DEF_OK);
}


/*
*********************************************************************************************************
*                                         CSP_GPIO_BitClr()
*
* Description : Clear a group of pins for a specific I/O port.
*
* Argument(s) : port_nbr    GPIO port number. (See 'CSP_GPIO_Cfg()' note #1).
*
*               pins        Bit mask specifying which pins to clear.
*
* Return(s)   : none
*
* Caller(s)   : Application.
*
* Note(s)     : none.                   
*********************************************************************************************************
*/

void  CSP_GPIO_BitClr (CSP_DEV_NBR   port_nbr,
                       CSP_GPIO_MSK  pins)
{
#if (CSP_CFG_ARG_CHK_EN == DEF_ENABLED)     
    if (port_nbr > CSP_GPIO_PORT_NBR_B) {
        return;
    }
#endif

    CSP_GPIO_REG_PIO_CODR(port_nbr) = pins;
}


/*
*********************************************************************************************************
*                                         CSP_GPIO_BitSet()
*
* Description : Set a group of pins for a specific I/O port
*
* Argument(s) : port_nbr  GPIO port number. (See 'CSP_GPIO_Cfg()' note #1).
*
*               pins      Bit mask specifying which pins to set.
*
* Return(s)   : none
*
* Caller(s)   : Application.
*
* Note(s)     : none.                   
*********************************************************************************************************
*/

void  CSP_GPIO_BitSet (CSP_DEV_NBR    port_nbr,
                       CSP_GPIO_MSK  pins)
{
#if (CSP_CFG_ARG_CHK_EN == DEF_ENABLED)         
    if (port_nbr > CSP_GPIO_PORT_NBR_B) {
        return;
    }
#endif
    
    CSP_GPIO_REG_PIO_SODR(port_nbr) = pins;
}


/*
*********************************************************************************************************
*                                         CSP_GPIO_BitToggle()
*
* Description : Toggle a group of pins for a specific I/O port.
*
* Argument(s) : port_nbr  GPIO port number. (See 'CSP_GPIO_Cfg()' note #1).
*
*               pins      Bit mask specifying which pins to Toggle.
*
* Return(s)   : none
*
* Caller(s)   : Application.
*
* Note(s)     : none.                   
*********************************************************************************************************
*/

void  CSP_GPIO_BitToggle (CSP_DEV_NBR    port_nbr,
                          CSP_GPIO_MSK  pins)                          
{
    CSP_GPIO_MSK  pins_set;
    CSP_GPIO_MSK  pins_clr;
    
#if (CSP_CFG_ARG_CHK_EN == DEF_ENABLED)    
    if (port_nbr > CSP_GPIO_PORT_NBR_B) {
        return;
    }
#endif
    
    pins_clr =  (CSP_GPIO_REG_PIO_ODSR(port_nbr)) & pins;    
    pins_set = ~(CSP_GPIO_REG_PIO_ODSR(port_nbr)) & pins;
    
    CSP_GPIO_REG_PIO_CODR(port_nbr) = pins_clr;
    CSP_GPIO_REG_PIO_SODR(port_nbr) = pins_set;    
}


/*
*********************************************************************************************************
*                                         CSP_GPIO_Rd()
*
* Description : Read the current value of the port.
*
* Argument(s) : port_nbr    GPIO port number. (See 'CSP_GPIO_Cfg()' note #1).
*
* Return(s)   : The current value of the port.
*
* Caller(s)   : Application.
*
* Note(s)     : none.                   
*********************************************************************************************************
*/

CSP_GPIO_MSK  CSP_GPIO_Rd (CSP_DEV_NBR  port_nbr)
{
    CSP_GPIO_MSK  port_val;

#if (CSP_CFG_ARG_CHK_EN == DEF_ENABLED)     
    if (port_nbr > CSP_GPIO_PORT_NBR_B) {
        return (CSP_GPIO_MSK)0;
    }
#endif

    port_val = CSP_GPIO_REG_PIO_ODSR(port_nbr);
    
    return (port_val);
}


/*
*********************************************************************************************************
*                                         CSP_GPIO_Wr()
*
* Description : Write a value to a specific port.
*
* Argument(s) : port_nbr    GPIO port number. (See 'CSP_GPIO_Cfg()' note #1).
*
*               val         Value to be written in the port.
*
* Return(s)   : The current value of the port.
*
* Caller(s)   : Application.
*
* Note(s)     : None.
*********************************************************************************************************
*/

void  CSP_GPIO_Wr (CSP_DEV_NBR   port_nbr,
                   CSP_GPIO_MSK  val)
{
    CPU_INT32U  pins_clr;
    CPU_INT32U  pins_set;
    
#if (CSP_CFG_ARG_CHK_EN == DEF_ENABLED)     
    if (port_nbr > CSP_GPIO_PORT_NBR_B) {
        return;
    }
#endif
    
    pins_set =  val;
    pins_clr = ~val;
    CSP_GPIO_REG_PIO_CODR(port_nbr) = pins_clr;
    CSP_GPIO_REG_PIO_SODR(port_nbr) = pins_set;    
}


/*
*********************************************************************************************************
*                                         CSP_GPIO_IntClr()
*
* Description : Clear pending interrupts for a specific port.
*
* Argument(s) : port_nbr    GPIO port number. (See 'CSP_GPIO_Cfg()' note #1).
*
* Return(s)   : Interrupt status.
*
* Caller(s)   : Application.
*
* Note(s)     : None.
*********************************************************************************************************
*/

CSP_GPIO_MSK CSP_GPIO_IntClr (CSP_DEV_NBR  port_nbr)
{
    CSP_GPIO_MSK  int_stat;
    
#if (CSP_CFG_ARG_CHK_EN == DEF_ENABLED)     
    if (port_nbr > CSP_GPIO_PORT_NBR_B) {
        return (CSP_GPIO_MSK)(0u);
    }
#endif
    
    int_stat = CSP_GPIO_REG_PIO_ISR(port_nbr);
    
    return (int_stat);
}