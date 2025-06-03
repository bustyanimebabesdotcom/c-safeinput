/**
 * safeinput.c - version 1.1.0
 *
 * Author:   https://github.com/bustyanimebabesdotcom
 * License:  The Unlicense
 *
 * Safe(r) input handling for C.
 *
 * This library provides a standalone, portable alternative to `scanf`,
 * focusing on input validation and buffer overflow prevention.
 *
 * It is designed for simplicity, robustness, and use in projects
 * where `scanf` and `gets` would otherwise be used by people who
 * unfortunately don't know any better.
 *
 * Main safety features:
 *   - Input length enforcement
 *   - Graceful EOF handling
 *   - Error feedback to stderr
 *   - Non-null-terminated byte-string support
 *
 * Phase 2 completed:
 *   - Implemented si_string (byte-counted, no null terminator)
 *   - Byte-for-byte `getchar()` loop with bounds checking
 *
 * TODO:
 *   - Wrap repeated logic into reusable helpers
 *   - Profile and optimize where sensible
 *
 * Use this in any project, commercial or personal.
 * Attribution is appreciated but not required.
 *
 * For more information, see LICENSE or visit <https://unlicense.org/>
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <limits.h>
#include <errno.h>
#include <math.h>
#include <stdbool.h>
#include <safeinput/safeinput.h>

/**
 * si_printError - calls fputs into stderr, simple wrapper
 */
static inline void si_printError ( const char *msg ) {

	fputs( msg, stderr );
}



/**
 * si_drainStdin - Drains leftover input from stdin to prevent buffer overflows.
 * Reads until newline or EOF is encountered
 * 
 * called by si_readByte()
 */
static inline void si_drainStdin ( void ) {

	int c;

	while ( ( c = getchar_unlocked() ) != '\n' && c != EOF )
		;
}



/**
 * si_readByte - read up to maxLen bytes from stdin into buffer, 
 * 					stop at newline or EOF, drain excess input, and report length.
 * 
 * @buf:		buffer to fill with input characters
 * @maxLen:		maximum number of bytes to read into buf
 * @outLen:		pointer to size_t to receive the number of bytes read
 * 
 * Returns:
 * 		0 - on success
 * 		1 - on error ( NULL buf or outLen, or maxLen < 1 )
 *	   -1 - on EOF ( caller should check and handle appropriately )
 */
static inline int si_readByte ( char *buf, size_t maxLen, size_t *outLen ) {

	if ( !buf || !outLen || maxLen < 1 ) return 1;

	size_t i = 0;
	int c;

	for ( ; i < maxLen; i++ ) {
		c = getchar_unlocked();
		if ( c == '\n' || c == EOF ) break;
		buf[i] = (char)c;
	}

	*outLen = i;

	if ( i == maxLen && c != '\n' && c != EOF ) {
		si_drainStdin();
		si_printError( "Input exceeding buffer size. Try again.\n" );
		return 1;
	}

	if ( c == EOF && i == 0 ) return EOF;

	return 0;
}



/**
 * si_getInt - a safer alternative to scanf for integers
 *
 * usage - int x = si_getInt();
 * 
 * returns INT_MIN on error or EOF
 */
int si_getInt ( void ) {

	char buffer[INPUT_BUFFER_SIZE];
	char *endptr;
	long value;

	while ( 1 ) {

		size_t len;
		if ( si_readByte( buffer, sizeof(buffer) - 1, &len )) return INT_MIN;
		buffer[len] = '\0';

		errno = 0;
		value = strtol( buffer, &endptr, 10 );
		
		if ( endptr == buffer || *endptr != '\0' || errno == ERANGE || value != (int)value ) {
			si_printError( "Invalid input. Try again.\n" );
			continue;
		}

		return (int)value;
	}
}



/**
 * si_getUInt - a safer alternative to scanf for unsigned integers
 *
 * usage - unsigned int x = si_getUInt();
 * 
 * returns UINT_MAX on error or EOF
 */
unsigned int si_getUInt ( void ) {

	char buffer[INPUT_BUFFER_SIZE];
	char *endptr;
	unsigned long value;

	while ( 1 ) {

		size_t len;
		if ( si_readByte( buffer, sizeof(buffer) - 1, &len )) return UINT_MAX;
		buffer[len] = '\0';

		errno = 0;
		value = strtoul( buffer, &endptr, 10 );

		if ( buffer[0] == '-' ) {
			si_printError( "Value can not be negative.\n" );
			continue;
		}

		if ( endptr == buffer || *endptr != '\0' || errno == ERANGE || value > UINT_MAX ) {
			si_printError( "Invalid input. Try again.\n" );
			continue;
		}

		return (unsigned int)value;
	}
}



/**
 * si_getFloat - a safer alternative to scanf for floating point numbers
 *
 * usage - float x = si_getFloat();
 * 
 * returns NAN on error or EOF
 */
