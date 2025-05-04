/*
	Based and simplified from https://github.com/benhoyt/inih
*/
#ifndef __INI_H__
#define __INI_H__

#include <stdio.h>
#include <stdint.h>
#include "dos.h"


/* Nonzero if ini_handler callback should accept lineno parameter. */
#define INI_HANDLER_LINENO 0

/* Typedef for prototype of handler function. */
#if INI_HANDLER_LINENO
typedef int (*ini_handler)(const char* section, const char* name, const char* value, int lineno);
#else
typedef int (*ini_handler)(const char* section, const char* name, const char* value);
#endif

/* Parse given INI-style file. May have [section]s, name=value pairs
   (whitespace stripped), and comments starting with ';' (semicolon). Section
   is "" if name=value pair parsed before any section heading. name:value
   pairs are also supported as a concession to Python's configparser.
   For each name=value pair parsed, call handler function with given section, 
   name, and value (data only valid for duration of handler call). Handler 
   should return nonzero on success, zero on error.
   Returns 0 on success, line number of first error on parse error (doesn't
   stop on first error), -1 on file open error, or -2 on memory allocation
   error (only when INI_USE_STACK is zero).
*/
int ini_parse(const char* filename, ini_handler handler);

/* Same as ini_parse(), but takes a file handle instead of filename. This doesn't
   close the file when it's finished -- the caller must do that. */
int ini_parse_stream(FILEH fh, ini_handler handler);

/* Chars that begin a start-of-line comment. Per Python configparser, allow
   both ; and # comments at the start of a line by default. */
#ifndef INI_START_COMMENT_PREFIXES
#define INI_START_COMMENT_PREFIXES ";#"
#endif

/* Nonzero to use stack for line buffer, zero to use heap (malloc/free). */
#ifndef INI_USE_STACK
#define INI_USE_STACK 1
#endif

/* Maximum line length for any line in INI file (stack or heap). Note that
   this must be 3 more than the longest line (due to '\r', '\n', and '\0'). */
#ifndef INI_MAX_LINE
#define INI_MAX_LINE 200
#endif

/* Initial size in bytes for heap line buffer. Only applies if INI_USE_STACK
   is zero. */
#ifndef INI_INITIAL_ALLOC
#define INI_INITIAL_ALLOC 200
#endif

/* Stop parsing on first error (default is to keep parsing). */
#ifndef INI_STOP_ON_FIRST_ERROR
#define INI_STOP_ON_FIRST_ERROR 0
#endif

/* Nonzero to call the handler at the start of each new section (with
   name and value NULL). Default is to only call the handler on
   each name=value pair. */
#ifndef INI_CALL_HANDLER_ON_NEW_SECTION
#define INI_CALL_HANDLER_ON_NEW_SECTION 0
#endif

/* Nonzero to allow a name without a value (no '=' or ':' on the line) and
   call the handler with value NULL in this case. Default is to treat
   no-value lines as an error. */
#ifndef INI_ALLOW_NO_VALUE
#define INI_ALLOW_NO_VALUE 1
#endif

/* Nonzero to allow whitespaces before and after the value string, and
   '=' or ':' characters. Default is to not allow any whitespace. */
#ifndef INI_ALLOW_VALUE_TRIM
#define INI_ALLOW_VALUE_TRIM 0
#endif


#endif	//__INI_H__
