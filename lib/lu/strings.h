
#ifndef LU_STRINGS_H
#define LU_STRINGS_H

#include <stdarg.h>

#include "memory.h"

/**
 * @file
 *
 * Dynamically allocated strings with common print and append functions.
 *
 * A `lustr` instance is intended to be declared directly (not as a pointer).
 * To initialise as empty (so that `lustr_free()` can be called safely
 * during cleanup) use the idiom:
 *
 *     lustr mystring = {};
 *     ...
 *     exit:
 *     status = lustr_free(&mystring, status);
 *     return status;
 *
 * Because the idiom above allocates the `mem` data on the stack, other lulib
 * routines that mutate strings can be treated as returning "just" the
 * character array:
 *
 *     lustr tmp = {};
 *     status = lu_someroutine(..., &tmp, ...);
 *     printf("all we care about is tmp.c, value %s", tmp.c);
 *     // we can even free tmp.c and ignore tmp
 *     free(tmp.c);
 *
 * So even though other routines may take `lustr*` arguments, they need not
 * "pollute" the rest of the code.
 *
 * However, there is no function to "wrap" an existing character array
 * because if the memory is re-allocated then ownership may be broken.
 *
 * See `lustr_snprintf()` and `lustr_snappendf()` for full argument details.
 */

struct lulog;

/// A string instance combines a `char*` pointer with a `lumem` structure for tracking memory allocation.
typedef struct lustr {
	char *c;  ///< The character data.
	lumem mem;  ///< Memory allocation data.
} lustr;

/// Construct an empty string.
int lustr_mk(struct lulog *log, lustr *str);
/// Construct an empty string with a given amount of memory already allocated.
int lustr_mkn(struct lulog *log, lustr *str, size_t n);
/// Construct a string, copying the character data given.
int lustr_mkstr(struct lulog *log, lustr *str, const char *c);
/// Free the memory associated with a string.
/** This does *not* free the instance itself - strings are intended to be declared as instances, not pointers. */
int lustr_free(lustr *str, int status);
/// Reserve memory for the string.
/** After calling, the string will have space for at least `n` more characters without further allocation. */
int lustr_reserve(struct lulog *log, lustr *str, int n);
/// Set the character data to the empty string (but do not free the memory).
int lustr_clear(struct lulog *log, lustr *str);

/// Replace a string with a char array.
int lustr_print(struct lulog *log, lustr *str, const char *text);
/// Replace a string with a char array, adding at most `max_size` characters.
int lustr_nprint(struct lulog *log, lustr *str, int max_size, int *all_chars, const char *text);
/// Replace a string with formatted data.
int lustr_printf(struct lulog *log, lustr *str, const char *format, ...);
/// Replace a string with formatted data adding at most `max_size` characters.
/**
 * @param[in] log Destination for error messages.
 * @param[in,out] str The string to modify.
 * @param[in] max_size The maximum number of characters to add (the string is always null-terminated), or -1 for no limit.
 * @param[out] all_chars The maximum number of characters that would be written if max_size had no effect (so output is truncated if this is larger than max_size), or `NULL`.
 * @param[in] format The format to use.
 * @param[in] ... The data to format.
 */
int lustr_nprintf(struct lulog *log, lustr *str, int max_size, int *all_chars, const char *format, ...);
/// Replace a string with formatted varags data.
int lustr_vprintf(struct lulog *log, lustr *str, const char *format, va_list ap);
/// Replace a string with formatted varags data adding at most `max_size` characters.
int lustr_vnprintf(struct lulog *log, lustr *str, int max_size, int *all_chars, const char *format, va_list ap);

/// Add a character to a string.
int lustr_add(struct lulog *log, lustr *str, char c);
/// Append a character array to a string.
int lustr_append(struct lulog *log, lustr *str, const char *text);
/// Append a character array to a string, adding at most `max_size` characters.
int lustr_nappend(struct lulog *log, lustr *str, int max_size, int *all_chars, const char *text);
/// Append formatted data array to a string.
int lustr_appendf(struct lulog *log, lustr *str, const char *format, ...);
/// Append formatted data array to a string, adding at most `max_size` characters.
/**
 * @param[in] log Destination for error messages.
 * @param[in,out] str The string to modify.
 * @param[in] max_size The maximum number of characters to add (the string is always null-terminated), or -1 for no limit.
 * @param[out] all_chars The maximum number of characters that would be written if max_size had no effect (so output is truncated if this is larger than max_size), or `NULL`.
 * @param[in] format The format to use.
 * @param[in] ... The data to format.
 */
int lustr_nappendf(struct lulog *log, lustr *str, int max_size, int *all_chars, const char *format, ...);
/// Append formatted varargs data array to a string.
int lustr_vappendf(struct lulog *log, lustr *str, const char *format, va_list ap);
/// Append formatted varargs data array to a string, adding at most `max_size` characters.
int lustr_vnappendf(struct lulog *log, lustr *str, int max_size, int *all_chars, const char *format, va_list ap);

#endif
