
#include <check.h>
#include <lib/lu/status_codes.h>
#include <stdlib.h>

#include "../lib/lu/log.h"
#include "../lib/lu/arrays.h"
#include "../lib/lu/array_macros.h"

typedef double type;

typedef struct {
    lumem mem;
    type *member;
} luary_stem;

LUARY_MKBASE(stem, type, member)

START_TEST(test_atype) {
    lulog *log;
    ck_assert(!lulog_mkstderr(&log, lulog_level_debug));
    luary_stem *array;
    ck_assert(!luary_mkstem(log, &array, 0));
    ck_assert(!array->mem.capacity);
    ck_assert(!array->mem.used);
    ck_assert(!luary_reservestem(log, array, 10));
    ck_assert(array->mem.capacity >= 10);
    ck_assert(!array->mem.used);
    ck_assert(!luary_freestem(&array, 0));
    ck_assert(!log->free(&log, 0));
} END_TEST

START_TEST(test_ijz) {
    lulog *log;
    ck_assert(!lulog_mkstderr(&log, lulog_level_debug));
    luary_ijz *ijz;
    ck_assert(!luary_mkijz(log, &ijz, 0));
    ck_assert(!luary_pushijz(log, ijz, 1, 2, 3.0));
    ck_assert(!luary_pushijz(log, ijz, 4, 5, 6.0));
    ck_assert(ijz->mem.used == 2);
    ck_assert(ijz->ijz[0].i == 1);
    ck_assert(ijz->ijz[0].j == 2);
    ck_assert(ijz->ijz[0].z == 3);
    ck_assert(ijz->ijz[1].i == 4);
    ck_assert(ijz->ijz[1].j == 5);
    ck_assert(ijz->ijz[1].z == 6);
    ck_assert(!luary_freeijz(&ijz, 0));
    ck_assert(!log->free(&log, 0));
} END_TEST


int main(void) {

    int failed = 0;
    Suite *s;
    TCase *c;
    SRunner *r;

    c = tcase_create("case");
    tcase_add_test(c, test_atype);
    tcase_add_test(c, test_ijz);
    s = suite_create("suite");
    suite_add_tcase(s, c);
    r = srunner_create(s);
    srunner_run_all(r, CK_VERBOSE);
    failed = srunner_ntests_failed(r);
    srunner_free(r);

    return failed ? 1 : 0;
}
