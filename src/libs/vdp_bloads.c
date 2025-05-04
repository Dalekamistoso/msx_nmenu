#include <stdint.h>
#include <stdbool.h>
#include "msx_const.h"
#include "dos.h"
#include "heap.h"
#include "vdp.h"

#define LOAD_CHUNK_SIZE		8192


bool bloads(const char* filename)
{
	bool ret = false;
	FILEH fh;
	BLOAD_HEADER header;

	// Open file
	if ((fh = dos2_fopen(filename, O_RDONLY)) >= ERR_FIRST) {
		return false;
	}

	// Read header
	if (dos2_fread((char*)&header, sizeof(BLOAD_HEADER), fh) != sizeof(BLOAD_HEADER) ||
		header.marker != 0xFE)
	{
		goto end_function;
	}

	uint16_t size = header.end - header.start;
	uint16_t sizeToRead, address = header.start;
	char *buffer = malloc(LOAD_CHUNK_SIZE);

	while (size) {
		sizeToRead = LOAD_CHUNK_SIZE;
		if (sizeToRead > size) {
			sizeToRead = size;
		}
		if ((dos2_fread(buffer, sizeToRead, fh) >> 8) == 0xff) {
			goto end_error;
		}
		bios_copyToVRAM(buffer, address, sizeToRead);
		address += sizeToRead;
		size -= sizeToRead;
	}
	ret = true;

end_error:
	free(LOAD_CHUNK_SIZE);
end_function:
	dos2_fclose(fh);
	return ret;
}