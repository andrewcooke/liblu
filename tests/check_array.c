
#include <check.h>
#include <stdlib.h>

#include "../lib/lu/lu.h"
#include "../lib/lu/lulog.h"
#include "../lib/lu/luarray.h"


START_TEST(test_ijz) {
    lulog *log;
    ck_assert(!lulog_mkstderr(&log, lulog_level_debug));
    luarray_ijz *ijz;
    ck_assert(!luarray_mkijzn(log, &ijz, 0));
    ck_assert(!luarray_pushijz(log, ijz, 1, 2, 3.0));
    ck_assert(!luarray_pushijz(log, ijz, 4, 5, 6.0));
    ck_assert(ijz->mem.used == 2);
    ck_assert(ijz->ijz[0].i == 1);
    ck_assert(ijz->ijz[0].j == 2);
    ck_assert(ijz->ijz[0].z == 3);
    ck_assert(ijz->ijz[1].i == 4);
    ck_assert(ijz->ijz[1].j == 5);
    ck_assert(ijz->ijz[1].z == 6);
    ck_assert(!luarray_freeijz(&ijz, 0));
    ck_assert(!log->free(&log, 0));
} END_TEST


int main(void) {

    int failed = 0;
    Suite *s;
    TCase *c;
    SRunner *r;

    c = tcase_create("case");
    tcase_add_test(c, test_ijz);
    s = suite_create("suite");
    suite_add_tcase(s, c);
    r = srunner_create(s);
    srunner_run_all(r, CK_VERBOSE);
    failed = srunner_ntests_failed(r);
    srunner_free(r);

    return failed ? 1 : 0;
}
