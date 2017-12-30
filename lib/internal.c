
#include "lu/internal.h"


int lu_both(int prev_status, int status) {
	return prev_status ? prev_status : status;
}
