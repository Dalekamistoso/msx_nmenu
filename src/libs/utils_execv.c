#include <stdint.h>
#include <string.h>
#include "dos.h"
#include "heap.h"
#include "utils.h"


FILEH executableFile;

// ========================================================
// Thanks to Laurent Halter for his help with this code
void execv(char *execStr)
{
	//Copy program arguments to 0x0080
	ADDR_POINTER_BYTE(0x0080) = strlen(execStr);
	strncpy((void *)0x81, execStr, 80);

	//Copy program SHELL (usually A:\COMMAND2.COM) to heap_top
	dos2_getEnv("SHELL", (char *)heap_top, 255);
	if (heap_top[0] == 0) {
		strcpy(heap_top, "A:\\COMMAND2.COM");
	}
	
	executableFile = dos2_fopen((char *)heap_top, O_RDONLY);
	if (executableFile >= ERR_FIRST) {
		return;
	}

	__asm
		ld   sp,(0x0006)

		ld   de,#0x100					// DE = Address to load file
		push de							// Set execution address to 0x0100 when returns from DOSCALL
		ld   bc,(_executableFile - 1)	// B = file handle
		ld   hl,#0xFFFF					// HL = size to load from file
		ld   c,#READ					// C = READ (MSXDOS2 BIOS call)
		DOSCALL
	__endasm;
}
