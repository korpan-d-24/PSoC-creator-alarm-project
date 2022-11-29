
/*******************************************************************************
* File Name: cyfitter_cfg.c
* 
* PSoC Creator  4.4
*
* Description:
* This file contains device initialization code.
* Except for the user defined sections in CyClockStartupError(), this file should not be modified.
* This file is automatically generated by PSoC Creator.
*
********************************************************************************
* Copyright (c) 2007-2020 Cypress Semiconductor.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions, 
* disclaimers, and limitations in the end user license agreement accompanying 
* the software package with which this file was provided.
********************************************************************************/

#include <string.h>
#include "cytypes.h"
#include "cydevice_trm.h"
#include "cyfitter.h"
#include "CyLib.h"
#include "cyfitter_cfg.h"
#include "cyapicallbacks.h"

#define CY_NEED_CYCLOCKSTARTUPERROR 1


#if defined(__C51__) || defined(__CX51__)
	#define CYPACKED
	#define CYPACKED_ATTR
	#define CYALIGNED
	
    #define CY_CFG_UNUSED
	#define CY_CFG_MEMORY_BARRIER() do { } while (0)



	#define CYMEMZERO(a,c) cymemzero((a),(c))
	#define CYCONFIGCPY(d,s,c) cyconfigcpy((c),(s),(d))
	#define CYCONFIGCPYCODE(d,s,c) cyconfigcpycode((c),(s),(d))
#else
	#error Unsupported toolchain
#endif




/* Clock startup error codes                                                   */
#define CYCLOCKSTART_NO_ERROR    0u
#define CYCLOCKSTART_XTAL_ERROR  1u
#define CYCLOCKSTART_32KHZ_ERROR 2u
#define CYCLOCKSTART_PLL_ERROR   3u
#define CYCLOCKSTART_FLL_ERROR   4u
#define CYCLOCKSTART_WCO_ERROR   5u


#ifdef CY_NEED_CYCLOCKSTARTUPERROR
/*******************************************************************************
* Function Name: CyClockStartupError
********************************************************************************
* Summary:
*  If an error is encountered during clock configuration (crystal startup error,
*  PLL lock error, etc.), the system will end up here.  Unless reimplemented by
*  the customer, this function will stop in an infinite loop.
*
* Parameters:
*   void
*
* Return:
*   void
*
*******************************************************************************/
CY_CFG_UNUSED
static void CyClockStartupError(uint8 errorCode);
CY_CFG_UNUSED
static void CyClockStartupError(uint8 errorCode)
{
    /* To remove the compiler warning if errorCode not used.                */
    errorCode = errorCode;

    /* If we have a clock startup error (bad MHz crystal, PLL lock, etc.),  */
    /* we will end up here to allow the customer to implement something to  */
    /* deal with the clock condition.                                       */

#ifdef CY_CFG_CLOCK_STARTUP_ERROR_CALLBACK
    CY_CFG_Clock_Startup_ErrorCallback();
#else
    /*  If not using CY_CFG_CLOCK_STARTUP_ERROR_CALLBACK, place your clock startup code here. */
    /* `#START CyClockStartupError` */



    /* `#END` */

    while(1) {}
#endif /* CY_CFG_CLOCK_STARTUP_ERROR_CALLBACK */
}
#endif

#define cfg_byte_table ((const void CYFAR *)0x080000u)
/* IOPINS0_1 Address: CYREG_PRT1_DM0 Size (bytes): 8 */
#define BS_IOPINS0_1_VAL ((const uint8 CYFAR *)0x000802E4u)

/* IOPINS0_2 Address: CYREG_PRT2_DM0 Size (bytes): 8 */
#define BS_IOPINS0_2_VAL ((const uint8 CYFAR *)0x000802ECu)

/* IOPINS0_5 Address: CYREG_PRT5_DM0 Size (bytes): 8 */
#define BS_IOPINS0_5_VAL ((const uint8 CYFAR *)0x000802F4u)

/* IOPINS0_6 Address: CYREG_PRT6_DM0 Size (bytes): 8 */
#define BS_IOPINS0_6_VAL ((const uint8 CYFAR *)0x000802FCu)


