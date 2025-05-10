#include <stdio.h>
#include <stdint.h>
#include <string.h>

/*
==============================================================================
 lVGM HEADER  (4 bytes)
==============================================================================
	xx[4]		"lVGM"            Light-VGM identifier

	xx			Options
				7 6 5 4 3 2 1 0
				│ │ │ │   │ │ └── Frequency (0: 60 Hz, 1: 50 Hz)
				│ │ │ │   │ └──── Loop data included
				│ │ │ │   └────── Devices (0: PSG only, 1: Device list incuded; see bellow)
				 └─┴─┴─┴────────── File format version (0)

	(xx)		Devices (only present if Options#2 is 1)
				7 6 5 4 3 2 1 0
				│   │ │ │ │ │ └── Include PSG data (AY-3-8910)
				│   │ │ │ │ └──── Include MSX-MUSIC data (YM2413/OPLL)
				│   │ │ │ └────── Include MSX-AUDIO data (Y8950/OPL1+ADPCM)
				│   │ └────────── Include SCC data (K051649)
				│   └──────────── Include SCC+ data (K052539)
				│   └──────────── Include secondary PSG data
				└──────────────── Include Moonsound data (YMF278/OPL4)

	(xx)		PSG - Most common value (only present if Options#2 is 0 or Devices#0 is 1)
*/

typedef struct {
	char fileId[4];
	union {
		struct {
			unsigned char freq   :1;	// 0: 60 Hz, 1: 50 Hz
			unsigned char loop   :1;	// Loop data included
			unsigned char device :2;	// Devices list included
			unsigned char ver    :4;	// File format version
		} options;
		uint8_t options_raw;
	};
	union {
		struct {
			unsigned char psg      :1;	// Include PSG data (AY-3-8910)
			unsigned char opll     :1;	// Include MSX-MUSIC data (YM2413/OPLL)
			unsigned char opl1     :1;	// Include MSX-AUDIO data (Y8950/OPL1+ADPCM)
			unsigned char scc      :1;	// Include SCC data (K051649)
			unsigned char scci     :1;	// Include SCC+ data (K052539)
			unsigned char psg2     :1;	// Include secondary PSG data
			unsigned char reserved :1; 	// Reserved
			unsigned char opl4     :1;	// Include Moonsound data (YMF278/OPL4)
		} devices;
		uint8_t devices_raw;
	};
} LVGM_INFO_t;



int main(int argc, char **argv)
{
	LVGM_INFO_t lvgm_info;
	FILE *fd;

	if ((fd = fopen(argv[1], "rb")) == NULL) {
		printf("Error opening file %s\n", argv[1]);
		return 1;
	}

	int len = fread(&lvgm_info, 1, sizeof(LVGM_INFO_t), fd);
	if (len != sizeof(LVGM_INFO_t)) {
		printf("Error reading file %s\n", argv[1]);
		fclose(fd);
		return 1;
	}

	// Fix: Compare the file ID properly without casting pointer to integer
	if (memcmp(lvgm_info.fileId, "lVGM", 4) != 0) {
		printf("Error: not a valid LVGM file\n");
		fclose(fd);
		return 1;
	}

	printf("ID        : %4s\n", lvgm_info.fileId);
	printf("Version   : %d\n", lvgm_info.options.ver);
	printf("Frequency : %s\n", lvgm_info.options.freq ? "50Hz" : "60Hz");
	printf("Loop      : %s\n", lvgm_info.options.loop ? "Yes" : "No");
	printf("Devices   : %s\n", lvgm_info.options.device ? "Yes" : "PSG Only");
	if (lvgm_info.options.device) {
		printf(" ├── PSG  : %s\n", lvgm_info.devices.psg ? "Present" : "No");
		printf(" ├── OPLL : %s\n", lvgm_info.devices.opll ? "Present" : "No");
		printf(" ├── OPL1 : %s\n", lvgm_info.devices.opl1 ? "Present" : "No");
		printf(" ├── SCC  : %s\n", lvgm_info.devices.scc ? "Present" : "No");
		printf(" ├── SCCI : %s\n", lvgm_info.devices.scci ? "Present" : "No");
		printf(" ├── PSG2 : %s\n", lvgm_info.devices.psg2 ? "Present" : "No");
		printf(" └── OPL4 : %s\n", lvgm_info.devices.opl4 ? "Present" : "No");
	}

	fclose(fd);
	return 0;
}