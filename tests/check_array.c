
#include <check.h>
#include <stdlib.h>

#include "../lib/lu.h"
#include "../lib/lulog.h"
#include "../lib/luarray.h"


START_TEST(test_xy) {
    lulog *log;
    ck_assert(!lulog_mkstderr(&log, lulog_level_debug));
    luarray_xy *xy;
    ck_assert(!luarray_mkxyn(log, &xy, 0));
    ck_assert(!luarray_pushxy(log, xy, 1, 2));
    ck_assert(!luarray_pushxy(log, xy, 3, 4));
    ck_assert(xy->mem.used == 2);
    ck_assert(xy->xy[0].x == 1);
    ck_assert(xy->xy[0].y == 2);
    ck_assert(xy->xy[1].x == 3);
    ck_assert(xy->xy[1].y == 4);
    ck_assert(!luarray_freexy(&xy, 0));
    ck_assert(!log->free(&log, 0));
} END_TEST


int main(void) {

    int failed = 0;
    Suite *s;
    TCase *c;
    SRunner *r;

    c = tcase_create("case");
    tcase_add_test(c, test_xy);
    s = suite_create("suite");
    suite_add_tcase(s, c);
    r = srunner_create(s);
    srunner_run_all(r, CK_VERBOSE);
    failed = srunner_ntests_failed(r);
    srunner_free(r);

    return failed ? 1 : 0;
}