/*******************************************************************************
* Function Name: ClockSetup
********************************************************************************
*
* Summary:
*  Performs the initialization of all of the clocks in the device based on the
*  settings in the Clock tab of the DWR.  This includes enabling the requested
*  clocks and setting the necessary dividers to produce the desired frequency. 
*
* Parameters:
*  void
*
* Return:
*  void
*
*******************************************************************************/
static void ClockSetup(void);
static void ClockSetup(void)
{
	uint32 timeout;
	uint8 pllLock;


	/* Configure Digital Clocks based on settings from Clock DWR */
	CY_SET_XTND_REG16((void CYFAR *)(CYREG_CLKDIST_DCFG0_CFG0), 0x0001u);
	CY_SET_REG8((void CYXDATA *)(CYREG_CLKDIST_DCFG0_CFG0 + 0x2u), 0x18u);

	/* Configure ILO based on settings from Clock DWR */
	CY_SET_REG8((void CYXDATA *)(CYREG_SLOWCLK_ILO_CR0), 0x02u);
	CY_SET_REG8((void CYXDATA *)(CYREG_CLKDIST_CR), 0x08u);

	/* Configure IMO based on settings from Clock DWR */
	CY_SET_REG8((void CYXDATA *)(CYREG_FASTCLK_IMO_CR), 0x03u);
	CY_SET_REG8((void CYXDATA *)(CYREG_IMO_TR1), (CY_GET_XTND_REG8((void CYFAR *)CYREG_FLSHID_CUST_TABLES_IMO_3MHZ)));

	/* Configure PLL based on settings from Clock DWR */
	CY_SET_XTND_REG16((void CYFAR *)(CYREG_FASTCLK_PLL_P), 0x0008u);
	CY_SET_XTND_REG16((void CYFAR *)(CYREG_FASTCLK_PLL_CFG0), 0x1251u);
	/* Wait up to 250us for the PLL to lock */
	pllLock = 0u;
	for (timeout = 250u / 10u; (timeout > 0u) && (pllLock != 0x03u); timeout--)
	{ 
		pllLock = 0x03u & ((uint8)((uint8)pllLock << 1) | ((CY_GET_REG8((void CYXDATA *)CYREG_FASTCLK_PLL_SR) & 0x01u) >> 0));
		CyDelayCycles(10u * 48u); /* Delay 10us based on 48MHz clock */
	}
	/* If we ran out of time the PLL didn't lock so go to the error function */
	if (timeout == 0u)
	{
		CyClockStartupError(CYCLOCKSTART_PLL_ERROR);
	}

	/* Configure Bus/Master Clock based on settings from Clock DWR */
	CY_SET_XTND_REG16((void CYFAR *)(CYREG_CLKDIST_MSTR0), 0x0100u);
	CY_SET_REG8((void CYXDATA *)(CYREG_CLKDIST_MSTR0), 0x07u);
	CY_SET_REG8((void CYXDATA *)(CYREG_CLKDIST_BCFG0), 0x00u);
	CY_SET_REG8((void CYXDATA *)(CYREG_CLKDIST_BCFG2), 0x48u);
	CY_SET_REG8((void CYXDATA *)(CYREG_CLKDIST_MSTR0), 0x00u);
	CY_SET_REG8((void CYXDATA *)(CYREG_CLKDIST_LD), 0x02u);

	CY_SET_REG8((void CYXDATA *)(CYREG_PM_ACT_CFG2), ((CY_GET_REG8((void CYXDATA *)CYREG_PM_ACT_CFG2) | 0x01u)));
}


/* Analog API Functions */


/*******************************************************************************
* Function Name: AnalogSetDefault
********************************************************************************
*
* Summary:
*  Sets up the analog portions of the chip to default values based on chip
*  configuration options from the project.
*
* Parameters:
*  void
*
* Return:
*  void
*
*******************************************************************************/
static void AnalogSetDefault(void);
static void AnalogSetDefault(void)
{
	uint8 bg_xover_inl_trim = CY_GET_XTND_REG8((void CYFAR *)(CYREG_FLSHID_MFG_CFG_BG_XOVER_INL_TRIM + 1u));
	CY_SET_REG8((void CYXDATA *)(CYREG_BG_DFT0), (bg_xover_inl_trim & 0x07u));
	CY_SET_REG8((void CYXDATA *)(CYREG_BG_DFT1), ((bg_xover_inl_trim >> 4) & 0x0Fu));
	CY_SET_REG8((void CYXDATA *)CYREG_PRT6_AG, 0x10u);
	CY_SET_REG8((void CYXDATA *)CYREG_CAPSL_CFG0, 0x00u);
	CY_SET_REG8((void CYXDATA *)CYREG_CMP0_SW3, 0x20u);
	CY_SET_REG8((void CYXDATA *)CYREG_BUS_SW3, 0x01u);
	CY_SET_REG8((void CYXDATA *)CYREG_PUMP_CR0, 0x44u);
}


