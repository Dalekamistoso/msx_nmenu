#include "dos.h"
#include "utils.h"


uint16_t loadFile(char *filename, void *destaddress, uint16_t size)
{ 
	RETW len = 0;
	if (fopen(filename)) {
		len = fread(destaddress, size);
		fclose();
	}
	return len;
}
