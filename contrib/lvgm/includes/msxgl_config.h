#pragma once

#ifndef TRUE
	#define TRUE					true
#endif
#ifndef FALSE
	#define FALSE					false
#endif

#define LVGM_USE_PSG				TRUE
#define LVGM_USE_MSXMUSIC			TRUE
#define LVGM_USE_MSXAUDIO			TRUE
#define LVGM_USE_SCC				TRUE
#define LVGM_USE_SCCI				TRUE
#define LVGM_USE_PSG2				TRUE
#define LVGM_USE_OPL4				TRUE
#define LVGM_USE_NOTIFY				TRUE

#define SCC_SLOT_MODE				SCC_SLOT_AUTO

#define TARGET						TARGET_DOS2

#define DOS_USE_FCB					FALSE
#define DOS_USE_HANDLE				TRUE

#define DOS_USE_UTILITIES			FALSE
#define DOS_USE_VALIDATOR			FALSE

#define MSX_VERSION					MSX_22P

#define BIOS_CALL_MAINROM			FALSE
#define BIOS_CALL_SUBROM			FALSE
#define BIOS_CALL_DISKROM			FALSE

#define VDP_VRAM_ADDR				VDP_VRAM_ADDR_17

#define VDP_UNIT					VDP_UNIT_U8

#define VDP_USE_VRAM16K				FALSE

#define VDP_USE_SPRITE				TRUE

#define VDP_USE_COMMAND				TRUE

#define VDP_USE_CUSTOM_CMD			FALSE

#define VDP_AUTO_INIT				TRUE

#define VDP_INIT_50HZ				VDP_INIT_ON

#define VDP_USE_UNDOCUMENTED		FALSE

#define VDP_USE_VALIDATOR			TRUE

#define VDP_USE_DEFAULT_PALETTE		TRUE
#define VDP_USE_MSX1_PALETTE		FALSE

#define VDP_USE_DEFAULT_SETTINGS	TRUE

#define VDP_USE_16X16_SPRITE		TRUE

#define VDP_USE_RESTORE_S0			TRUE

#define VDP_ISR_SAFE_MODE			VDP_ISR_SAFE_DEFAULT

#define VDP_USE_PALETTE16			FALSE

//-----------------------------------------------------------------------------
// INPUT MODULE
//-----------------------------------------------------------------------------

#define INPUT_USE_JOYSTICK			TRUE

#define INPUT_USE_KEYBOARD			TRUE

#define INPUT_USE_MOUSE				FALSE

#define INPUT_USE_DETECT			FALSE

#define INPUT_USE_ISR_PROTECTION	TRUE

#define INPUT_JOY_UPDATE			FALSE

#ifndef INPUT_KB_UPDATE // Add function to update all keyboard rows at once
	#define INPUT_KB_UPDATE				FALSE
#endif

#ifndef INPUT_KB_UPDATE // First row to update
	#define INPUT_KB_UPDATE_MIN			0
#endif

#ifndef INPUT_KB_UPDATE // Last row to update (10 for numerical-pad, 8 otherwise)
	#define INPUT_KB_UPDATE_MAX			8
#endif

//-----------------------------------------------------------------------------
// PADDLE MODULE
//-----------------------------------------------------------------------------

#ifndef PADDLE_USE_CALIB // Add support for Mouse handling functions
	#define PADDLE_USE_CALIB		FALSE
#endif

//-----------------------------------------------------------------------------
// MEMORY MODULE
//-----------------------------------------------------------------------------

// MEM_USE_VALIDATOR
#ifndef MEM_USE_VALIDATOR
	#define MEM_USE_VALIDATOR		FALSE
#endif

// MEM_USE_FASTCOPY
#ifndef MEM_USE_FASTCOPY
	#define MEM_USE_FASTCOPY		FALSE
#endif

// MEM_USE_FASTSET
#ifndef MEM_USE_FASTSET
	#define MEM_USE_FASTSET			FALSE
