
#ifndef LU_STATUS_H
#define LU_STATUS_H

#define LU_STATUS int status = LU_OK;
#define LU_CLEANUP exit:
#define LU_RETURN return status;
#define LU_NO_CLEANUP LU_CLEANUP LU_RETURN

#define LU_RETURN2(prev) return prev ? prev : status;
#define LU_NO_CLEANUP2(prev) LU_CLEANUP LU_RETURN2(prev)

#define LU_CHECK(stmt) if ((status = stmt)) {goto exit;}

#define LU_ASSERT(value, message, error) if (value) {luerror(log, message); status = error; goto exit;}

#endif
