#pragma once
#include <stdint.h>


#if SCREEN!=8 && SCREEN!=7
	#define VRAM_PAGE1   0x08000
#else
	#define VRAM_PAGE1   0x10000
#endif


typedef struct {
	uint8_t  marker;	// [1] 0x00 - Always 0xFE
	uint16_t start;		// [2] 0x01 - Start address
	uint16_t end;		// [2] 0x03 - End address
	uint16_t exec;		// [2] 0x05 - Execution address
} BLOAD_HEADER;


//=========================================================
// MSX1

void setRegVDP(char reg, char value) __sdcccall(1);
void setRegVDP8(char reg, char value) __sdcccall(1);

void setVDP_Read(uint32_t vram) __sdcccall(1);
void setVDP_Write(uint32_t vram) __sdcccall(1);

//void bios_fillVRAM(uint16_t vram, uint16_t size, uint8_t value) __sdcccall(0);
void bios_copyToVRAM(void *address, uint16_t vram, uint16_t size) __sdcccall(0);
//void msx1_copyToVRAM(void *address, uint16_t vram, uint16_t size) __naked __sdcccall(0);
void msx2_copyToVRAM(void *address, uint16_t vram, uint16_t size) __sdcccall(0);
//void bios_copyFromVRAM(uint16_t vram, void *address, uint16_t size) __sdcccall(0);

void enableVDP();
void disableVDP();

//void enableSPR();
//void disableSPR();

//void setScreen0();
//void setScreen2();
//void setSPR16();

//void setColor(char forecolor, char background, char border) __sdcccall(0);
//void setBorder(char border) __sdcccall(1);

//void sc2_spritesInit();
//void sc2_putSprite(uint8_t layer, uint8_t x, uint8_t y, uint8_t color, uint8_t pattern) __sdcccall(0);

//uint16_t unpackToVRAM(uint16_t address) __z88dk_fastcall;

//=========================================================
// MSX2

// BitBlt commands
#define CMD_HMMC	0b11110000       // High speed move CPU to VRAM: transfer data from the CPU to VRAM
#define CMD_YMMM	0b11100000       // High speed move VRAM to VRAM, Y coordinate only: transfer VRAM to VRAM in Y coordinate
#define CMD_HMMM	0b11010000       // High speed move VRAM to VRAM: transfer data from VRAM to VRAM
#define CMD_HMMV	0b11000000       // High speed move VDP to VRAM: paint in a specific rectangular area in the VRAM
#define CMD_LMMC	0b10110000       // Logical move CPU to VRAM: transfer data from the CPU to VRAM
#define CMD_LMCM	0b10100000       // Logical move VRAM to CPU: transfer data from VRAM to the CPU through VDP
#define CMD_LMMM	0b10010000       // Logical move VRAM to VRAM: transfer data from VRAM to VRAM
#define CMD_LMMV	0b10000000       // Logical move VDP to VRAM: paint in a specific rectangular area in the VRAM
#define CMD_LINE	0b01110000       // LINE command: used to draw straight line in VRAM
#define CMD_SRCH	0b01100000       // SRCH command: used to search for the specific color in VRAM
#define CMD_PSET	0b01010000       // PSET command: used to draw a dot in VRAM
#define CMD_POINT	0b01000000       // POINT command: used to read the color of the specified dot located in VRAM
#define CMD_STOP	0b00000000       // 
// Logical operations
#define LOG_IMP		0b00000000       // DC=SC
#define LOG_AND		0b00000001       // DC=SC & DC
#define LOG_OR		0b00000010       // DC=SC | DC
#define LOG_XOR		0b00000011       // DC=SC ^ DC
#define LOG_NOT		0b00000100       // DC=!SC
#define LOG_TIMP	0b00001000       // if SC=0 then DC=DC else DC=SC
#define LOG_TAND	0b00001001       // if SC=0 then DC=DC else DC=SC & DC
#define LOG_TOR	 	0b00001010       // if SC=0 then DC=DC else DC=SC | DC
#define LOG_TXOR	0b00001011       // if SC=0 then DC=DC else DC=SC ^ DC
#define LOG_TNOT	0b00001100       // if SC=0 then DC=DC else DC=!SC
// Most used combinations
#define LMMM_TIMP	CMD_LMMM | LOG_TIMP

typedef struct {
	uint16_t sx;
	uint16_t sy;
	uint16_t dx;
	uint16_t dy;
	uint16_t nx;
	uint16_t ny;
	uint8_t  col;
	uint8_t  arg;
	uint8_t  cmd;
} BITBLT;


//void enable50Hz();
//void enable60Hz();

//void enable192lines();
//void enable212lines();

//void enableInterlacedLines();
//void disableInterlacedLines();

void setPalette(char *paletteAddress) __sdcccall(1);
void setColorPal(uint8_t colIndex, uint16_t grb) __sdcccall(1);	// grb bitfield: 00000GGG.0RRR0BBB

void setVPage(uint8_t page) __sdcccall(1);
void copyToPage1();

//void clearSC5();
//void clearSC5lines(uint16_t startline, uint16_t numLines) __sdcccall(1);
void clearSC7();
void clearSC7lines(uint16_t startline, uint16_t numLines) __sdcccall(1);

void bitBlt(uint16_t sx, uint16_t sy, uint16_t dx, uint16_t dy, uint16_t nx, uint16_t ny, uint8_t col, uint8_t arg, uint8_t cmd) __sdcccall(0);
void fastVCopy(char *bitbltData) __sdcccall(1);
void waitVDPready();

bool bloads(const char* filename);