#endif

// MEM_USE_DYNAMIC
#ifndef MEM_USE_DYNAMIC
	#define MEM_USE_DYNAMIC			FALSE
#endif

// MEM_USE_BUILTIN
#ifndef MEM_USE_BUILTIN
	#define MEM_USE_BUILTIN			FALSE
#endif

//-----------------------------------------------------------------------------
// CLOCK MODULE
//-----------------------------------------------------------------------------
// #ifdef CHECK_DEFAULT_CLOCK

// // RTC_USE_CLOCK
// #ifndef RTC_USE_CLOCK
// 	#warning RTC_USE_CLOCK is not defined in "msxgl_config.h"! Default value will be used: TRUE
// 	#define RTC_USE_CLOCK			TRUE
// #endif

// // RTC_USE_CLOCK_EXTRA
// #ifndef RTC_USE_CLOCK_EXTRA
// 	#warning RTC_USE_CLOCK_EXTRA is not defined in "msxgl_config.h"! Default value will be used: TRUE
// 	#define RTC_USE_CLOCK_EXTRA		TRUE
// #endif

// // RTC_USE_SAVEDATA
// #ifndef RTC_USE_SAVEDATA
// 	#warning RTC_USE_SAVEDATA is not defined in "msxgl_config.h"! Default value will be used: TRUE
// 	#define RTC_USE_SAVEDATA		TRUE
// #endif

// // RTC_USE_SAVESIGNED
// #ifndef RTC_USE_SAVESIGNED
// 	#warning RTC_USE_SAVESIGNED is not defined in "msxgl_config.h"! Default value will be used: TRUE
// 	#define RTC_USE_SAVESIGNED		TRUE
// #endif

// #endif

//-----------------------------------------------------------------------------
// DRAW MODULE
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// PRINT MODULE
//-----------------------------------------------------------------------------

// PRINT_USE_VALIDATOR
#ifndef PRINT_USE_VALIDATOR
	#define PRINT_USE_VALIDATOR			FALSE
#endif
// PRINT_USE_VRAM
#ifndef PRINT_USE_VRAM
	#define PRINT_USE_VRAM				FALSE
#endif
// PRINT_USE_SPRITE
#ifndef PRINT_USE_SPRITE
	#define PRINT_USE_SPRITE			FALSE
#endif
// PRINT_USE_FX_SHADOW
#ifndef PRINT_USE_FX_SHADOW
	#define PRINT_USE_FX_SHADOW			FALSE
#endif
// PRINT_USE_FX_OUTLINE
#ifndef PRINT_USE_FX_OUTLINE
	#define PRINT_USE_FX_OUTLINE		FALSE
#endif
// PRINT_USE_2_PASS_FX
#ifndef PRINT_USE_2_PASS_FX
	#define PRINT_USE_2_PASS_FX		FALSE
#endif
// PRINT_USE_UNIT
#ifndef PRINT_USE_UNIT
	#define PRINT_USE_UNIT				FALSE
#endif
// PRINT_WIDTH
#ifndef PRINT_WIDTH
	#define PRINT_WIDTH					PRINT_WIDTH_8
#endif
// PRINT_HEIGHT
#ifndef PRINT_HEIGHT
	#define PRINT_HEIGHT				PRINT_HEIGHT_8
#endif
// PRINT_COLOR
#ifndef PRINT_COLOR_NUM
	#define PRINT_COLOR_NUM				1
#endif
// PRINT_SKIP_SPACE
#ifndef PRINT_SKIP_SPACE
	#define PRINT_SKIP_SPACE			FALSE
#endif
// PRINT_USE_GRAPH
#ifndef PRINT_USE_GRAPH
	#define PRINT_USE_GRAPH				TRUE
#endif
// PRINT_USE_FORMAT
#ifndef PRINT_USE_FORMAT
	#define PRINT_USE_FORMAT			TRUE