float si_getFloat ( void ) {

	char buffer[INPUT_BUFFER_SIZE];
	char *endptr;
	float value;

	while ( 1 ) {

		size_t len;
		if ( si_readByte( buffer, sizeof(buffer) - 1, &len )) return NAN;
		buffer[len] = '\0';

		errno = 0;
		value = strtof( buffer, &endptr );

		if ( endptr == buffer || *endptr != '\0' || errno == ERANGE || !isfinite( value )) {
			si_printError( "Invalid input. Try again.\n" );
			continue;
		}

		return value;
	}
}



/**
 * si_getDouble - a safer alternative to scanf for doubles
 *
 * usage - double x = si_getDouble();
 * 
 * returns NAN on error or EOF
 */
double si_getDouble ( void ) {

	char buffer[INPUT_BUFFER_SIZE];
	char *endptr;
	double value;

	while ( 1 ) {

		size_t len;
		if ( si_readByte( buffer, sizeof(buffer) - 1, &len )) return NAN;
		buffer[len] = '\0';

		errno = 0;
		value = strtod( buffer, &endptr );
		
		if ( endptr == buffer || *endptr != '\0' || errno == ERANGE || !isfinite( value )) {
			si_printError( "Invalid input. Try again.\n" );
			continue;
		}

		return value;
	}
}



/**
 * si_getLong - a safer alternative to scanf for longs
 *
 * usage - long x = si_getLong();
 * 
 * returns LONG_MIN on error or EOF
 */
long si_getLong ( void ) {

	char buffer[INPUT_BUFFER_SIZE];
	char *endptr;
	long value;

	while ( 1 ) {

		size_t len;
		if ( si_readByte( buffer, sizeof(buffer) - 1, &len )) return LONG_MIN;
		buffer[len] = '\0';

		errno = 0;
		value = strtol( buffer, &endptr, 10 );
		
		if ( endptr == buffer || *endptr != '\0' || errno == ERANGE) {
			si_printError( "Invalid input. Try again.\n" );
			continue;
		}

		return value;
	}
}



/**
 * si_getULong - a safer alternative to scanf for unsigned longs
 *
 * usage - unsigned long x = si_getULong();
 * 
 * returns ULONG_MAX on error or EOF
 */
unsigned long si_getULong ( void ) {

	char buffer[INPUT_BUFFER_SIZE];
	char *endptr;
	unsigned long value;

	while ( 1 ) {

		size_t len;
		if ( si_readByte( buffer, sizeof(buffer) - 1, &len )) return ULONG_MAX;
		buffer[len] = '\0';

		errno = 0;
		value = strtoul( buffer, &endptr, 10 );
		
		if ( endptr == buffer || *endptr != '\0' || errno == ERANGE ) {
			si_printError( "Invalid input. Try again.\n" );
			continue;
		}

		return value;
	}
}



/**
 * si_getLongLong - a safer alternative to scanf for long longs
 *
 * usage - long long x = si_getLongLong();
 * 
 * returns LLONG_MIN on error or EOF
 */
long long si_getLongLong ( void ) {

	char buffer[INPUT_BUFFER_SIZE];
	char *endptr;
	long long value;

	while ( 1 ) {

		size_t len;
		if ( si_readByte( buffer, sizeof(buffer) - 1, &len )) return LLONG_MIN;
		buffer[len] = '\0';

		errno = 0;
		value = strtoll( buffer, &endptr, 10 );
		
		if ( endptr == buffer || *endptr != '\0' || errno == ERANGE ) {
			si_printError( "Invalid input. Try again.\n" );
			continue;
		}

		return value;
	}
}



/**
 * si_getULongLong - a safer alternative to scanf for unsigned long longs
 *
 * usage - unsigned long long x = si_getULongLong();
 * 
 * returns ULLONG_MAX on error or EOF
 */
unsigned long long si_getULongLong ( void ) {

	char buffer[INPUT_BUFFER_SIZE];
	char *endptr;
	unsigned long long value;

	while ( 1 ) {

		size_t len;
		if ( si_readByte( buffer, sizeof(buffer) - 1, &len )) return ULLONG_MAX;
		buffer[len] = '\0';

		errno = 0;
		value = strtoull( buffer, &endptr, 10 );
		
		if ( endptr == buffer || *endptr != '\0' || errno == ERANGE ) {
			si_printError( "Invalid input. Try again.\n" );
			continue;
		}

		return value;
	}
}



/**
 * si_getChar - a safer alternative to scanf for chars
 *
 * usage - int x = si_getChar();
 *
 * IMPORTANT: Only accepts one character. " a" and "a " are considered invalid.
 * 
 * returns EOF on error or EOF
 */
int si_getChar ( void ) {

	char buffer[CHAR_INPUT_BUFFER_SIZE];
	size_t len;

	while ( 1 ) {

		int result = si_readByte( buffer, sizeof(buffer) - 1, &len );
		buffer[len] = '\0';

		if ( result == EOF ) return EOF;
		if ( result == 1 ) continue;
		if ( len == 0 ) return '\n';
		if ( len == 1 ) return (unsigned char)buffer[0];
		si_printError( "Invalid input. Please enter a single character.\n" );

	}
}



