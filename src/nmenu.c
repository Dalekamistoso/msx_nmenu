/*
	Copyright (c) 2025 Natalia Pujol Cremades
	info@abitwitches.com

	See LICENSE file.
*/
#pragma opt_code_size
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "msx_const.h"
#include "dos.h"
#include "heap.h"
#include "utils.h"
#include "vdp.h"
#include "msx2ansi.h"
#include "ini.h"
#include "nmenu.h"


// ========================================================
static uint8_t msxVersionROM;
static uint8_t kanjiMode;
static uint8_t originalLINL40;
static uint8_t originalSCRMOD;
static uint8_t originalFORCLR;
static uint8_t originalBAKCLR;
static uint8_t originalBDRCLR;

const char *SECTION_BACKGROUND = "Background";
const char *SECTION_SETTINGS   = "Settings";
const char *SECTION_LINES      = "Lines";

const char entryPattern[] = ANSI_RESET ANSI_CURSORPOS(%u,%u)ANSI_COLOR(%s)ANSI_COLOR(%s)"%s";

MENU_t *menu;
uint8_t selected = 0;
uint8_t lastSel = 0xff;

char *buff;
void *heapBackup;


// ========================================================
// Function declarations

void restoreScreen();


// ========================================================
static void abortRoutine()
{
	restoreScreen();
	dos2_exit(1);
}

static void checkPlatformSystem()
{
	// Check MSX2 ROM or higher
	msxVersionROM = getRomByte(MSXVER);
	if (!msxVersionROM) {
		die("MSX1 not supported!");
	}

	// Check MSX-DOS 2 or higher
	if (dosVersion() < VER_MSXDOS2x) {
		die("MSX-DOS 2.x or higher required!");
	}

	// Set abort exit routine
	dos2_setAbortRoutine((void*)abortRoutine);

	// Backup original values
	originalLINL40 = varLINL40;
	originalSCRMOD = varSCRMOD;
	originalFORCLR = varFORCLR;
	originalBAKCLR = varBAKCLR;
	originalBDRCLR = varBDRCLR;
	kanjiMode = (detectKanjiDriver() ? getKanjiMode() : 0);
}


// ========================================================
uint8_t last_posx = 0;
uint8_t last_posy = 0;
uint8_t index = 0;

static int handler(const char* section, const char* name, const char* value)
{
	#define MATCH_SECTION(s) strcmp(section, s) == 0
	#define MATCH_NAME(n) strcmp(name, n) == 0
	#define MATCH(s, n) MATCH_SECTION(s) && MATCH_NAME(n)

	if (MATCH(SECTION_BACKGROUND, "bg.color")) {
		strncpy(menu->bgColor, value, MAX_COLOR-1);
	} else
	if (MATCH(SECTION_BACKGROUND, "bg.file.ansi")) {
		strcpy(menu->bgFileANSI, value);
	} else
	if (MATCH(SECTION_BACKGROUND, "bg.file.sc7")) {
		strcpy(menu->bgFileSC7, value);
	} else
	if (MATCH(SECTION_SETTINGS, "menu.bg.color")) {
		strncpy(menu->menuBgColor, value, MAX_COLOR-1);
	} else
	if (MATCH(SECTION_SETTINGS, "menu.fr.color")) {
		strncpy(menu->menuFgColor, value, MAX_COLOR-1);
	} else
	if (MATCH(SECTION_SETTINGS, "selected.bg.color")) {
		strncpy(menu->selectedBgColor, value, MAX_COLOR-1);
	} else
	if (MATCH(SECTION_SETTINGS, "selected.fr.color")) {
		strncpy(menu->selectedFgColor, value, MAX_COLOR-1);
	} else
	if (MATCH_SECTION(SECTION_LINES)) {
		if (index >= 0 && index < MAX_MENU_ENTRIES) {
			MENU_ENTRY_t *entry = &menu->entries[index];
			if (!entry->enabled) {
				entry->posx = last_posx;
				entry->posy = ++last_posy;
			}
			if (MATCH_NAME("posx")) {
				entry->enabled = true;
				last_posx = entry->posx = atoi(value);
			} else
			if (MATCH_NAME("posy")) {
				entry->enabled = true;
				last_posy = entry->posy = atoi(value);
			} else
			if (MATCH_NAME("text")) {
				entry->enabled = true;
				strcpy(entry->text, value);
			} else
			if (MATCH_NAME("cmd")) {
				strcpy(entry->exec, value);
			} else
			if (MATCH_NAME("next")) {
				index++;
			}
		}
	}
/*
sprintf(buff, "Section: %s, Name: %s, Value: %s", section, name, value);
cputs(buff);
*/
	return 1;
}


