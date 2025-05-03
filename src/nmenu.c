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

MENU_t *menu;
char *buff;

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

static int handler(const char* section, const char* name, const char* value)
{
	#define MATCH_SECTION(s) strcmp(section, s) == 0
	#define MATCH_NAME(n) strcmp(name, n) == 0
	#define MATCH(s, n) MATCH_SECTION(s) && MATCH_NAME(n)

	if (MATCH(SECTION_BACKGROUND, "bg.color")) {
		menu->bgColor = atoi(value);
	} else
	if (MATCH(SECTION_BACKGROUND, "bg.file.ansi")) {
		strcpy(menu->bgFileANSI, value);
	} else
	if (MATCH(SECTION_BACKGROUND, "bg.file.sc7")) {
		strcpy(menu->bgFileSC7, value);
	} else
	if (MATCH(SECTION_SETTINGS, "menu.bg.color")) {
		menu->menuBgColor = atoi(value);
	} else
	if (MATCH(SECTION_SETTINGS, "menu.fg.color")) {
		menu->menuFgColor = atoi(value);
	} else
	if (MATCH(SECTION_SETTINGS, "selected.bg.color")) {
		menu->selectedBgColor = atoi(value);
	} else
	if (MATCH(SECTION_SETTINGS, "selected.fg.color")) {
		menu->selectedFgColor = atoi(value);
	} else
	if (MATCH_SECTION(SECTION_LINES)) {
		char *dot = strchr(name, '.');
		if (dot) {
			*dot++ = '\0';
			int index = atoi(dot) - 1;
			if (index >= 0 && index < MAX_MENU_ENTRIES) {
				MENU_ENTRY_t *entry = &menu->entries[index];
				if (!entry->enabled) {
					entry->enabled = true;
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
					strcpy(entry->text, value);
				} else
				if (MATCH_NAME("exec")) {
					strcpy(entry->exec, value);
				}
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
bool menu_init()
{
	// Initialize string buffer
	buff = malloc(200);

	// Allocate memory for menu structure
	menu = (MENU_t*)malloc(sizeof(MENU_t));
	memset(menu, 0, sizeof(MENU_t));
	menu->entries = (MENU_ENTRY_t*)malloc(sizeof(MENU_ENTRY_t) * MAX_MENU_ENTRIES);
	memset(menu->entries, 0, sizeof(MENU_ENTRY_t) * MAX_MENU_ENTRIES);
	if (!menu || !menu->entries) {
		return false;
	}
	return true;
}

void menu_print()
{
	// Print menu entries
	MENU_ENTRY_t *entry = menu->entries;

	csprintf(buff, ANSI_BGCOL(%u)ANSI_FRCOL(%u), menu->menuBgColor, menu->menuFgColor);
	AnsiPrint(buff);
	for (uint8_t i = 0; i < MAX_MENU_ENTRIES; i++) {
		if (entry->text[0]) {
			csprintf(buff, ANSI_CURSORPOS(%u,%u)"%s", entry->posy, entry->posx, entry->text);
			AnsiPrint(buff);
		}
		entry++;
	}
}

bool menu_loop()
{
	// Set backgrounds
	AnsiPrint(ANSI_BGCOL(menu->menuBgColor) ANSI_CLRSCR);
	if (menu->bgFileANSI[0]) {
		// Load background ANSI file
		AnsiPrint(ANSI_CURSOROFF ANSI_BGCOL(menu->bgColor));
		AnsiPrint(menu->bgFileANSI);
	} else if (menu->bgFileSC7[0]) {
		// Load background SC7 file
		bloads(menu->bgFileSC7);
	}

	// Print full menu entries
	menu_print();

	// Menu loop
	bool end = false;
	uint8_t key, selected = 0, lastSel = 0xff;
	MENU_ENTRY_t *entry;

	while (!end) {
		if (selected != lastSel) {
			// Highlight selected entry
			if (lastSel < MAX_MENU_ENTRIES) {
				entry = &menu->entries[lastSel];
				csprintf(buff, ANSI_CURSORPOS(%u,%u)ANSI_BGCOL(%u)ANSI_FRCOL(%u)"%s",
					entry->posy, entry->posx, menu->menuBgColor, menu->menuFgColor, entry->text);
				AnsiPrint(buff);
			}
			entry = &menu->entries[selected];
			csprintf(buff, ANSI_CURSORPOS(%u,%u)ANSI_BGCOL(%u)ANSI_FRCOL(%u)"%s",
				entry->posy, entry->posx, menu->selectedBgColor, menu->selectedFgColor, entry->text);
			AnsiPrint(buff);
			lastSel = selected;
		}
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

	//Platform system checks
	checkPlatformSystem();

	// Initialize menu structure
	menu_init();

	// Initialize menu from INI file
	ini_parse("nmenu.ini", handler);

	AnsiInit();
	AnsiStartBuffer();
	AnsiPrint(ANSI_CURSOROFF);

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
