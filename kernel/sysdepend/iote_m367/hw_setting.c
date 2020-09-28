/*
 *----------------------------------------------------------------------
 *    micro T-Kernel 3.00.02.B1
 *
 *    Copyright (C) 2006-2020 by Ken Sakamura.
 *    This software is distributed under the T-License 2.2.
 *----------------------------------------------------------------------
 *
 *    Released by TRON Forum(http://www.tron.org) at 2020/10/.
 *
 *----------------------------------------------------------------------
 */

#include <sys/machine.h>
#ifdef IOTE_M367

/*
 *	hw_setting.c (M367 IoT-Engine)
 *	startup / shoutdown processing for hardware
 */

#include "kernel.h"
#include <tm/tmonitor.h>

#include "sysdepend.h"

/* 
 * Setup pin functions Tadle
 */
typedef struct {
	UW	addr;
	UW	data;
} T_SETUP_REG;

LOCAL const T_SETUP_REG setup_regs[] = {
	// PA4	RTS5		Console UART
	// PA5	RXD5
	// PA6	TXD5
	// PA7	CTS5
	{PORT_FR1(A),	0x0000000f},
	{PORT_FR2(A),	0x000000f0},
	{PORT_PUP(A),	0x0000000a},
	{PORT_CR(A),	0x00000053},
	{PORT_IE(A),	0x000000ae},

	// PE5	TXD1		UART
	// PE6	RXD1
	{PORT_FR1(E),	0x00000060},
	{PORT_CR(E),	0x00000020},
	{PORT_IE(E),	0x00000040},

	// PF5	P-IN		CD (SD Card)
	// PF6	SCL1		I2C
	// PF7	SDA1
	{PORT_FR4(F),	0x000000c0},
	{PORT_OD(F),	0x000000c0},
	{PORT_CR(F),	0x000000c0},
	{PORT_IE(F),	0x000000e0},

	// PK1	P-OUT		CS (SD Card)
	// PK2	SP0DI		SPI (SD Card)
	// PK3	SP0DO
	// PK4	SP0CLK-OUT
	{PORT_DATA(K),	0x00000002},
	{PORT_FR2(K),	0x0000001c},
	{PORT_CR(K),	0x0000001a},
	{PORT_IE(K),	0x00000004},

	// PL1	P-OUT		I2C_ENABLE
	{PORT_DATA(L),	0x00000000},
	{PORT_CR(L),	0x00000002},

	{0, 0}
};

/*
 * Startup Device
 */
EXPORT void knl_startup_hw(void)
{
	const T_SETUP_REG	*p;

	/* Disable Watch Dog Timer */
	*(_UW*)WDOG_WDMOD &= ~0x00000080;
	*(_UW*)WDOG_WDCR = WDOG_WDCR_DISABLE;

	startup_clock(PLL_MODE_6X);

	/* Setup Pin Function */
	for(p = setup_regs; p->addr != 0; p++) {
		*(_UW*)(p->addr) = p->data;
	}
}

#if USE_SHUTDOWN
/*
 * Shutdown device
 */
EXPORT void knl_shutdown_hw( void )
{
	disint();
	while(1);
}
#endif /* USE_SHUTDOWN */

/*
 * Re-start device
 *	mode = -1		reset and re-start	(Reset -> Boot -> Start)
 *	mode = -2		fast re-start		(Start)
 *	mode = -3		Normal re-start		(Boot -> Start)
 */
EXPORT ER knl_restart_hw( W mode )
{
	switch(mode) {
	case -1: /* Reset and re-start */
		SYSTEM_MESSAGE("\n<< SYSTEM RESET & RESTART >>\n");
		return E_NOSPT;
	case -2: /* fast re-start */
		SYSTEM_MESSAGE("\n<< SYSTEM FAST RESTART >>\n");
		return E_NOSPT;
	case -3: /* Normal re-start */
		SYSTEM_MESSAGE("\n<< SYSTEM RESTART >>\n");
		return E_NOSPT;
	default:
		return E_PAR;
	}
}


#endif /* IOTE_M367 */