/**
 * si_getCharFiltered - a safer alternative to scanf for chars
 *
 * usage - int x = si_getCharFiltered( "abc" );
 * 
 * returns EOF on error or EOF
 */
int si_getCharFiltered ( const char *allowed ) {

	if ( !allowed ){
		si_printError( "ERROR: NULL passed to 'allowed'.\n" );
		exit(EXIT_FAILURE);
	}

	if ( allowed[0] == '\0' ) {
		si_printError( "No allowed characters specified. Exiting.\n" );
		return 1;
	}

	char buffer[CHAR_INPUT_BUFFER_SIZE];
	size_t len;

	while ( 1 ) {

		int result = si_readByte( buffer, sizeof(buffer) - 1, &len );
		buffer[len] = '\0';
		
		if ( result == EOF ) return EOF;
		if ( result == 1 ) continue;
		
		// Ensure input is exactly one character
		if ( len != 1 ) {
			si_printError( "Invalid input. Please enter a single character.\n" );
			continue;
		}

		char c = buffer[0];

		if ( strchr( allowed, c ) != NULL ) return (unsigned char)c;
	
		fprintf( stderr, "Invalid input. Allowed: %s\n", allowed );
		
	}
}



/**
 * si_getCString - allocates and returns a line of user input from stdin
 *
 * NOTE: 	Caller must free the returned string!!!
 * 
 *
 * safe usage example
 * 
 *
 *	char *input = si_getCString();
 *
 *	if (!input) fputs( "Failed to read input. Try again.\n", stderr );
 *	else {
 *		printf("You entered: %s\n", input);
 *		free(input);
 *		input = NULL;
 *	}
 *
 * NOTE: 	we use memcpy() because it copies exactly len + 1 bytes from buffer, 
 *			avoiding overflow and termination issues.
 * 			the overhead is minimal, memcpy() is well optimized.
 * 
 * returns NULL on error or EOF
 */
char *si_getCString ( void ) {

	char buffer[INPUT_BUFFER_SIZE];
	size_t len;

	// read input (return NULL on error/EOF )
	if ( si_readByte( buffer, sizeof(buffer)-1, &len )) return NULL;

	// add null-terminator for C string
	buffer[len] = '\0';

	// Allocate memory for string( length + 1 for null terminator )
	char *str = malloc( len+1 );
	// Check for allocation failure
	if ( !str ) {
		si_printError( "Memory allocation failed.\n" );
		return NULL;
	}

	// copy buffer to allocated string
	memcpy( str, buffer, len+1 );
	return str;
}



/**
 * si_getString - reads a line from stdin into a heap-allocated si_string
 * 
 * NOTE: 	Caller is responsible for freeing the returned buffer:
 * 			free(str.data);
 * 
 * Safe usage example:
 * 
 * 		si_string str = si_getString();
 *
 *		if (!str.data) return;
 *		printf("%.*s", (int)str.len, str.data)
 *		free(str.data);
 *		str.data = NULL;
 *
 * NOTE: 	Uses memcpy() to copy exactly len bytes from internal buffer
 * 			No null terminator is appended-use the length field for safe access.
 * 
 * Returns:
 * 		A si_string with .data == NULL and .len == 0 on error or EOF,
 * 		otherwise .data points to malloc( len ? len : 1 ) and .len is the byte count.
 */
si_string si_getString ( void ) {

	char buffer[INPUT_BUFFER_SIZE];
	size_t len;

	// read input ( return NULL on error/EOF )
	if ( si_readByte( buffer, sizeof(buffer), &len )) return (si_string){ NULL, 0 };

	// Allocate memory for string data, length is stored in len parameter
	si_string str;
	str.data = malloc( len ? len : 1 );
	if ( !str.data ) {
		si_printError( "Memory allocation failed.\n" );
		return (si_string){ NULL, 0 };
	}

	// copy buffer to string data
	memcpy( str.data, buffer, len );
	str.len = len;
	return str;
}



/**
 * si_getBool - a safer alternative to scanf for boolean values
 *
 * Accepts "y" or "n" (case-insensitive).
 *
 * usage - bool x = si_getBool();
 * 
 * returns false on EOF. Terrible solution but it's the best i could figure out.
 */
bool si_getBool ( void ) {

	while ( 1 ) {
		// Read single character, convert to lowercase
		int c = si_getChar();

		if ( c == EOF ) {
			si_printError( "EOF detected. Returning false by default.\n" );
			return false;
		}

		// we avoid using tolower() due to EOF potentially triggering UB
		if ( c == 'Y' || c == 'y' ) return true;
		if ( c == 'N' || c == 'n' ) return false;
		si_printError( "Invalid input. Enter 'y' or 'n'.\n" );
	}
}
