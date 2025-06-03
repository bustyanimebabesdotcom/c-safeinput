// safeinput.h - version 1.1.0

#ifndef SAFEINPUT_H_
#define SAFEINPUT_H_

// === Includes ===
#include <stdbool.h>
#include <stddef.h> // for size_t

#ifdef __cplusplus
extern "C" {
#endif

// === STRUCTs ===
typedef struct si_string {
	char	*data;	// pointer to data
	size_t	len;	// byte counter
} si_string;

// === INPUT BUFFER ===
#define INPUT_BUFFER_SIZE				128
#define CHAR_INPUT_BUFFER_SIZE			4

// === Input handling ===
int	si_getInt						( void );
unsigned int si_getUInt				( void );

float si_getFloat					( void );
double si_getDouble					( void );

long si_getLong						( void );
unsigned long si_getULong			( void );
long long si_getLongLong			( void );
unsigned long long si_getULongLong	( void );

int si_getChar						( void );
int si_getCharFiltered				( const char *allowed );

char *si_getCString					( void );
si_string si_getString				( void );

bool si_getBool						( void );

#ifdef __cplusplus
}
#endif

#endif // SAFEINPUT_H_
