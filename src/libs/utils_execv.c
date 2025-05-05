#include <stdint.h>
#include <string.h>
#include "msx_const.h"
#include "dos.h"
#include "heap.h"
#include "utils.h"


#define LAUNCHER_BASE		0xc000


// ========================================================
static void launcher() __naked
{
	__asm
		/*
		    OPEN FILE HANDLE (43H)
		    Parameters:    C = 43H (_OPEN) 
		                DE = Drive/path/file ASCIIZ string
		                       or fileinfo block pointer
		                   A = Open mode. b0 set => no write
		                                  b1 set => no read
		                                  b2 set => inheritable   
		                                  b3..b7 -  must be clear
		    Results:       A = Error [NOFIL, IATTR, DIRX, NHAND, NORAM, ACCV, FILRO]
		                   B = New file handle
		*/
		ld   hl, (_heap_top)			; Get SHELL filename to read
		ex   de, hl
		ld   a, #O_RDONLY				; Open mode: read only
		ld   c, #OPEN					; DOS Call: OPEN FILE [FCB]
		DOSCALL
		/*
		    READ FROM FILE HANDLE (48H)
		    Parameters:    C = 48H (_READ) 
		                   B = File handle
		                  DE = Buffer address
		                  HL = Number of bytes to read  
		    Results:       A = Error [ACCV, EOF]
		                  HL = Number of bytes actually read
		*/
		ld   de, #0x0100				; Address to load the program
		ld   hl, #0xffff				; Read 0xffff bytes
		ld   c, #READ					; DOS Call: READ FILE [FCB]
		DOSCALL
		/*
		    CLOSE FILE HANDLE (45H)
		    Parameters:    C = 45H (_CLOSE) 
		                   B = File handle
		    Results:       A = Error
		*/
		ld   c, #CLOSE					; DOS Call: CLOSE FILE [FCB]
		DOSCALL

		ld   sp, (HIMEM)				; Set HIMEM to memory top
		jp   0x0100						; Execute the program

	__endasm;
}

// ========================================================
void execv(char *execStr)
{
	//Copy launcher to address 0xd000
	memcpy((void *)LAUNCHER_BASE, (const void *)launcher, 0x100);
	//Copy program arguments to 0x0080
	ADDR_POINTER_BYTE(0x0080) = strlen(execStr);
	memcpy((void *)0x81, execStr, 80);

	//Copy program SHELL COMMAND2.COM and location to heap_top
	char ret =dos2_getEnv("SHELL", (char *)heap_top, 255);

	__asm
		jp LAUNCHER_BASE
	__endasm;
}
