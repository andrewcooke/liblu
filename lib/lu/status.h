
#ifndef LU_STATUS_H
#define LU_STATUS_H

/**
 * @file
 *
 * Macros for consistent status and cleanup.
 *
 * This module builds on the following common C-language practices:
 *
 * * resources that will be freed on exit are declared as `NULL`;
 *
 * * most functions have a single exit block where resources are freed;
 *
 * * most functions return an integer status;
 *
 * * on success, the status returned is "false" (ie zero);
 *
 * * if a call returns an error, execution jumps to the exit block;
 *
 * * if a call returns an error, that same error is returned from the caller.
 *
 * Together these implement a form of "exception handling" (where the status
 * value is the "exception type") that avoids memory leaks (`free()` can
 * be called on resources and `NULL` values are ignored).
 *
 * Note that macros intended for use on a single line include the trailing
 * semicolon.
 *
 * These features are optional, but some macros may assume that they are
 * used (eg in `dynamic_memory.h`).  In each case these are documented as
 * assuming these conventions.
 *
 * Here is an example of the most common macros in use:
 *
 *     int myfunction(...) {
 *         LU_STATUS  // declare the status and initialize to zero
 *         char *resource = NULL;  // a resource used internally
 *         if (!(resource = malloc(...))) {  // allocate the resource
 *             status = LU_ERR_MEM;
 *             goto exit;
 *         }
 *         LU_CHECK(subroutine(...))  // call subroutine and check status
 *         ...  // do more useful work
 *         LU_EXIT // declare the exit block
 *         free(resource);
 *         LU_RETURN  // return the status value
 *     }
 *
 * If `subroutine` returns an error then "do more useful work" is skipped,
 * the resource is freed, and the error returned from to the caller (because
 * `LU_CHECK()` updates `status` and jumps to `exit`).
 *
 * @ingroup lu
 */
/// @{

/// Introduce the status value; typically used as the first line in a function.
#define LU_STATUS int status = LU_OK;
/// Mark the start of the cleanup code
#define LU_CLEANUP exit:
/// Return the status value; typically used as the last line in a function.
#define LU_RETURN return status;

/// Shorthand for an empty cleanup section and status return.
#define LU_NO_CLEANUP LU_CLEANUP LU_RETURN

/// Return the current status, if set, otherwise the previous status; typically used in free()-like functions.
#define LU_RETURN2(prev) return prev ? prev : status;
/// Shorthand for an empty cleanup section and previous status return.
#define LU_NO_CLEANUP2(prev) LU_CLEANUP LU_RETURN2(prev)

/// If `stmt` fails (ie returns a status that is true) then go to cleanup.
#define LU_CHECK(stmt) if ((status = stmt)) {goto exit;}
/// Update status only if it was not already set; typically used to handle errors in the cleanup block.
#define LU_CLEAN(stmt) {int _status = stmt; status = status ? status : _status;}

/// If `value` is not true then set status to `error`, log the message, and go to cleanup.
#define LU_ASSERT(value, error, log, ...) if (!(value)) {luerror(log, __VA_ARGS__); status = error; goto exit;}

/// @}
#endif