/*******************************************************************************
* Function Name: SetAnalogRoutingPumps
********************************************************************************
*
* Summary:
* Enables or disables the analog pumps feeding analog routing switches.
* Intended to be called at startup, based on the Vdda system configuration;
* may be called during operation when the user informs us that the Vdda voltage
* crossed the pump threshold.
*
* Parameters:
*  enabled - 1 to enable the pumps, 0 to disable the pumps
*
* Return:
*  void
*
*******************************************************************************/
void SetAnalogRoutingPumps(uint8 enabled)
{
	uint8 regValue = CY_GET_REG8((void CYXDATA *)CYREG_PUMP_CR0);
	if (enabled != 0u)
	{
		regValue |= 0x22u;
	}
	else
	{
		regValue &= (uint8)~0x22u;
	}
	CY_SET_REG8((void CYXDATA *)CYREG_PUMP_CR0, regValue);
}


#define CY_AMUX_UNUSED CYREG_BOOST_SR
/* This is an implementation detail of the AMux. Code that depends on it may be
   incompatible with other versions of PSoC Creator. */
uint8 CYXDATA * const CYCODE CapSense_AMuxCH0__addrTable[10] = {
	(uint8 CYXDATA *)CYREG_PRT5_AMUX, 
	(uint8 CYXDATA *)CYREG_PRT5_AMUX, 
	(uint8 CYXDATA *)CYREG_PRT5_AMUX, 
	(uint8 CYXDATA *)CYREG_PRT5_AMUX, 
	(uint8 CYXDATA *)CYREG_PRT5_AMUX, 
	(uint8 CYXDATA *)CYREG_PRT5_AMUX, 
	(uint8 CYXDATA *)CYREG_PRT5_AMUX, 
	(uint8 CYXDATA *)CYREG_PRT6_AMUX, 
	(uint8 CYXDATA *)CYREG_CMP0_SW3, 
	(uint8 CYXDATA *)CYREG_DAC1_SW3, 
};

/* This is an implementation detail of the AMux. Code that depends on it may be
   incompatible with other versions of PSoC Creator. */
const uint8 CYCODE CapSense_AMuxCH0__maskTable[10] = {
	0x01u, 
	0x02u, 
	0x04u, 
	0x08u, 
	0x10u, 
	0x20u, 
	0x40u, 
	0x10u, 
	0x01u, 
	0x10u, 
};

/*******************************************************************************
* Function Name: CapSense_AMuxCH0_Set
********************************************************************************
* Summary:
*  This function is used to set a particular channel as active on the AMux.
*
* Parameters:  
*   channel - The mux channel input to set as active
*
* Return:
*   void
*
*******************************************************************************/
void CapSense_AMuxCH0_Set(uint8 channel)
{
	if (channel < 10)
	{
		*CapSense_AMuxCH0__addrTable[channel] |= CapSense_AMuxCH0__maskTable[channel];
	}
}

/*******************************************************************************
* Function Name: CapSense_AMuxCH0_Unset
********************************************************************************
* Summary:
*  This function is used to clear a particular channel from being active on the
*  AMux.
*
* Parameters:  
*   channel - The mux channel input to mark inactive
*
* Return:
*   void
*
*******************************************************************************/
void CapSense_AMuxCH0_Unset(uint8 channel)
{
	if (channel < 10)
	{
		*CapSense_AMuxCH0__addrTable[channel] &= (uint8)~CapSense_AMuxCH0__maskTable[channel];
	}
}



/*******************************************************************************
* Function Name: cyfitter_cfg
********************************************************************************
* Summary:
*  This function is called by the start-up code for the selected device. It
*  performs all of the necessary device configuration based on the design
*  settings.  This includes settings from the Design Wide Resources (DWR) such
*  as Clocks and Pins as well as any component configuration that is necessary.
*
* Parameters:
*   void
*
* Return:
*   void
*
*******************************************************************************/

