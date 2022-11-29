/*******************************************************************************
* File Name: cyfitter_cfg.h
* 
* PSoC Creator  4.4
*
* Description:
* This file provides basic startup and mux configuration settings
* This file is automatically generated by PSoC Creator.
*
********************************************************************************
* Copyright (c) 2007-2020 Cypress Semiconductor.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions, 
* disclaimers, and limitations in the end user license agreement accompanying 
* the software package with which this file was provided.
********************************************************************************/

#ifndef CYFITTER_CFG_H
#define CYFITTER_CFG_H

#include "cytypes.h"

extern void cyfitter_cfg(void);

/* For internal use in cyfitter_cfg */
#ifndef CYCONFIGCPY_DECLARED
	extern void cymemzero(void CYFAR *addr, uint16 size);
	extern void cyconfigcpy(uint16 size, const void CYFAR *src, void CYFAR *dest) small;
	extern void cyconfigcpycode(uint16 size, const void CYCODE *src, void CYFAR *dest);
#endif

/* For internal use in cyfitter_cfg */
extern void cfg_write_bytes_code(const void CYCODE *table);
/* For internal use in cyfitter_cfg */
extern void cfg_write_bytes(const void CYFAR *table);
/* Analog Set/Unset methods */
extern void SetAnalogRoutingPumps(uint8 enabled);
extern void CapSense_AMuxCH0_Set(uint8 channel);
extern void CapSense_AMuxCH0_Unset(uint8 channel);
/* CapSense_AMuxCH0__addrTable is an implementation detail of the AMux.
   Code that depends on it may be incompatible with other versions
   of PSoC Creator. */
extern uint8 CYXDATA * const CYCODE CapSense_AMuxCH0__addrTable[10];
/* CapSense_AMuxCH0__maskTable is an implementation detail of the AMux.
   Code that depends on it may be incompatible with other versions
   of PSoC Creator. */
extern const uint8 CYCODE CapSense_AMuxCH0__maskTable[10];


#endif /* CYFITTER_CFG_H */

/*[]*/