// ========================================================
void entry_print(MENU_ENTRY_t *entry, bool selected)
{
	// Print entry
	if (selected) {
		csprintf(buff, entryPattern,
			entry->posy, entry->posx, menu->selectedBgColor, menu->selectedFgColor, entry->text);
	} else {
		csprintf(buff, entryPattern,
			entry->posy, entry->posx, menu->menuBgColor, menu->menuFgColor, entry->text);
	}
	AnsiPrint(buff);
}

void menu_show()
{
	waitVDPready();
	disableVDP();

	// Set backgrounds
	AnsiPrint(ANSI_COLOR(menu->menuBgColor) ANSI_CLRSCR);
	if (menu->bgFileSC7[0] && fileexists(menu->bgFileSC7)) {
		// Load background SC7 file
		bloads(menu->bgFileSC7);
	}
	if (menu->bgFileANSI[0] && fileexists(menu->bgFileANSI)) {
		// Load background ANSI file
		AnsiPrint(ANSI_RESET);
		uint16_t size = filesize(menu->bgFileANSI);
		char *ansiBuffer = malloc(size + 1);
		loadFile(menu->bgFileANSI, ansiBuffer, size);
		AnsiPrint(ansiBuffer);
		free(size);
	}

	// Print full menu entries
	MENU_ENTRY_t *entry = menu->entries;

	for (uint8_t i = 0; i < MAX_MENU_ENTRIES; i++) {
		if (entry->text[0]) {
			entry_print(entry, false);
		}
		entry++;
	}

	waitVDPready();
	enableVDP();
}

bool menu_init(char *iniFilename)
{
	// Initialize heap to start value
	heap_top = heapBackup;

	// Initialize generic string buffer
	buff = malloc(200);

	// Allocate memory for menu structure
	menu = (MENU_t*)malloc(sizeof(MENU_t));
	memset(menu, 0, sizeof(MENU_t));
	menu->entries = (MENU_ENTRY_t*)malloc(sizeof(MENU_ENTRY_t) * MAX_MENU_ENTRIES);
	memset(menu->entries, 0, sizeof(MENU_ENTRY_t) * MAX_MENU_ENTRIES);
	if (!menu || !menu->entries) {
		return false;
	}

	// Initialize menu data from INI file
	index = 0;
	ini_parse(iniFilename, handler);

	// Start menu display
	menu_show();

	return true;
}

bool menu_loop()
{
	// Menu loop
	bool end = false;
	uint8_t key;
	MENU_ENTRY_t *entry;

	selected = 0;
	lastSel = 0xff;

	while (!end) {
		if (selected != lastSel) {
			// Clear last selected entry
			if (lastSel < MAX_MENU_ENTRIES) {
				entry_print(&menu->entries[lastSel], false);
			}
			// Highlight selected entry
			entry = &menu->entries[selected];
			lastSel = selected;
			entry_print(entry, true);
		}
		// Wait for a pressed key
 		key = getchar();
		switch (key) {
			case KEY_ESC:
				end = true;
				break;
			case KEY_UP:
				if (selected > 0) {
					selected--;
					while (selected > 0 && !menu->entries[selected].enabled) {
						selected--;
					}
					if (!menu->entries[selected].enabled) {
						selected = lastSel;
					}
				}
				break;
			case KEY_DOWN:
				if (selected < MAX_MENU_ENTRIES - 1) {
					selected++;
					while (selected < MAX_MENU_ENTRIES - 1 && !menu->entries[selected].enabled) {
						selected++;
					}
					if (!menu->entries[selected].enabled) {
						selected = lastSel;
					}
				}
				break;
			case KEY_RETURN:
			case KEY_SELECT:
			case KEY_SPACE:
				break;
		}
	}

	return true;
}

