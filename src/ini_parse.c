/*
	Based and simplified from https://github.com/benhoyt/inih
*/
#include <ctype.h>
#include <string.h>
#include "ini.h"
#include "dos.h"
#include "heap.h"


#define MAX_SECTION 50
#define MAX_NAME 50

const char* const EMPTY_STRING = "\0";

/* Used by ini_parse_string() to keep track of string parsing state. */
typedef struct {
	const char* ptr;
	size_t num_left;
} ini_parse_string_ctx;

/* Strip whitespace chars off end of given string, in place. Return s. */
static char* rstrip(char* s)
{
	char* p = s + strlen(s);
#if INI_ALLOW_VALUE_TRIM
	while (p > s && isspace((unsigned char)(*--p)))
#else
	while (p > s && iscntrl((unsigned char)(*--p)))
#endif
		*p = '\0';
	return s;
}

/* Return pointer to first non-whitespace char in given string. */
static char* lskip(const char* s)
{
	while (*s && isspace((unsigned char)(*s)))
		s++;
	return (char*)s;
}

/* Return pointer to first char (of chars) or inline comment in given string,
   or pointer to NUL at end of string if neither found. Inline comment must
   be prefixed by a whitespace character to register as a comment. */
static char* find_chars_or_comment(const char* s, const char* chars)
{
	while (*s && (!chars || !strchr(chars, *s))) {
		s++;
	}
	return (char*)s;
}

/* Similar to strncpy, but ensures dest (size bytes) is
   NUL-terminated, and doesn't pad with NULs. */
static char* strncpy0(char* dest, const char* src, size_t size)
{
	/* Could use strncpy internally, but it causes gcc warnings (see issue #91) */
	size_t i;
	for (i = 0; i < size - 1 && src[i]; i++)
		dest[i] = src[i];
	dest[i] = '\0';
	return dest;
}

/* See documentation in header file. */
int ini_parse_stream(FILEH fh, ini_handler handler)
{
	/* Uses a fair bit of stack (use heap instead if you need to) */
#if INI_USE_STACK
	char line[INI_MAX_LINE];
	uint16_t max_line = INI_MAX_LINE;
#else
	char* line;
	uint16_t max_line = INI_INITIAL_ALLOC;
#endif
	char section[MAX_SECTION] = "";
	char prev_name[MAX_NAME] = "";

	char* start;
	char* end;
	char* name;
	char* value;
	uint16_t lineno = 0;
	uint16_t error = 0;

#if !INI_USE_STACK
	line = (char*)malloc(INI_INITIAL_ALLOC);
	if (!line) {
		return -2;
	}
#endif

#if INI_HANDLER_LINENO
#define HANDLER(s, n, v) handler(s, n, v, lineno)
#else
#define HANDLER(s, n, v) handler(s, n, v)
#endif

	/* Scan through stream line by line */
	while (dos2_fgets(line, (uint16_t)max_line, fh)) {
		lineno++;

		start = line;
		start = lskip(rstrip(start));

		if (strchr(INI_START_COMMENT_PREFIXES, *start)) {
			/* Start-of-line comment */
		}
		else if (*start == '[') {
			/* A "[section]" line */
			end = find_chars_or_comment(start + 1, "]");
			if (*end == ']') {
				*end = '\0';
				strncpy0(section, start + 1, sizeof(section));
				*prev_name = '\0';
#if INI_CALL_HANDLER_ON_NEW_SECTION
				if (!HANDLER(section, NULL, NULL) && !error)
					error = lineno;
#endif
			}
			else if (!error) {
				/* No ']' found on section line */
				error = lineno;
			}
		}
		else if (*start) {
			/* Not a comment, must be a name[=:]value pair */
			end = find_chars_or_comment(start, "=: \t");
			if (*end == '=' || *end == ':' || *end == ' ' || *end == '\t') {
				*end = '\0';
				name = rstrip(start);
				value = end + 1;
#if INI_ALLOW_VALUE_TRIM
				value = lskip(value);
#endif
				rstrip(value);

				/* Valid name[=:]value pair found, call handler */
				strncpy0(prev_name, name, sizeof(prev_name));
				if (!HANDLER(section, name, value) && !error)
					error = lineno;
			}
			else if (!error) {
				/* No '=' or ':' found on name[=:]value line */
#if INI_ALLOW_NO_VALUE
				*end = '\0';
				name = rstrip(start);
				if (!HANDLER(section, name, EMPTY_STRING) && !error)
					error = lineno;
#else
				error = lineno;
#endif
			}
		}

#if INI_STOP_ON_FIRST_ERROR
		if (error) {
			break;
		}
#endif
	}

#if !INI_USE_STACK
	free(INI_INITIAL_ALLOC);
#endif

	return error;
}

/* See documentation in header file. */
int ini_parse_file(FILEH fh, ini_handler handler)
{
	return ini_parse_stream(fh, handler);
}

/* See documentation in header file. */
int ini_parse(const char* filename, ini_handler handler)
{
	FILEH fh;
	int error;

	fh = dos2_fopen(filename, O_RDONLY);
	if (fh >= ERR_FIRST)
		return -1;
	error = ini_parse_file(fh, handler);
	dos2_fclose(fh);
	return error;
}