#endif
// PRINT_USE_32B
#ifndef PRINT_USE_32B
	#define PRINT_USE_32B				FALSE
#endif

//-----------------------------------------------------------------------------
// GAME MODULE
//-----------------------------------------------------------------------------

// GAME_USE_STATE
#ifndef GAME_USE_STATE
	#define GAME_USE_STATE				TRUE
#endif

// GAME_USE_VSYNC
#ifndef GAME_USE_VSYNC
	#define GAME_USE_VSYNC				TRUE
#endif

// GAME_USE_LOOP
#ifndef GAME_USE_LOOP
	#define GAME_USE_LOOP				TRUE
#endif

//-----------------------------------------------------------------------------
// STRING MODULE
//-----------------------------------------------------------------------------

// STRING_USE_FORMAT
#ifndef STRING_USE_FORMAT
	#define STRING_USE_FORMAT			TRUE
#endif

// STRING_USE_INT32
#ifndef STRING_USE_INT32
	#define STRING_USE_INT32			FALSE
#endif

// STRING_USE_FROM_INT8
#ifndef STRING_USE_FROM_INT8
	#define STRING_USE_FROM_INT8		TRUE
#endif

// STRING_USE_FROM_UINT8
#ifndef STRING_USE_FROM_UINT8
	#define STRING_USE_FROM_UINT8		TRUE
#endif

// STRING_USE_FROM_INT16
#ifndef STRING_USE_FROM_INT16
	#define STRING_USE_FROM_INT16		TRUE
#endif

// STRING_USE_FROM_UINT16
#ifndef STRING_USE_FROM_UINT16
	#define STRING_USE_FROM_UINT16		TRUE
#endif

//-----------------------------------------------------------------------------
// MATH MODULE
//-----------------------------------------------------------------------------

// RANDOM_8_METHOD
#ifndef RANDOM_8_METHOD
	#define RANDOM_8_METHOD				RANDOM_8_ION
#endif

// RANDOM_16_METHOD
#ifndef RANDOM_16_METHOD
	#define RANDOM_16_METHOD			RANDOM_16_XORSHIFT
#endif

//-----------------------------------------------------------------------------
// PCM ENCDER
//-----------------------------------------------------------------------------

// PCMENC_FREQ
#ifndef PCMENC_FREQ
	#define PCMENC_FREQ					PCMENC_11K
#endif

//-----------------------------------------------------------------------------
// PT3 MODULE
//-----------------------------------------------------------------------------

// PT3_SKIP_HEADER
#ifndef PT3_SKIP_HEADER
	#define PT3_SKIP_HEADER				TRUE
#endif

// PT3_AUTOPLAY
#ifndef PT3_AUTOPLAY
	#define PT3_AUTOPLAY				FALSE
#endif

// PT3_EXTRA
#ifndef PT3_EXTRA
	#define PT3_EXTRA					TRUE
#endif

//-----------------------------------------------------------------------------
// TRILO MODULE
//-----------------------------------------------------------------------------

// TRILO_USE_SFXPLAY
#ifndef TRILO_USE_SFXPLAY
	#define TRILO_USE_SFXPLAY			TRUE
#endif

// TRILO_USE_TREMOLO
#ifndef TRILO_USE_TREMOLO
	#define TRILO_USE_TREMOLO			TRUE
#endif

// TRILO_USE_TAIL
#ifndef TRILO_USE_TAIL
	#define TRILO_USE_TAIL				FALSE
#endif

//-----------------------------------------------------------------------------
// MISC
//-----------------------------------------------------------------------------

// DEBUG_TOOL
#if !defined(DEBUG_TOOL)
	#define DEBUG_TOOL					DEBUG_DISABLE
#endif

// PROFILE_LEVEL
#if !defined(PROFILE_LEVEL)
	#define PROFILE_LEVEL					10
#endif

