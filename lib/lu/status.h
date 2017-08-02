
#ifndef LU_STATUS_H
#define LU_STATUS_H

/**
 * @defgroup lu_status Macros for consistent status and cleanup.
 *
 * kopd floop
 * poop
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
