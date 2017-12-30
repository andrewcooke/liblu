
#ifndef LU_INTERNAL_H
#define LU_INTERNAL_H

/**
 * @file
 *
 * @brief Intrusive support for consistent status and cleanup.
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
 * The macros here assume that the exit block is labelled "finally".  They
 * have short, non-namespaced names because, if used, they will be ubiquitous
 * and reflect a wholesale adoption of the entire library.
 *
 * Here is an example of the macros in use:
 *
 *     int myfunction(...) {
 *         int status = LU_OK;  // declare the status and initialize to zero
 *         char *resource = NULL;  // a resource used internally
 *         assert(resource = malloc(...),
 *                LU_ERR_MEM, log, "Could not allocate resource");
 *         try(subroutine(...));  // call subroutine and check status
 *         ...  // do more useful work
 *         finally:
 *         free(resource);
 *         return status;
 *     }
 *
 * If `subroutine` returns an error then "do more useful work" is skipped,
 * the resource is freed, and the error returned from to the caller (because
 * `try()` updates `status` and jumps to `exit`).
 *
 * @ingroup lu
 */
/// @{

/// If `stmt` fails (ie returns a status that is true) then go to cleanup.
#define try(stmt) if ((status = stmt)) {goto finally;}

/// If `value` is not true then set status to `error`, log the message, and go to cleanup.
#define assert(value, error, log, ...) if (!(value)) {luerror(log, __VA_ARGS__); status = error; goto finally;}

/// Return the previous state, if it was not OK, otherwise the latest.
int lu_both(int prev_status, int status);

/// @}
#endif