void restoreOriginalScreenMode() __naked
{
	// Restore original screen mode
	__asm
		ld   a, (_originalSCRMOD)
		or   a
		jr   nz, .screen1
		ld   ix, #INITXT				; Restore SCREEN 0
		jr   .bioscall
	.screen1:
		ld   ix, #INIT32				; Restore SCREEN 1
	.bioscall:
		JP_BIOSCALL
	__endasm;
}

void restoreScreen()
{
	// Clear & restore original screen parameters & colors
	__asm
		ld   ix, #DISSCR				; Disable screen
		BIOSCALL
		ld   ix, #INIFNK				; Restore function keys
		BIOSCALL
	__endasm;

	varLINL40 = originalLINL40;
	varFORCLR = originalFORCLR;
	varBAKCLR = originalBAKCLR;
	varBDRCLR = originalBDRCLR;

	if (kanjiMode) {
		// Restore kanji mode if needed
		setKanjiMode(kanjiMode);
	} else {
		// Restore original screen mode
		restoreOriginalScreenMode();
	}

	__asm
		ld   ix, #ENASCR
		BIOSCALL
	__endasm;

	// Restore abort routine
	dos2_setAbortRoutine((void*)0x0000);
}

// ========================================================
int main(char **argv, int argc) __sdcccall(0)
{
	argv, argc;

	// A way to avoid using low memory when using BIOS calls from DOS
	if (heap_top < (void*)0x8000)
		heap_top = (void*)0x8000;
	heapBackup = heap_top;

	//Platform system checks
	checkPlatformSystem();

	// Initialize ANSI screen
	AnsiInit();
	AnsiStartBuffer();
	AnsiPrint(ANSI_CURSOROFF);

	// Initialize menu structure
	menu_init("nmenu.ini");

/*
AnsiPrint(
	ANSI_FRCOL(7)ANSI_CURSOROFF
	ANSI_BGCOL(0)" ABC "
	ANSI_BGCOL(1)" ABC "
	ANSI_BGCOL(2)" ABC "
	ANSI_BGCOL(3)" ABC "
	ANSI_BGCOL(4)" ABC "
	ANSI_BGCOL(5)" ABC "
	ANSI_BGCOL(6)" ABC "
	ANSI_BGCOL(7)" ABC "
	"\n\r"ANSI_BGCOL(0)
	ANSI_FRCOL(0)" ABC "
	ANSI_FRCOL(1)" ABC "
	ANSI_FRCOL(2)" ABC "
	ANSI_FRCOL(3)" ABC "
	ANSI_FRCOL(4)" ABC "
	ANSI_FRCOL(5)" ABC "
	ANSI_FRCOL(6)" ABC "
	ANSI_FRCOL(7)" ABC "
	"\n\r"ANSI_FRCOL(7)
	ANSI_BGCOL1(0)" ABC "
	ANSI_BGCOL1(1)" ABC "
	ANSI_BGCOL1(2)" ABC "
	ANSI_BGCOL1(3)" ABC "
	ANSI_BGCOL1(4)" ABC "
	ANSI_BGCOL1(5)" ABC "
	ANSI_BGCOL1(6)" ABC "
	ANSI_BGCOL1(7)" ABC "
	"\n\r"ANSI_BGCOL(0)
	ANSI_FRCOL1(0)" ABC "
	ANSI_FRCOL1(1)" ABC "
	ANSI_FRCOL1(2)" ABC "
	ANSI_FRCOL1(3)" ABC "
	ANSI_FRCOL1(4)" ABC "
	ANSI_FRCOL1(5)" ABC "
	ANSI_FRCOL1(6)" ABC "
	ANSI_FRCOL1(7)" ABC "
	"\n\r"
);
AnsiPrint(ANSI_INVERSE);
AnsiPrint("Mundo\n\n"ANSI_RESET);
*/
	menu_loop();

	AnsiEndBuffer();
	AnsiFinish();

	restoreScreen();

	return 0;
}