void cyfitter_cfg(void)
{
	/* Enable/Disable Debug functionality based on settings from System DWR */
	CY_SET_XTND_REG8((void CYFAR *)CYREG_MLOGIC_DEBUG, (CY_GET_XTND_REG8((void CYFAR *)CYREG_MLOGIC_DEBUG) | 0x05u));

	{

		CYPACKED typedef struct {
			void CYFAR *address;
			uint16 size;
		} CYPACKED_ATTR cfg_memset_t;

		static const cfg_memset_t CYCODE cfg_memset_list[] = {
			/* address, size */
			{(void CYFAR *)(CYREG_PRT0_DR), 16u},
			{(void CYFAR *)(CYREG_PRT3_DR), 32u},
			{(void CYFAR *)(CYREG_PRT12_DR), 16u},
			{(void CYFAR *)(CYREG_PRT15_DR), 16u},
			{(void CYFAR *)(CYDEV_UCFG_B0_P0_U0_BASE), 4096u},
			{(void CYFAR *)(CYDEV_UCFG_B1_P2_U0_BASE), 2048u},
			{(void CYFAR *)(CYDEV_UCFG_DSI0_BASE), 2560u},
			{(void CYFAR *)(CYDEV_UCFG_DSI12_BASE), 512u},
			{(void CYFAR *)(CYREG_BCTL0_MDCLK_EN), 32u},
		};

		uint8 CYDATA i;

		/* Zero out critical memory blocks before beginning configuration */
		for (i = 0u; i < (sizeof(cfg_memset_list)/sizeof(cfg_memset_list[0])); i++)
		{
			const cfg_memset_t CYCODE * CYDATA ms = &cfg_memset_list[i];
			CYMEMZERO(ms->address, ms->size);
		}

		cfg_write_bytes(cfg_byte_table);

		/* Perform normal device configuration. Order is not critical for these items. */
		CY_SET_XTND_REG16((void CYFAR *)(CYREG_CAPSR_CFG0), 0x0300u);
		CY_SET_XTND_REG16((void CYFAR *)(CYREG_LUT0_CR), 0x0003u);

		/* B0_P5_U1_CFG24 Starting address: CYDEV_UCFG_B0_P5_U1_CFG24 */
		CY_SET_XTND_REG32((void CYFAR *)(CYREG_B0_P5_U1_CFG24), 0xC4040000u);

		/* Enable digital routing */
		CY_SET_XTND_REG8((void CYFAR *)CYREG_BCTL0_BANK_CTL, CY_GET_XTND_REG8((void CYFAR *)CYREG_BCTL0_BANK_CTL) | 0x02u);
		CY_SET_XTND_REG8((void CYFAR *)CYREG_BCTL1_BANK_CTL, CY_GET_XTND_REG8((void CYFAR *)CYREG_BCTL1_BANK_CTL) | 0x02u);

		/* Enable UDB array */
		CY_SET_REG8((void CYXDATA *)CYREG_PM_ACT_CFG0, CY_GET_REG8((void CYXDATA *)CYREG_PM_ACT_CFG0) | 0x40u);
		CY_SET_REG8((void CYXDATA *)CYREG_PM_AVAIL_CR2, CY_GET_REG8((void CYXDATA *)CYREG_PM_AVAIL_CR2) | 0x10u);
	}


	/* Perform second pass device configuration. These items must be configured in specific order after the regular configuration is done. */
	CYCONFIGCPY((void CYFAR *)(CYREG_PRT1_DM0), (const void CYFAR *)(BS_IOPINS0_1_VAL), 8u);
	CYCONFIGCPY((void CYFAR *)(CYREG_PRT2_DM0), (const void CYFAR *)(BS_IOPINS0_2_VAL), 8u);
	CYCONFIGCPY((void CYFAR *)(CYREG_PRT5_DM0), (const void CYFAR *)(BS_IOPINS0_5_VAL), 8u);
	CYCONFIGCPY((void CYFAR *)(CYREG_PRT6_DM0), (const void CYFAR *)(BS_IOPINS0_6_VAL), 8u);
	CY_SET_REG8((void CYXDATA *)(CYREG_INTC_CSR_EN), 0x01u);
	/* Switch Boost to the precision bandgap reference from its internal reference */
	CY_SET_REG8((void CYXDATA *)CYREG_BOOST_CR2, (CY_GET_REG8((void CYXDATA *)CYREG_BOOST_CR2) | 0x08u));
	if (CYDEV_CHIP_REV_ACTUAL < 5u)
	{
		CY_SET_REG8((void CYXDATA *)CYREG_PWRSYS_WAKE_TR2, 0x3Au);
	}


	/* Set Flash Cycles based on max possible frequency in case a glitch occurs during ClockSetup(). */
	CY_SET_REG8((void CYXDATA *)(CYREG_CACHE_CR), (((CYDEV_INSTRUCT_CACHE_ENABLED) != 0) ? 0x31u : 0x00u));
	/* Setup clocks based on selections from Clock DWR */
	ClockSetup();
	/* Set Flash Cycles based on newly configured 24.00MHz Bus Clock. */
	CY_SET_REG8((void CYXDATA *)(CYREG_CACHE_CR), (((CYDEV_INSTRUCT_CACHE_ENABLED) != 0) ? 0xB1u : 0x80u));

	/* Perform basic analog initialization to defaults */
	AnalogSetDefault();

	/* Configure alternate active mode */
	CYCONFIGCPY((void CYFAR *)CYDEV_PM_STBY_BASE, (const void CYFAR *)CYDEV_PM_ACT_BASE, 14u);
	CY_SET_REG8((void CYXDATA *)CYREG_PM_STBY_CFG0, CY_GET_REG8((void CYXDATA *)CYREG_PM_STBY_CFG0) & (uint8)~0x02u);	/* Disable CPU */
}
