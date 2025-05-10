#include "dos.h"
#include "utils.h"


uint16_t loadFile(char *filename, void *destaddress, uint16_t size)
{ 
	FILEH fh;
	RETW len = 0;
	if ((fh = dos2_fopen(filename, O_RDONLY)) < ERR_FIRST) {
		len = dos2_fread(destaddress, size, fh);
		dos2_fclose(fh);
	}
	return len;
}
